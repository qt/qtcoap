// Copyright (C) 2017 Witekio.
// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef COAPHANDLER_H
#define COAPHANDLER_H

#include <QObject>
#include <QCoapClient>
#include <QCoapMessage>
#include <QCoapResource>
#include <qcoapnamespace.h>

QT_BEGIN_NAMESPACE

class QCoapReply;
class QCoapResourceDiscoveryReply;
class QCoapResource;

QT_END_NAMESPACE

class CoapHandler : public QObject
{
    Q_OBJECT
public:
    explicit CoapHandler(QObject *parent = nullptr);

    bool runGet(const QUrl &url);
    bool runPost(const QUrl &url);
    bool runPut(const QUrl &url);
    bool runDelete(const QUrl &url);
    bool runObserve(const QUrl &url);
    bool runDiscover(const QUrl &url);

public Q_SLOTS:
    void onFinished(QCoapReply *reply);
    void onNotified(QCoapReply *reply, QCoapMessage message);
    void onDiscovered(QCoapResourceDiscoveryReply *reply, QList<QCoapResource> resources);
    void onResponseToMulticast(QCoapReply *reply, const QCoapMessage& message,
                               const QHostAddress &sender);
    void onError(QCoapReply *reply, QtCoap::Error error);

private:
    QCoapClient m_coapClient;
};

#endif // COAPHANDLER_H
