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

#include "qcoapclient_p.h"
#include "qcoapprotocol_p.h"
#include "qcoapreply.h"
#include "qcoapdiscoveryreply.h"
#include "qcoapnamespace.h"
#include "qcoapsecurityconfiguration.h"
#include "qcoapqudpconnection.h"
#include <QtCore/qiodevice.h>
#include <QtCore/qurl.h>
#include <QtCore/qloggingcategory.h>
#include <QtNetwork/qudpsocket.h>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcCoapClient, "qt.coap.client")

QCoapClientPrivate::QCoapClientPrivate(QCoapProtocol *protocol, QCoapConnection *connection)
    : protocol(protocol)
    , connection(connection)
    , workerThread(new QThread)
{
    protocol->moveToThread(workerThread);
    connection->moveToThread(workerThread);
    workerThread->start();
}

QCoapClientPrivate::~QCoapClientPrivate()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
    delete protocol;
    delete connection;
}

/*!
    \class QCoapClient
    \inmodule QtCoap

    \brief The QCoapClient class allows the application to
    send CoAP requests and receive replies.

    \reentrant

    The QCoapClient class contains signals that get triggered when the
    reply of a sent request has arrived.

    The application can use a QCoapClient to send requests over a CoAP
    network. It provides functions for standard requests: each returns a QCoapReply object,
    to which the response data shall be delivered; this can be read when the finished()
    signal arrives.

    A simple request can be sent with:
    \code
        QCoapClient *client = new QCoapClient(this);
        connect(client, &QCoapClient::finished, this, &TestClass::slotFinished);
        client->get(QCoapRequest(Qurl("coap://coap.me/test")));
    \endcode

    \note After processing of the request has finished, it is the responsibility
    of the user to delete the QCoapReply object at an appropriate time. Do not
    directly delete it inside the slot connected to finished(). You can use the
    deleteLater() function.

    You can also use an \e observe request. This can be used as above, or more
    conveniently with the QCoapReply::notified() signal:
    \code
        QCoapRequest request = QCoapRequest(Qurl("coap://coap.me/obs"));
        QCoapReply *reply = client->observe(request);
        connect(reply, &QCoapReply::notified, this, &TestClass::slotNotified);
    \endcode

    And the observation can be cancelled with:
    \code
        client->cancelObserve(reply);
    \endcode

    When a reply arrives, the QCoapClient emits a finished() signal.

    \note For a discovery request, the returned object is a QCoapDiscoveryReply.
    It can be used the same way as a QCoapReply but contains also a list of
    resources.

    \sa QCoapRequest, QCoapReply, QCoapDiscoveryReply
*/

/*!
    \fn void QCoapClient::finished(QCoapReply *reply)

    This signal is emitted along with the \l QCoapReply::finished() signal
    whenever a CoAP reply is received, after either a success or an error.
    The \a reply parameter will contain a pointer to the reply that has just
    been received.

    \sa error(), QCoapReply::finished(), QCoapReply::error()
*/

/*!
    \fn void QCoapClient::responseToMulticastReceived(QCoapReply *reply,
                                                      const QCoapMessage& message,
                                                      const QHostAddress &sender)

    This signal is emitted when a unicast response to a multicast request
    arrives. The \a reply parameter contains a pointer to the reply that has just
    been received, \a message contains the payload and the message details,
    and \a sender contains the sender address.

    \sa error(), QCoapReply::finished(), QCoapReply::error()
*/

/*!
    \fn void QCoapClient::error(QCoapReply *reply, QtCoap::Error error)

    This signal is emitted whenever an error occurs. The \a reply parameter
    can be \nullptr if the error is not related to a specific QCoapReply. The
    \a error parameter contains the error code.

    \sa finished(), QCoapReply::error(), QCoapReply::finished()
*/

/*!
    Constructs a QCoapClient object for the given \a securityMode and
    sets \a parent as the parent object.

    The default for \a securityMode is SecurityMode::NoSec, which
    disables security.

    This connects using a QCoapQUdpConnection; to use a custom transport,
    sub-class QCoapConnection and pass an instance to one of the other
    constructors.
*/
QCoapClient::QCoapClient(QtCoap::SecurityMode securityMode, QObject *parent) :
    QCoapClient(new QCoapProtocol, new QCoapQUdpConnection(securityMode), parent)
{
}

/*!
    Constructs a QCoapClient object with the given \a connection and
    sets \a parent as the parent object.
*/
QCoapClient::QCoapClient(QCoapConnection *connection, QObject *parent) :
    QCoapClient(new QCoapProtocol, connection, parent)
{
}

