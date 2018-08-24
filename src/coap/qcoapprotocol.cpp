/****************************************************************************
**
** Copyright (C) 2017 Witekio.
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCoap module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qrandom.h>
#include <QtCore/qthread.h>
#include <QtNetwork/qnetworkdatagram.h>
#include "qcoapprotocol_p.h"
#include "qcoapinternalrequest_p.h"
#include "qcoapinternalreply_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCoapProtocol
    \brief The QCoapProtocol class handles the logical part of the CoAP
    protocol.

    \reentrant

    The QCoapProtocol is used by the QCoapClient class to handle the logical
    part of the protocol. It can encode requests and decode replies. It also
    handles what to do when a message is received, along with retransmission of
    lost messages.

    \sa QCoapClient
*/

/*!
    Constructs a new QCoapProtocol and sets \a parent as the parent object.
*/
QCoapProtocol::QCoapProtocol(QObject *parent) :
    QObject(*new QCoapProtocolPrivate, parent)
{
    qRegisterMetaType<QCoapInternalRequest *>();
    qRegisterMetaType<QHostAddress>();
}

QCoapProtocol::~QCoapProtocol()
{
    Q_D(QCoapProtocol);

    // Clear table to avoid double deletion from QObject parenting and QSharedPointer.
    d->exchangeMap.clear();
}

/*!
    Creates and sets up a new QCoapInternalRequest related to the request
    associated to the \a reply. The request will then be sent to the server
    using the given \a connection.
*/
void QCoapProtocol::sendRequest(QPointer<QCoapReply> reply, QCoapConnection *connection)
{
    Q_D(QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == thread());

    if (reply.isNull() || !reply->request().isValid())
        return;

    auto internalRequest = QSharedPointer<QCoapInternalRequest>::create(reply->request(), this);
    internalRequest->setMaxTransmissionWait(maxTransmitWait());
    connect(reply, &QCoapReply::finished, this, &QCoapProtocol::finished);

    // Set a unique Message Id and Token
    QCoapMessage *requestMessage = internalRequest->message();
    internalRequest->setMessageId(d->generateUniqueMessageId());
    internalRequest->setToken(d->generateUniqueToken());
    internalRequest->setConnection(connection);

    d->registerExchange(requestMessage->token(), reply, internalRequest);
    QMetaObject::invokeMethod(reply, "_q_setRunning", Qt::QueuedConnection,
                              Q_ARG(QCoapToken, requestMessage->token()),
                              Q_ARG(QCoapMessageId, requestMessage->messageId()));

    // Set block size for blockwise request/replies, if specified
    if (d->blockSize > 0) {
        internalRequest->setToRequestBlock(0, d->blockSize);
        if (requestMessage->payload().length() > d->blockSize)
            internalRequest->setToSendBlock(0, d->blockSize);
    }

    if (requestMessage->type() == QCoapMessage::Confirmable)
        internalRequest->setTimeout(QtCoap::randomGenerator.bounded(minTimeout(), maxTimeout()));
    else
        internalRequest->setTimeout(maxTimeout());

    connect(internalRequest.data(), SIGNAL(timeout(QCoapInternalRequest *)),
            this, SLOT(onRequestTimeout(QCoapInternalRequest *)));
    connect(internalRequest.data(), SIGNAL(maxTransmissionSpanReached(QCoapInternalRequest *)),
            this, SLOT(onRequestMaxTransmissionSpanReached(QCoapInternalRequest *)));

    d->sendRequest(internalRequest.data());
}

/*!
    \internal

    Encodes and sends the given \a request to the server.
*/
void QCoapProtocolPrivate::sendRequest(QCoapInternalRequest *request)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    if (!request || !request->connection()) {
        qWarning("QtCoap: Request null or not bound to any connection: aborted.");
        return;
    }

    request->restartTransmission();
    QByteArray requestFrame = encode(request);
    QUrl uri = request->targetUri();
    request->connection()->sendRequest(requestFrame, uri.host(), static_cast<quint16>(uri.port()));
}

