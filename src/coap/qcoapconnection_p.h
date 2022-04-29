/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCoap module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/

#ifndef QCOAPCONNECTION_P_H
#define QCOAPCONNECTION_P_H

#include <QtCoap/qcoapnamespace.h>
#include <QtCoap/qcoapsecurityconfiguration.h>

#include <QtCore/qqueue.h>
#include <QtCore/qobject.h>
#include <QtNetwork/qabstractsocket.h>
#include <private/qobject_p.h>

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

class QCoapConnectionPrivate;
class Q_AUTOTEST_EXPORT QCoapConnection : public QObject
{
    Q_OBJECT
public:
    enum class ConnectionState : quint8 {
        Unconnected,
        Bound
    };

    explicit QCoapConnection(QtCoap::SecurityMode securityMode = QtCoap::SecurityMode::NoSecurity,
                             QObject *parent = nullptr);
    virtual ~QCoapConnection();

    bool isSecure() const;
    QtCoap::SecurityMode securityMode() const;
    ConnectionState state() const;
    QCoapSecurityConfiguration securityConfiguration() const;

    Q_INVOKABLE void setSecurityConfiguration(const QCoapSecurityConfiguration &configuration);
    Q_INVOKABLE void disconnect();

Q_SIGNALS:
    void error(QAbstractSocket::SocketError error);
    void readyRead(const QByteArray &data, const QHostAddress &sender);
    void bound();
    void securityConfigurationChanged();

private:
    void startToSendRequest();

protected:
    QCoapConnection(QObjectPrivate &dd, QObject *parent = nullptr);

    virtual void bind(const QString &host, quint16 port) = 0;
    virtual void writeData(const QByteArray &data, const QString &host, quint16 port) = 0;
    virtual void close() = 0;

private:
    friend class QCoapProtocolPrivate;

    Q_DECLARE_PRIVATE(QCoapConnection)
};

struct CoapFrame {
    QByteArray currentPdu;
    QString host;
    quint16 port = 0;

    CoapFrame(const QByteArray &pdu, const QString &hostName, quint16 portNumber)
    : currentPdu(pdu), host(hostName), port(portNumber) {}
};

class Q_AUTOTEST_EXPORT QCoapConnectionPrivate : public QObjectPrivate
{
public:
    QCoapConnectionPrivate(QtCoap::SecurityMode security = QtCoap::SecurityMode::NoSecurity);

    ~QCoapConnectionPrivate() override = default;

    void sendRequest(const QByteArray &request, const QString &host, quint16 port);

    QCoapSecurityConfiguration securityConfiguration;
    QtCoap::SecurityMode securityMode;
    QCoapConnection::ConnectionState state;
    QQueue<CoapFrame> framesToSend;

    Q_DECLARE_PUBLIC(QCoapConnection)
};

QT_END_NAMESPACE

#endif // QCOAPCONNECTION_P_H
