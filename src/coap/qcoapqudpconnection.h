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

#ifndef QCOAPQUDPCONNECTION_H
#define QCOAPQUDPCONNECTION_H

#include <QtCoap/qcoapconnection.h>
#include <QtCoap/qcoapnamespace.h>
#include <QtCoap/qcoapglobal.h>

#include <QtCore/qglobal.h>
#include <QtCore/qstring.h>
#include <QtNetwork/qudpsocket.h>

QT_BEGIN_NAMESPACE

class QCoapQUdpConnectionPrivate;
class QSslPreSharedKeyAuthenticator;
class Q_COAP_EXPORT QCoapQUdpConnection : public QCoapConnection
{
    Q_OBJECT

public:
    explicit QCoapQUdpConnection(QtCoap::SecurityMode security = QtCoap::NoSec,
                                 QObject *parent = nullptr);

    ~QCoapQUdpConnection() override = default;

    QUdpSocket *socket() const;

public Q_SLOTS:
    void setSocketOption(QAbstractSocket::SocketOption, const QVariant &value);

#if QT_CONFIG(dtls)
private Q_SLOTS:
    void pskRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void handshakeTimeout();
#endif

protected:
    explicit QCoapQUdpConnection(QCoapQUdpConnectionPrivate &dd, QObject *parent = nullptr);

    void bind(const QString &host, quint16 port) override;
    void writeData(const QByteArray &data, const QString &host, quint16 port) override;

    void createSocket();

    Q_DECLARE_PRIVATE(QCoapQUdpConnection)
};

QT_END_NAMESPACE

#endif // QCOAPQUDPCONNECTION_H