/*!
    \internal

    This slot is used to send again the given \a request after a timeout or
    aborts the request and transfers a timeout error to the reply.
*/
void QCoapProtocolPrivate::onRequestTimeout(QCoapInternalRequest *request)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    if (!isRequestRegistered(request))
        return;

    if (request->message()->type() == QCoapMessage::Confirmable
            && request->retransmissionCounter() < maxRetransmit) {
        sendRequest(request);
    } else {
        onRequestError(request, QtCoap::TimeOutError);
    }
}

/*!
    \internal

    This slot is called when the maximum span for this transmission has been
    reached, and triggers a timeout error if the request is still running.
*/
void QCoapProtocolPrivate::onRequestMaxTransmissionSpanReached(QCoapInternalRequest *request)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    if (isRequestRegistered(request))
        onRequestError(request, QtCoap::TimeOutError);
}

/*!
    \internal

    Method triggered when a request fails.
*/
void QCoapProtocolPrivate::onRequestError(QCoapInternalRequest *request, QCoapInternalReply *reply)
{
    QtCoap::Error error = QtCoap::responseCodeError(reply->responseCode());
    onRequestError(request, error, reply);
}

/*!
    \internal

    Method triggered when a request fails.
*/
void QCoapProtocolPrivate::onRequestError(QCoapInternalRequest *request, QtCoap::Error error,
                                          QCoapInternalReply *reply)
{
    Q_Q(QCoapProtocol);
    Q_ASSERT(request);

    auto userReply = userReplyForToken(request->token());

    if (!userReply.isNull()) {
        // Set error from content, or error enum
        if (reply) {
            QMetaObject::invokeMethod(userReply.data(), "_q_setContent", Qt::QueuedConnection,
                                      Q_ARG(QHostAddress, reply->senderAddress()),
                                      Q_ARG(QCoapMessage, *reply->message()),
                                      Q_ARG(QtCoap::ResponseCode, reply->responseCode()));
        } else {
            QMetaObject::invokeMethod(userReply.data(), "_q_setError", Qt::QueuedConnection,
                                      Q_ARG(QtCoap::Error, error));
        }

        QMetaObject::invokeMethod(userReply.data(), "_q_setFinished", Qt::QueuedConnection,
                                  Q_ARG(QtCoap::Error, QtCoap::NoError));
    }

    forgetExchange(request);
    emit q->error(userReply.data(), error);
}
/*!
    \internal

    Decode and process the given \a frame after reception.
*/
void QCoapProtocolPrivate::onFrameReceived(const QNetworkDatagram &frame)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    QSharedPointer<QCoapInternalReply> reply(decode(frame));
    const QCoapMessage *messageReceived = reply->message();

    QCoapInternalRequest *request = nullptr;
    if (!messageReceived->token().isEmpty())
        request = requestForToken(messageReceived->token());

    if (!request) {
        request = findRequestByMessageId(messageReceived->messageId());

        // No matching request found, drop the frame.
        if (!request)
            return;
    }

    QHostAddress originalTarget(request->targetUri().host());
    if (!originalTarget.isMulticast() && !originalTarget.isEqual(frame.senderAddress())) {
        qDebug().nospace() << "QtCoap: Answer received from incorrect host ("
                           << frame.senderAddress() << " instead of "
                           << originalTarget << ")";
        return;
    }

    request->stopTransmission();
    addReply(request->token(), reply);

    if (QtCoap::isError(reply->responseCode())) {
        onRequestError(request, reply.data());
        return;
    }

    // Reply when the server asks for an ACK
    if (request->isObserveCancelled()) {
        // Remove option to ensure that it will stop
        request->removeOption(QCoapOption::Observe);
        sendReset(request);
    } else if (messageReceived->type() == QCoapMessage::Confirmable) {
        sendAcknowledgment(request);
    }

    // Send next block, ask for next block, or process the final reply
    if (reply->hasMoreBlocksToSend()) {
        request->setToSendBlock(reply->nextBlockToSend(), blockSize);
        request->setMessageId(generateUniqueMessageId());
        sendRequest(request);
    } else if (reply->hasMoreBlocksToReceive()) {
        request->setToRequestBlock(reply->currentBlockNumber() + 1, reply->blockSize());
        request->setMessageId(generateUniqueMessageId());
        sendRequest(request);
    } else {
        onLastMessageReceived(request);
    }
}

