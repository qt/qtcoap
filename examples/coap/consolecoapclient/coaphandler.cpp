// Copyright (C) 2017 Witekio.
// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "coaphandler.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QCoapClient>
#include <QCoapReply>
#include <QCoapResourceDiscoveryReply>

Q_LOGGING_CATEGORY(lcCoapClient, "qt.coap.client")

CoapHandler::CoapHandler(QObject *parent) : QObject(parent)
{
    connect(&m_coapClient, &QCoapClient::finished, this, &CoapHandler::onFinished);
    connect(&m_coapClient, &QCoapClient::error, this, &CoapHandler::onError);
    connect(&m_coapClient, &QCoapClient::responseToMulticastReceived,
            this, &CoapHandler::onResponseToMulticast);
}

bool CoapHandler::runGet(const QUrl &url)
{
    return m_coapClient.get(url);
}

bool CoapHandler::runPost(const QUrl &url)
{
    return m_coapClient.post(url);
}

bool CoapHandler::runPut(const QUrl &url)
{
    return m_coapClient.put(url);
}

bool CoapHandler::runDelete(const QUrl &url)
{
    return m_coapClient.deleteResource(url);
}

bool CoapHandler::runObserve(const QUrl &url)
{
    QCoapReply *observeReply = m_coapClient.observe(url);
    if (!observeReply)
        return false;

    connect(observeReply, &QCoapReply::notified, this, &CoapHandler::onNotified);
    return true;
}

bool CoapHandler::runDiscover(const QUrl &url)
{
    QCoapResourceDiscoveryReply *discoverReply = m_coapClient.discover(url);
    if (!discoverReply)
        return false;

    connect(discoverReply, &QCoapResourceDiscoveryReply::discovered, this, &CoapHandler::onDiscovered);
    return true;
}

void CoapHandler::onFinished(QCoapReply *reply)
{
    if (reply->errorReceived() == QtCoap::Error::Ok)
        qCInfo(lcCoapClient) << "Request finished with payload:" << reply->readAll();
    else
        qCWarning(lcCoapClient, "Request failed");

    // Don't forget to remove the reply
    reply->deleteLater();
}

void CoapHandler::onNotified(QCoapReply *reply, QCoapMessage message)
{
    Q_UNUSED(message)

    // You can alternatively use `message.payload();`
    qCInfo(lcCoapClient) << "Received Observe notification with payload:" << reply->readAll();
}

void CoapHandler::onDiscovered(QCoapResourceDiscoveryReply *reply, QList<QCoapResource> resources)
{
    Q_UNUSED(reply)

    for (const QCoapResource &res : qAsConst(resources))
        qCInfo(lcCoapClient) << "Discovered resource:" << res.path() << res.title();
}

void CoapHandler::onResponseToMulticast(QCoapReply *reply, const QCoapMessage& message,
                                        const QHostAddress &sender)
{
    if (reply->errorReceived() == QtCoap::Error::Ok)
        qCInfo(lcCoapClient) << "Got a response for multicast request from:" << sender.toString()
                             << "with payload:" << message.payload();
    else
        qCWarning(lcCoapClient, "Multicast request failed");
}

void CoapHandler::onError(QCoapReply *reply, QtCoap::Error error)
{
    if (reply)
        qCInfo(lcCoapClient) << "CoAP reply error:" << reply->errorString();
    else
        qCWarning(lcCoapClient) << "CoAP error:" << error;
}
