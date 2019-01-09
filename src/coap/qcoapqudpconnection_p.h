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

#ifndef QCOAPQUDPCONNECTION_P_H
#define QCOAPQUDPCONNECTION_P_H

#include <QtCoap/qcoapqudpconnection.h>
#include <QtCoap/qcoapsecurityconfiguration.h>
#include <private/qcoapconnection_p.h>

#include <QtNetwork/qudpsocket.h>
#include <QtCore/qqueue.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

class QDtls;
class QSslPreSharedKeyAuthenticator;
class Q_AUTOTEST_EXPORT QCoapQUdpConnectionPrivate : public QCoapConnectionPrivate
{
public:
    QCoapQUdpConnectionPrivate(QtCoap::SecurityMode security = QtCoap::NoSec);

    virtual bool bind();

    void bindSocket();
    void writeToSocket(const QByteArray &data, const QString &host, quint16 port);
    QUdpSocket* socket() const { return udpSocket; }

    void setSecurityConfiguration(const QCoapSecurityConfiguration &configuration);

#if QT_CONFIG(dtls)
    QNetworkDatagram receiveDatagramDecrypted() const;
    void handleEncryptedDatagram();
#endif

    void _q_socketReadyRead();
    void _q_socketError(QAbstractSocket::SocketError);

    QPointer<QUdpSocket> udpSocket;
#if QT_CONFIG(dtls)
    QPointer<QDtls> dtls;
#endif

    Q_DECLARE_PUBLIC(QCoapQUdpConnection)
};

QT_END_NAMESPACE

#endif // QCOAPQUDPCONNECTION_P_H