/*!
    \internal

    Returns the internal request for the given \a token.
*/
QCoapInternalRequest *QCoapProtocolPrivate::requestForToken(const QByteArray &token)
{
    auto it = exchangeMap.find(token);
    if (it != exchangeMap.constEnd())
        return it->request.data();

    return nullptr;
}

/*!
    \internal

    Returns the QCoapReply instance of the given \a token.
*/
QPointer<QCoapReply> QCoapProtocolPrivate::userReplyForToken(const QCoapToken &token)
{
    auto it = exchangeMap.find(token);
    if (it != exchangeMap.constEnd())
        return it->userReply;

    return nullptr;
}

/*!
    \internal

    Returns the replies for the exchange identified by \a token.
*/
QVector<QSharedPointer<QCoapInternalReply> > QCoapProtocolPrivate::repliesForToken(
    const QCoapToken &token)
{
    auto it = exchangeMap.find(token);
    if (it != exchangeMap.constEnd())
        return it->replies;

    return {};
}

/*!
    \internal

    Returns the last reply for the exchange identified by \a token.
*/
QCoapInternalReply *QCoapProtocolPrivate::lastReplyForToken(const QCoapToken &token)
{
    auto it = exchangeMap.find(token);
    if (it != exchangeMap.constEnd())
        return it->replies.last().data();

    return nullptr;
}

/*!
    \internal

    Finds an internal request matching the given \a reply.
*/
QCoapInternalRequest *QCoapProtocolPrivate::findRequestByUserReply(const QCoapReply *reply)
{
    for (auto it = exchangeMap.constBegin(); it != exchangeMap.constEnd(); ++it) {
        if (it->userReply == reply)
            return it->request.data();
    }

    return nullptr;
}

/*!
    \internal

    Finds an internal request containing the message id \a messageId.
*/
QCoapInternalRequest *QCoapProtocolPrivate::findRequestByMessageId(quint16 messageId)
{
    for (auto it = exchangeMap.constBegin(); it != exchangeMap.constEnd(); ++it) {
        if (it->request->message()->messageId() == messageId)
            return it->request.data();
    }

    return nullptr;
}

/*!
    \internal

    Handles what to do when we received the last block of a reply.

    Merges all blocks, removes the request from the map, updates the
    associated QCoapReply and emits the
    \l{QCoapProtocol::finished(QCoapReply*)}{finished(QCoapReply*)} signal.
*/
void QCoapProtocolPrivate::onLastMessageReceived(QCoapInternalRequest *request)
{
    Q_ASSERT(request);
    if (!request || !isRequestRegistered(request))
        return;

    auto replies = repliesForToken(request->token());
    Q_ASSERT(!replies.isEmpty());

    //! TODO: Change QPointer<QCoapReply> into something independent from
    //! User. QSharedPointer(s)?
    QPointer<QCoapReply> userReply = userReplyForToken(request->token());
    if (userReply.isNull() || replies.isEmpty()
            || (request->isObserve() && request->isObserveCancelled())) {
        forgetExchange(request);
        return;
    }

    auto lastReply = replies.last();
    // Ignore empty ACK messages
    if (lastReply->message()->type() == QCoapMessage::Acknowledgment
            && lastReply->responseCode() == QtCoap::EmptyMessage) {
        exchangeMap[request->token()].replies.takeLast();
        return;
    }

    // Merge payloads for blockwise transfers
    if (replies.size() > 1) {
        std::stable_sort(std::begin(replies), std::end(replies),
        [](QSharedPointer<QCoapInternalReply> a, QSharedPointer<QCoapInternalReply> b) -> bool {
            return (a->currentBlockNumber() < b->currentBlockNumber());
        });

        QByteArray finalPayload;
        int lastBlockNumber = -1;
        for (auto reply : qAsConst(replies)) {
            int currentBlock = static_cast<int>(reply->currentBlockNumber());
            QByteArray replyPayload = reply->message()->payload();
            if (replyPayload.isEmpty() || currentBlock <= lastBlockNumber)
                continue;

            finalPayload.append(replyPayload);
            lastBlockNumber = currentBlock;
        }

        lastReply->message()->setPayload(finalPayload);
    }

    // Forward the answer
    QMetaObject::invokeMethod(userReply, "_q_setContent", Qt::QueuedConnection,
                              Q_ARG(QHostAddress, lastReply->senderAddress()),
                              Q_ARG(QCoapMessage, *lastReply->message()),
                              Q_ARG(QtCoap::ResponseCode, lastReply->responseCode()));

    if (request->isObserve()) {
        QMetaObject::invokeMethod(userReply, "_q_setNotified", Qt::QueuedConnection);
        forgetExchangeReplies(request->token());
    } else {
        QMetaObject::invokeMethod(userReply, "_q_setFinished", Qt::QueuedConnection,
                                  Q_ARG(QtCoap::Error, QtCoap::NoError));
        forgetExchange(request);
    }
}

