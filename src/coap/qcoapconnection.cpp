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

#include "qcoapconnection_p.h"
#include <QtNetwork/qnetworkdatagram.h>

QT_BEGIN_NAMESPACE

/*!
    \class QCoapConnection
    \brief The QCoapConnection class handles the transfer of a frame to a
    server.

    \reentrant

    The QCoapConnection class is used by the QCoapClient class to send
    requests to a server. It has a socket listening for UDP messages,
    that is used to send the CoAP frames.

    When a reply is available, the QCoapConnection object emits a
    \l{QCoapConnection::readyRead(const QNetworkDatagram&)}
    {readyRead(const QNetworkDatagram&)} signal.

    \sa QCoapClient
*/

/*!
    Constructs a new QCoapConnection and sets \a parent as the parent object.
*/
QCoapConnection::QCoapConnection(QObject *parent) :
    QCoapConnection(*new QCoapConnectionPrivate, parent)
{
}

/*!
    \internal

    Constructs a new QCoapConnection with \a dd as the d_ptr.
    This constructor must be used when internally subclassing
    the QCoapConnection class.
*/
QCoapConnection::QCoapConnection(QCoapConnectionPrivate &dd, QObject *parent) :
    QObject(dd, parent)
{
    createSocket();
    qRegisterMetaType<QNetworkDatagram>();
}

/*!
    \internal

    Creates the socket used by the connection and set it in connection's private
    class
*/
void QCoapConnection::createSocket()
{
    Q_D(QCoapConnection);

    //! TODO Create DTLS socket here when available
    d->setSocket(new QUdpSocket(this));
}

/*!
    \internal

    Binds the socket to a random port and returns \c true if it succeeds.
*/
bool QCoapConnectionPrivate::bind()
{
    return socket()->bind(QHostAddress::Any, 0, QAbstractSocket::ShareAddress);
}

/*!
    \internal

    Binds the socket and call the socketBound() slot.
*/
void QCoapConnectionPrivate::bindSocket()
{
    Q_Q(QCoapConnection);

    if (socket()->state() == QUdpSocket::ConnectedState)
        socket()->disconnectFromHost();

    q->connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
               q, SLOT(_q_socketError(QAbstractSocket::SocketError)));
    q->connect(udpSocket, SIGNAL(readyRead()), q, SLOT(_q_socketReadyRead()));

    if (bind())
        _q_socketBound();
}

/*!
    Binds the socket if it is not already done and sends the given
    \a request frame to the given \a host and \a port.
*/
void QCoapConnection::sendRequest(const QByteArray &request, const QString &host, quint16 port)
{
    Q_D(QCoapConnection);

    CoapFrame frame(request, host, port);
    d->framesToSend.enqueue(frame);

    if (d->state == Bound) {
        QMetaObject::invokeMethod(this, "_q_startToSendRequest", Qt::QueuedConnection);
    } else if (d->state == Unconnected) {
        connect(this, SIGNAL(bound()), this, SLOT(_q_startToSendRequest()), Qt::QueuedConnection);
        d->bindSocket();
    }
}

/*!
    Sets the QUdpSocket socket \a option to \a value.
*/
void QCoapConnection::setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value)
{
    Q_D(QCoapConnection);
    d->socket()->setSocketOption(option, value);
}

/*!
    \internal

    Writes the given \a data frame to the socket to the stored \a host and \a port.
*/
void QCoapConnectionPrivate::writeToSocket(const CoapFrame &frame)
{
    if (!socket()->isWritable()) {
        bool opened = socket()->open(socket()->openMode() | QIODevice::WriteOnly);
        if (!opened) {
            qWarning() << "QtCoap: Failed to open the UDP socket with write permission";
            return;
        }
    }

    QHostAddress host(frame.host);
    if (host.isNull()) {
        qWarning() << "QtCoap: Invalid host IP address" << frame.host
                   << "- only IPv4/IPv6 destination addresses are supported.";
        return;
    }

    qint64 bytesWritten = socket()->writeDatagram(frame.currentPdu,
                                                  QHostAddress(frame.host), frame.port);
    if (bytesWritten < 0)
        qWarning() << "QtCoap: Failed to write datagram:" << socket()->errorString();
}

/*!
    \internal

    This slot changes the connection state to "Bound" and emits the
    \l{QCoapConnection::bound()}{bound()} signal.
*/
void QCoapConnectionPrivate::_q_socketBound()
{
    Q_Q(QCoapConnection);

    if (state == QCoapConnection::Bound)
        return;

    setState(QCoapConnection::Bound);
    emit q->bound();
}

/*!
    \internal

    This slot writes the current stored frame to the socket.
*/
void QCoapConnectionPrivate::_q_startToSendRequest()
{
    const CoapFrame frame = framesToSend.dequeue();
    writeToSocket(frame);
}

/*!
    \internal

    This slot reads all data stored in the socket and emits
    \l{QCoapConnection::readyRead(const QNetworkDatagram&)}
    {readyRead(const QNetworkDatagram&)} signal for each received datagram.
*/
void QCoapConnectionPrivate::_q_socketReadyRead()
{
    Q_Q(QCoapConnection);

    if (!socket()->isReadable()) {
        bool opened = socket()->open(socket()->openMode() | QIODevice::ReadOnly);
        if (!opened) {
            qWarning() << "QtCoap: Failed to open the UDP socket with read permission";
            return;
        }
    }

    while (socket()->hasPendingDatagrams()) {
        emit q->readyRead(socket()->receiveDatagram());
    }
}

/*!
    \internal

    This slot emits the \l{QCoapConnection::error(QAbstractSocket::SocketError)}
    {error(QAbstractSocket::SocketError)} signal.
*/
void QCoapConnectionPrivate::_q_socketError(QAbstractSocket::SocketError error)
{
    Q_Q(QCoapConnection);

    qWarning() << "CoAP UDP socket error" << error << socket()->errorString();
    emit q->error(error);
}

/*!
    Returns the socket.
*/
QUdpSocket *QCoapConnection::socket() const
{
    Q_D(const QCoapConnection);
    return d->udpSocket;
}

/*!
    Returns the connection state.
*/
QCoapConnection::ConnectionState QCoapConnection::state() const
{
    Q_D(const QCoapConnection);
    return d->state;
}

/*!
    \internal

    Sets the socket.

    \sa socket()
*/
void QCoapConnectionPrivate::setSocket(QUdpSocket *socket)
{
    udpSocket = socket;
}

/*!
    \internal

    Sets the connection state.

    \sa state()
*/
void QCoapConnectionPrivate::setState(QCoapConnection::ConnectionState newState)
{
    state = newState;
}

QT_END_NAMESPACE

#include "moc_qcoapconnection.cpp"