/*!
    Base constructor, taking the \a protocol, \a connection, and \a parent
    as arguments.
*/
QCoapClient::QCoapClient(QCoapProtocol *protocol, QCoapConnection *connection, QObject *parent) :
    QObject(*new QCoapClientPrivate(protocol, connection), parent)
{
    Q_D(QCoapClient);

    qRegisterMetaType<QCoapReply *>();
    qRegisterMetaType<QCoapMessage>();
    qRegisterMetaType<QPointer<QCoapReply>>();
    qRegisterMetaType<QPointer<QCoapDiscoveryReply>>();
    qRegisterMetaType<QCoapConnection *>();
    qRegisterMetaType<QtCoap::Error>();
    qRegisterMetaType<QtCoap::ResponseCode>();
    // Requires a name, as this is a typedef
    qRegisterMetaType<QCoapToken>("QCoapToken");
    qRegisterMetaType<QCoapMessageId>("QCoapMessageId");
    qRegisterMetaType<QAbstractSocket::SocketOption>();

    connect(d->connection, &QCoapConnection::readyRead, d->protocol,
            [this](const QByteArray &data, const QHostAddress &sender) {
                    Q_D(QCoapClient);
                    d->protocol->d_func()->onFrameReceived(data, sender);
            });
    connect(d->connection, &QCoapConnection::error, d->protocol,
            [this](QAbstractSocket::SocketError socketError) {
                    Q_D(QCoapClient);
                    d->protocol->d_func()->onConnectionError(socketError);
            });

    connect(d->protocol, &QCoapProtocol::finished,
            this, &QCoapClient::finished);
    connect(d->protocol, &QCoapProtocol::responseToMulticastReceived,
            this, &QCoapClient::responseToMulticastReceived);
    connect(d->protocol, &QCoapProtocol::error,
            this, &QCoapClient::error);
}

/*!
    Destroys the QCoapClient object and frees up any
    resources. Note that QCoapReply objects that are returned from
    this class have the QCoapClient set as their parents, which means that
    they will be deleted along with it.
*/
QCoapClient::~QCoapClient()
{
    qDeleteAll(findChildren<QCoapReply *>(QString(), Qt::FindDirectChildrenOnly));
}

/*!
    Sends the \a request using the GET method and returns a new QCoapReply object.

    \sa post(), put(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::get(const QCoapRequest &request)
{
    Q_D(QCoapClient);

    QCoapRequest copyRequest(request, QtCoap::Get);
    copyRequest.adjustUrl(d->connection->isSecure());

    return d->sendRequest(copyRequest);
}

/*!
    \overload

    Sends a GET request to \a url and returns a new QCoapReply object.

    \sa post(), put(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::get(const QUrl &url)
{
    QCoapRequest request(url);
    return get(request);
}

/*!
    Sends the \a request using the PUT method and returns a new QCoapReply
    object. Uses \a data as the payload for this request.

    \sa get(), post(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::put(const QCoapRequest &request, const QByteArray &data)
{
    Q_D(QCoapClient);

    QCoapRequest copyRequest(request, QtCoap::Put);
    copyRequest.setPayload(data);
    copyRequest.adjustUrl(d->connection->isSecure());

    return d->sendRequest(copyRequest);
}

/*!
    \overload

    Sends the \a request using the PUT method and returns a new QCoapReply
    object. Uses \a device content as the payload for this request.
    A null device is treated as empty content.

    \note The device has to be open and readable before calling this function.

    \sa get(), post(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::put(const QCoapRequest &request, QIODevice *device)
{
    return put(request, device ? device->readAll() : QByteArray());
}

/*!
    \overload

    Sends a PUT request to \a url and returns a new QCoapReply object.
    Uses \a data as the payload for this request.

    \sa get(), post(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::put(const QUrl &url, const QByteArray &data)
{
    return put(QCoapRequest(url), data);
}

/*!
    Sends the \a request using the POST method and returns a new QCoapReply
    object. Uses \a data as the payload for this request.

    \sa get(), put(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::post(const QCoapRequest &request, const QByteArray &data)
{
    Q_D(QCoapClient);

    QCoapRequest copyRequest(request, QtCoap::Post);
    copyRequest.setPayload(data);
    copyRequest.adjustUrl(d->connection->isSecure());

    return d->sendRequest(copyRequest);
}

/*!
    \overload

    Sends the \a request using the POST method and returns a new QCoapReply
    object. Uses \a device content as the payload for this request.
    A null device is treated as empty content.

    \note The device has to be open and readable before calling this function.

    \sa get(), put(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::post(const QCoapRequest &request, QIODevice *device)
{
    if (!device)
        return nullptr;

    return post(request, device->readAll());
}

/*!
    \overload

    Sends a POST request to \a url and returns a new QCoapReply object.
    Uses \a data as the payload for this request.

    \sa get(), put(), deleteResource(), observe(), discover()
*/
QCoapReply *QCoapClient::post(const QUrl &url, const QByteArray &data)
{
    return post(QCoapRequest(url), data);
}