/*!
    \internal

    Sends an internal request acknowledging the given \a request, reusing its
    URI and connection.
*/
void QCoapProtocolPrivate::sendAcknowledgment(QCoapInternalRequest *request)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    QCoapInternalRequest ackRequest;
    ackRequest.setTargetUri(request->targetUri());

    auto internalReply = lastReplyForToken(request->token());
    ackRequest.initForAcknowledgment(internalReply->message()->messageId(),
                                     internalReply->message()->token());
    ackRequest.setConnection(request->connection());
    sendRequest(&ackRequest);
}

/*!
    \internal

    Sends a Reset message (RST), reusing the details of the given
    \a request. A Reset message indicates that a specific message has been
    received, but cannot be properly processed.
*/
void QCoapProtocolPrivate::sendReset(QCoapInternalRequest *request)
{
    Q_Q(const QCoapProtocol);
    Q_ASSERT(QThread::currentThread() == q->thread());

    QCoapInternalRequest resetRequest;
    resetRequest.setTargetUri(request->targetUri());

    auto lastReply = lastReplyForToken(request->token());
    resetRequest.initForReset(lastReply->message()->messageId());
    resetRequest.setConnection(request->connection());
    sendRequest(&resetRequest);
}

/*!
    Cancels resource observation. The QCoapReply::notified() signal will not
    be emitted after cancellation.

    A Reset (RST) message will be sent at the reception of the next message.
*/
void QCoapProtocol::cancelObserve(QPointer<QCoapReply> reply)
{
    Q_D(QCoapProtocol);

    if (reply.isNull())
        return;

    QCoapInternalRequest *request = d->requestForToken(reply->request().token());
    if (request) {
        // Stop here if already cancelled
        if (!request->isObserve() || request->isObserveCancelled())
            return;

        request->setObserveCancelled();
    }

    // Set as cancelled even if request is not tracked anymore
    QMetaObject::invokeMethod(reply, "_q_setObserveCancelled", Qt::QueuedConnection);
}

/*!
    \internal

    Returns a currently unused message Id.
*/
quint16 QCoapProtocolPrivate::generateUniqueMessageId() const
{
    // TODO: Optimize message id generation for large sets
    // TODO: Store used message id for the period specified by CoAP spec
    quint16 id = 0;
    while (isMessageIdRegistered(id))
        id = static_cast<quint16>(QtCoap::randomGenerator.bounded(0x10000));

    return id;
}

/*!
    \internal

    Returns a currently unused token.
*/
QCoapToken QCoapProtocolPrivate::generateUniqueToken() const
{
    // TODO: Optimize token generation for large sets
    // TODO: Store used token for the period specified by CoAP spec
    QCoapToken token;
    while (isTokenRegistered(token)) {
        // TODO: Allow setting minimum token size as a security setting
        quint8 length = static_cast<quint8>(QtCoap::randomGenerator.bounded(1, 8));

        token.resize(length);
        quint8 *tokenData = reinterpret_cast<quint8 *>(token.data());
        for (int i = 0; i < token.size(); ++i)
            tokenData[i] = static_cast<quint8>(QtCoap::randomGenerator.bounded(256));
    }

    return token;
}

