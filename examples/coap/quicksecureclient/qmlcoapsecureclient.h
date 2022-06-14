// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef QMLCOAPSECURECLIENT_H
#define QMLCOAPSECURECLIENT_H

#include <QCoapClient>
#include <QCoapSecurityConfiguration>

class QmlCoapSecureClient : public QObject
{
    Q_OBJECT

public:
    QmlCoapSecureClient(QObject *parent = nullptr);
    ~QmlCoapSecureClient() override;

    Q_INVOKABLE void setSecurityMode(QtCoap::SecurityMode mode);
    Q_INVOKABLE void sendGetRequest(const QString &host, const QString &path, int port);
    Q_INVOKABLE void setSecurityConfiguration(const QString &preSharedKey, const QString &identity);
    Q_INVOKABLE void setSecurityConfiguration(const QString &localCertificatePath,
                                              const QString &caCertificatePath,
                                              const QString &privateKeyPath);
    Q_INVOKABLE void disconnect();

Q_SIGNALS:
    void finished(const QString &result);

private:
    QCoapClient *m_coapClient;
    QCoapSecurityConfiguration m_configuration;
    QtCoap::SecurityMode m_securityMode;
};

#endif // QMLCOAPSECURECLIENT_H
