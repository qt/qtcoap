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

#ifndef QCOAPPROTOCOL_H
#define QCOAPPROTOCOL_H

#include <QtCoap/qcoapglobal.h>
#include <QtCoap/qcoapreply.h>
#include <QtCoap/qcoapresource.h>
#include <QtCore/qobject.h>
#include <QtNetwork/qudpsocket.h>
#include <QtNetwork/qhostaddress.h>

QT_BEGIN_NAMESPACE

class QCoapInternalRequest;
class QCoapInternalReply;
class QCoapProtocolPrivate;
class QCoapConnection;
class Q_COAP_EXPORT QCoapProtocol : public QObject
{
    Q_OBJECT
public:
    explicit QCoapProtocol(QObject *parent = nullptr);
    ~QCoapProtocol();

    int ackTimeout() const;
    double ackRandomFactor() const;
    int maxRetransmit() const;
    quint16 blockSize() const;
    int maxTransmitSpan() const;
    int maxTransmitWait() const;
    static constexpr int maxLatency();

    int minTimeout() const;
    int maxTimeout() const;

    static QVector<QCoapResource> resourcesFromCoreLinkList(const QHostAddress &sender, const QByteArray &data);

Q_SIGNALS:
    void finished(QCoapReply *reply);
    void error(QCoapReply *reply, QtCoap::Error error);

public Q_SLOTS:
    void cancelObserve(QPointer<QCoapReply> reply);
    void setAckTimeout(int ackTimeout);
    void setAckRandomFactor(double ackRandomFactor);
    void setMaxRetransmit(int maxRetransmit);
    void setBlockSize(quint16 blockSize);

private Q_SLOTS:
    void sendRequest(QPointer<QCoapReply> reply, QCoapConnection *connection);

private:
    Q_DECLARE_PRIVATE(QCoapProtocol)
    Q_PRIVATE_SLOT(d_func(), void onRequestTimeout(QCoapInternalRequest*))
    Q_PRIVATE_SLOT(d_func(), void onRequestMaxTransmissionSpanReached(QCoapInternalRequest*))
    Q_PRIVATE_SLOT(d_func(), void sendRequest(QCoapInternalRequest*))
    Q_PRIVATE_SLOT(d_func(), void onFrameReceived(const QByteArray&, const QHostAddress&))
    Q_PRIVATE_SLOT(d_func(), void onRequestAborted(const QCoapToken&))
    Q_PRIVATE_SLOT(d_func(), void onConnectionError(QAbstractSocket::SocketError))
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QHostAddress)

#endif // QCOAPPROTOCOL_H