/*!
    \internal

    Encodes the \a request to a QByteArray frame.
*/
QByteArray QCoapProtocolPrivate::encode(QCoapInternalRequest *request)
{
    return request->toQByteArray();
}

/*!
    \internal

    Decodes the \a frame and returns a new unmanaged
    QCoapInternalReply object.
*/
QCoapInternalReply *QCoapProtocolPrivate::decode(const QNetworkDatagram &frame)
{
    Q_Q(QCoapProtocol);
    QCoapInternalReply *reply = QCoapInternalReply::createFromFrame(frame.data(), q);
    reply->setSenderAddress(frame.senderAddress());

    return reply;
}

/*!
    \internal

    Aborts the request corresponding to the given \a reply. It is triggered
    by the destruction of the QCoapReply object or a call to
    QCoapReply::abortRequest().
*/
void QCoapProtocolPrivate::onRequestAborted(const QCoapToken &token)
{
    QCoapInternalRequest *request = requestForToken(token);
    if (!request)
        return;

    request->stopTransmission();
    forgetExchange(request);
}

/*!
    \internal

    Triggered when an error occurs in the QCoapConnection.
*/
void QCoapProtocolPrivate::onConnectionError(QAbstractSocket::SocketError socketError)
{
    Q_Q(QCoapProtocol);

    QtCoap::Error coapError;
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError :
        coapError = QtCoap::HostNotFoundError;
        break;
    case QAbstractSocket::AddressInUseError :
        coapError = QtCoap::AddressInUseError;
        break;
    default:
        coapError = QtCoap::UnknownError;
        break;
    }

    emit q->error(nullptr, coapError);
}

/*!
    Decodes the \a data to a list of QCoapResource objects.
    The \a data byte array is a frame returned by a discovery request.
*/
QVector<QCoapResource> QCoapProtocol::resourcesFromCoreLinkList(const QHostAddress &sender,
                                                                const QByteArray &data)
{
    QVector<QCoapResource> resourceList;

    QLatin1String quote = QLatin1String("\"");
    const QList<QByteArray> links = data.split(',');
    for (QByteArray link : links) {
        QCoapResource resource;
        resource.setHost(sender);

        const QList<QByteArray> parameterList = link.split(';');
        for (QByteArray parameter : parameterList) {
            QString parameterString = QString::fromUtf8(parameter);
            int length = parameterString.length();
            if (parameter.startsWith('<'))
                resource.setPath(parameterString.mid(1, length - 2));
            else if (parameter.startsWith("title="))
                resource.setTitle(parameterString.mid(6).remove(quote));
            else if (parameter.startsWith("rt="))
                resource.setResourceType(parameterString.mid(3).remove(quote));
            else if (parameter.startsWith("if="))
                resource.setInterface(parameterString.mid(3).remove(quote));
            else if (parameter.startsWith("sz="))
                resource.setMaximumSize(parameterString.mid(3).remove(quote).toInt());
            else if (parameter.startsWith("ct="))
                resource.setContentFormat(parameterString.mid(3).remove(quote).toUInt());
            else if (parameter == "obs")
                resource.setObservable(true);
        }

        if (!resource.path().isEmpty())
            resourceList.push_back(resource);
    }

    return resourceList;
}

/*!
    \internal

    Registers a new CoAP exchange using \a token.
*/
void QCoapProtocolPrivate::registerExchange(const QCoapToken &token, QCoapReply *reply,
                                            QSharedPointer<QCoapInternalRequest> request)
{
    CoapExchangeData data = { reply, request,
                              QVector<QSharedPointer<QCoapInternalReply> >()
                            };

    exchangeMap.insert(token, data);
}

/*!
    \internal

    Adds \a reply to the list of replies of the exchange identified by
    \a token.
    Returns \c true if the reply was successfully added. This method will fail
    and return \c false if no exchange is associated with the \a token
    provided.
*/
bool QCoapProtocolPrivate::addReply(const QCoapToken &token,
                                    QSharedPointer<QCoapInternalReply> reply)
{
    if (!isTokenRegistered(token) || !reply) {
        qWarning() << "QtCoap: Reply token '" << token << "' not registered, or reply is null.";
        return false;
    }

    exchangeMap[token].replies.push_back(reply);
    return true;
}