/*!
    Sends the \a request using the DELETE method and returns a new QCoapReply
    object.

    \sa get(), put(), post(), observe(), discover()
 */
QCoapReply *QCoapClient::deleteResource(const QCoapRequest &request)
{
    Q_D(QCoapClient);

    QCoapRequest copyRequest(request, QtCoap::Delete);
    copyRequest.adjustUrl(d->connection->isSecure());

    return d->sendRequest(copyRequest);
}

/*!
    \overload

    Sends a DELETE request to the target \a url.

    \sa get(), put(), post(), observe(), discover()
 */
QCoapReply *QCoapClient::deleteResource(const QUrl &url)
{
    return deleteResource(QCoapRequest(url));
}

/*!
    \overload

    Discovers the resources available at the endpoints which have joined
    the \a group. Returns a new QCoapDiscoveryReply object which emits the
    \l QCoapDiscoveryReply::discovered() signal whenever a response arrives.
    The \a group is one of the CoAP multicast group addresses and defaults to
    QtCoap::AllCoapNodesIPv4.

    Discovery path defaults to "/.well-known/core", but can be changed
    by passing a different path to \a discoveryPath. Discovery is described in
    \l{https://tools.ietf.org/html/rfc6690#section-1.2.1}{RFC 6690}.

    \sa get(), post(), put(), deleteResource(), observe()
*/
QCoapDiscoveryReply *QCoapClient::discover(QtCoap::MulticastGroup group,
                                           const QString &discoveryPath)
{
    Q_D(QCoapClient);

    QString base;
    switch (group) {
    case QtCoap::AllCoapNodesIPv4:
        base = QStringLiteral("224.0.1.187");
        break;
    case QtCoap::AllCoapNodesIPv6LinkLocal:
        base = QStringLiteral("ff02::fd");
        break;
    case QtCoap::AllCoapNodesIPv6SiteLocal:
        base = QStringLiteral("ff05::fd");
        break;
    }

    QUrl discoveryUrl;
    discoveryUrl.setHost(base);
    discoveryUrl.setPath(discoveryPath);

    QCoapRequest request(discoveryUrl);
    request.setMethod(QtCoap::Get);
    request.adjustUrl(d->connection->isSecure());

    return d->sendDiscovery(request);
}

/*!
    Discovers the resources available at the given \a url and returns
    a new QCoapDiscoveryReply object which emits the
    \l QCoapDiscoveryReply::discovered() signal whenever the response
    arrives.

    Discovery path defaults to "/.well-known/core", but can be changed
    by passing a different path to \a discoveryPath. Discovery is described in
    \l{https://tools.ietf.org/html/rfc6690#section-1.2.1}{RFC 6690}.

    \sa get(), post(), put(), deleteResource(), observe()
*/
QCoapDiscoveryReply *QCoapClient::discover(const QUrl &url, const QString &discoveryPath)
{
    Q_D(QCoapClient);

    QUrl discoveryUrl(url);
    discoveryUrl.setPath(url.path() + discoveryPath);

    QCoapRequest request(discoveryUrl);
    request.setMethod(QtCoap::Get);
    request.adjustUrl(d->connection->isSecure());

    return d->sendDiscovery(request);
}

/*!
    Sends a request to observe the target \a request and returns
    a new QCoapReply object which emits the \l QCoapReply::notified()
    signal whenever a new notification arrives.

    \sa cancelObserve(), get(), post(), put(), deleteResource(), discover()
*/
QCoapReply *QCoapClient::observe(const QCoapRequest &request)
{
    QCoapRequest copyRequest(request, QtCoap::Get);
    copyRequest.enableObserve();

    return get(copyRequest);
}

/*!
    \overload

    Sends a request to observe the target \a url and returns
    a new QCoapReply object which emits the \l QCoapReply::notified()
    signal whenever a new notification arrives.

    \sa cancelObserve(), get(), post(), put(), deleteResource(), discover()
*/
QCoapReply *QCoapClient::observe(const QUrl &url)
{
    return observe(QCoapRequest(url));
}

/*!
    \overload

    Cancels the observation of a resource using the reply returned by the
    observe() method.

    \sa observe()
*/
void QCoapClient::cancelObserve(QCoapReply *notifiedReply)
{
    Q_D(QCoapClient);
    QMetaObject::invokeMethod(d->protocol, "cancelObserve",
                              Q_ARG(QPointer<QCoapReply>, QPointer<QCoapReply>(notifiedReply)));
}