/*!
    \internal

    Remove the exchange identified by its \a token. This is
    typically done when finished or aborted.
    It will delete the QCoapInternalRequest and QCoapInternalReplies
    associated with the exchange.

    Returns \c true if the exchange was found and removed, \c false otherwise.
*/
bool QCoapProtocolPrivate::forgetExchange(const QCoapToken &token)
{
    return exchangeMap.remove(token) > 0;
}

/*!
    \internal

    Remove the exchange using a request.

    \sa forgetExchange(const QCoapToken &)
*/
bool QCoapProtocolPrivate::forgetExchange(const QCoapInternalRequest *request)
{
    return forgetExchange(request->token());
}

/*!
    \internal

    Remove all replies for the exchange corresponding to \a token.
*/
bool QCoapProtocolPrivate::forgetExchangeReplies(const QCoapToken &token)
{
    auto it = exchangeMap.find(token);
    if (it == exchangeMap.end())
        return false;

    it->replies.clear();
    return true;
}

/*!
    \internal

    Returns \c true if the \a token is reserved or in use; returns false if
    this token can be used to identify a new exchange.
*/
bool QCoapProtocolPrivate::isTokenRegistered(const QCoapToken &token) const
{
    // Reserved for empty messages and uninitialized tokens
    if (token == QByteArray())
        return true;

    return exchangeMap.contains(token);
}

/*!
    \internal

    Returns \c true if the \a request is present in a currently registered
    exchange.
*/
bool QCoapProtocolPrivate::isRequestRegistered(const QCoapInternalRequest *request) const
{
    for (auto it = exchangeMap.constBegin(); it != exchangeMap.constEnd(); ++it) {
        if (it->request.data() == request)
            return true;
    }

    return false;
}

/*!
    \internal

    Returns \c true if a request has a message id equal to \a id, or if \a id
    is reserved.
*/
bool QCoapProtocolPrivate::isMessageIdRegistered(quint16 id) const
{
    // Reserved for uninitialized message Id
    if (id == 0)
        return true;

    for (auto it = exchangeMap.constBegin(); it != exchangeMap.constEnd(); ++it) {
        if (it->request->message()->messageId() == id)
            return true;
    }

    return false;
}

/*!
    Returns the ACK_TIMEOUT value in milliseconds.
    The default is 2000.

    \sa minTimeout(), setAckTimeout()
*/
int QCoapProtocol::ackTimeout() const
{
    Q_D(const QCoapProtocol);
    return d->ackTimeout;
}

/*!
    Returns the ACK_RANDOM_FACTOR value.
    The default is 1.5.

    \sa setAckRandomFactor()
*/
double QCoapProtocol::ackRandomFactor() const
{
    Q_D(const QCoapProtocol);
    return d->ackRandomFactor;
}

/*!
    Returns the MAX_RETRANSMIT value. This is the maximum number of
    retransmissions of a message, before notifying a timeout error.
    The default is 4.

    \sa setMaxRetransmit()
*/
int QCoapProtocol::maxRetransmit() const
{
    Q_D(const QCoapProtocol);
    return d->maxRetransmit;
}

/*!
    Returns the max block size wanted.
    The default is 0, which invites the server to choose the block size.

    \sa setBlockSize()
*/
quint16 QCoapProtocol::blockSize() const
{
    Q_D(const QCoapProtocol);
    return d->blockSize;
}

/*!
    Returns the MAX_TRANSMIT_SPAN in milliseconds, as defined in
    \l{https://tools.ietf.org/search/rfc7252#section-4.8.2}{RFC 7252}.

    It is the maximum time from the first transmission of a Confirmable
    message to its last retransmission.
*/
int QCoapProtocol::maxTransmitSpan() const
{
    if (maxRetransmit() <= 0)
        return 0;

    return static_cast<int>(ackTimeout() * (1u << (maxRetransmit() - 1)) * ackRandomFactor());
}

/*!
    Returns the MAX_TRANSMIT_WAIT in milliseconds, as defined in
    \l{https://tools.ietf.org/search/rfc7252#section-4.8.2}{RFC 7252}.

    It is the maximum time from the first transmission of a Confirmable
    message to the time when the sender gives up on receiving an
    acknowledgment or reset.
*/
int QCoapProtocol::maxTransmitWait() const
{
    if (maxRetransmit() <= 0 || ackTimeout() <= 0)
        return 0;

    return static_cast<int>(static_cast<unsigned int>(ackTimeout())
                            * ((1u << (maxRetransmit() + 1)) - 1) * ackRandomFactor());
}

/*!
    Returns the MAX_LATENCY in milliseconds, as defined in
    \l{https://tools.ietf.org/search/rfc7252#section-4.8.2}{RFC 7252}. This
    value is arbitrarily set to 100 seconds by the standard.

    It is the maximum time a datagram is expected to take from the start of
    its transmission to the completion of its reception.
*/
constexpr int QCoapProtocol::maxLatency()
{
    return 100 * 1000;
}

/*!
    Returns the minimum duration for messages timeout. The timeout is defined
    as a random value between minTimeout() and maxTimeout(). This is a
    convenience method identical to ackTimeout().

    \sa ackTimeout(), setAckTimeout()
*/
int QCoapProtocol::minTimeout() const
{
    Q_D(const QCoapProtocol);
    return d->ackTimeout;
}

/*!
    Returns the maximum duration for messages timeout in milliseconds.

    \sa maxTimeout(), setAckTimeout(), setAckRandomFactor()
*/
int QCoapProtocol::maxTimeout() const
{
    Q_D(const QCoapProtocol);
    return static_cast<int>(d->ackTimeout * d->ackRandomFactor);
}

/*!
    Sets the ACK_TIMEOUT value to \a ackTimeout in milliseconds.
    The default is 2000 ms.

    Timeout only applies to Confirmable message. The actual timeout for
    reliable transmissions is a random value between ackTimeout() and
    ackTimeout() * ackRandomFactor().

    \sa ackTimeout(), setAckRandomFactor(), minTimeout(), maxTimeout()
*/
void QCoapProtocol::setAckTimeout(int ackTimeout)
{
    Q_D(QCoapProtocol);
    d->ackTimeout = ackTimeout;
}

/*!
    Sets the ACK_RANDOM_FACTOR value to \a ackRandomFactor. This value
    should be greater than or equal to 1.
    The default is 1.5.

    \sa ackRandomFactor(), setAckTimeout()
*/
void QCoapProtocol::setAckRandomFactor(double ackRandomFactor)
{
    Q_D(QCoapProtocol);
    if (ackRandomFactor < 1)
        qWarning() << "QtCoap: The Ack random factor should be >= 1";

    d->ackRandomFactor = qMax(1., ackRandomFactor);
}

/*!
    Sets the MAX_RETRANSMIT value to \a maxRetransmit, but never
    to more than 25.
    The default is 4.

    \sa maxRetransmit()
*/
void QCoapProtocol::setMaxRetransmit(int maxRetransmit)
{
    Q_D(QCoapProtocol);
    if (maxRetransmit < 0) {
        qWarning("QtCoap: Max retransmit cannot be negative.");
        return;
    }

    if (maxRetransmit > 25) {
        qWarning("QtCoap: Max retransmit count is capped at 25.");
        maxRetransmit = 25;
    }

    d->maxRetransmit = maxRetransmit;
}

/*!
    Sets the max block size wanted to \a blockSize.

    The \a blockSize should be zero, or range from 16 to 1024 and be a
    power of 2. A size of 0 invites the server to choose the block size.

    \sa blockSize()
*/
void QCoapProtocol::setBlockSize(quint16 blockSize)
{
    Q_D(QCoapProtocol);

    if ((blockSize & (blockSize - 1)) != 0) {
        qWarning("QtCoap: Block size should be a power of 2");
        return;
    }

    if (blockSize != 0 && (blockSize < 16 || blockSize > 1024)) {
        qWarning("QtCoap: Block size should be set to zero,"
                 "or to a power of 2 from 16 through 1024");
        return;
    }

    d->blockSize = blockSize;
}

QT_END_NAMESPACE

#include "moc_qcoapprotocol.cpp"