/*!
    \overload

    Cancels the observation of a resource identified by the \a url.

    \sa observe()
*/
void QCoapClient::cancelObserve(const QUrl &url)
{
    Q_D(QCoapClient);
    const auto adjustedUrl = QCoapRequest::adjustedUrl(url, d->connection->isSecure());
    QMetaObject::invokeMethod(d->protocol, "cancelObserve", Q_ARG(QUrl, adjustedUrl));
}

/*!
    \internal

    Sends the CoAP \a request to its own URL and returns a new QCoapReply
    object.
*/
QCoapReply *QCoapClientPrivate::sendRequest(const QCoapRequest &request)
{
    Q_Q(QCoapClient);

    // Prepare the reply
    QCoapReply *reply = new QCoapReply(request, q);

    if (!send(reply)) {
        delete reply;
        return nullptr;
    }

    return reply;
}

/*!
    \internal

    Sends the CoAP \a request to its own URL and returns a
    new QCoapDiscoveryReply object.
*/
QCoapDiscoveryReply *QCoapClientPrivate::sendDiscovery(const QCoapRequest &request)
{
    Q_Q(QCoapClient);

    // Prepare the reply
    QCoapDiscoveryReply *reply = new QCoapDiscoveryReply(request, q);

    if (!send(reply)) {
        delete reply;
        return nullptr;
    }

    return reply;
}

/*!
    \internal

    Connect to the reply and use the protocol to send it.
*/
bool QCoapClientPrivate::send(QCoapReply *reply)
{
    const auto scheme = connection->isSecure() ? QLatin1String("coaps") : QLatin1String("coap");
    if (reply->request().url().scheme() != scheme) {
        qCWarning(lcCoapClient, "Failed to send request, URL has an incorrect scheme.");
        return false;
    }

    if (!QCoapRequest::isUrlValid(reply->request().url())) {
        qCWarning(lcCoapClient, "Failed to send request for an invalid URL.");
        return false;
    }

    // According to https://tools.ietf.org/html/rfc7252#section-8.1,
    // multicast requests MUST be Non-confirmable.
    if (QHostAddress(reply->url().host()).isMulticast()
            && reply->request().type() == QCoapMessage::Confirmable) {
        qCWarning(lcCoapClient, "Failed to send request, "
                                "multicast requests must be non-confirmable.");
        return false;
    }

    QMetaObject::invokeMethod(protocol, "sendRequest", Qt::QueuedConnection,
                              Q_ARG(QPointer<QCoapReply>, QPointer<QCoapReply>(reply)),
                              Q_ARG(QCoapConnection *, connection));

    return true;
}

/*!
    Sets the security configuration parameters from \a configuration.
    Configuration will be ignored if the QtCoap::NoSec mode is used.

    \note This method must be called before the handshake starts.
*/
void QCoapClient::setSecurityConfiguration(const QCoapSecurityConfiguration &configuration)
{
    Q_D(QCoapClient);

    QMetaObject::invokeMethod(d->connection, "setSecurityConfiguration", Qt::QueuedConnection,
                              Q_ARG(QCoapSecurityConfiguration, configuration));
}

/*!
    Sets the maximum block size used by the protocol to \a blockSize
    when sending requests and receiving replies. The block size must be
    a power of two.

    \sa QCoapProtocol::setBlockSize()
*/
void QCoapClient::setBlockSize(quint16 blockSize)
{
    Q_D(QCoapClient);

    QMetaObject::invokeMethod(d->protocol, "setBlockSize", Qt::QueuedConnection,
                              Q_ARG(quint16, blockSize));
}

/*!
    Sets the QUdpSocket socket \a option to \a value.
*/
void QCoapClient::setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value)
{
    Q_D(QCoapClient);

    QMetaObject::invokeMethod(d->connection, "setSocketOption", Qt::QueuedConnection,
                              Q_ARG(QAbstractSocket::SocketOption, option),
                              Q_ARG(QVariant, value));
}

/*!
    Sets the \c MAX_SERVER_RESPONSE_DELAY value to \a responseDelay in milliseconds.
    The default is 250 seconds.

    As defined in \l {RFC 7390 - Section 2.5}, \c MAX_SERVER_RESPONSE_DELAY is the expected
    maximum response delay over all servers that the client can send a multicast request to.
*/
void QCoapClient::setMaxServerResponseDelay(uint responseDelay)
{
    Q_D(QCoapClient);
    QMetaObject::invokeMethod(d->protocol, "setMaxServerResponseDelay", Qt::QueuedConnection,
                              Q_ARG(uint, responseDelay));
}

#if 0
//! Disabled until fully supported
/*!
    Sets the protocol used by the client. Allows use of a custom protocol.
*/
void QCoapClient::setProtocol(QCoapProtocol *protocol)
{
    Q_D(QCoapClient);
    // FIXME: Protocol running on incorrect thread
    d->protocol = protocol;
}
#endif
QT_END_NAMESPACE
