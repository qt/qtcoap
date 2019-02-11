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

#include "coaphandler.h"

#include <QDebug>
#include <QCoapClient>
#include <QCoapReply>
#include <QCoapDiscoveryReply>

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
    QCoapDiscoveryReply *discoverReply = m_coapClient.discover(url);
    if (discoverReply)
        return false;

    connect(discoverReply, &QCoapDiscoveryReply::discovered, this, &CoapHandler::onDiscovered);
    return true;
}

void CoapHandler::onFinished(QCoapReply *reply)
{
    if (reply->errorReceived() == QtCoap::NoError)
        qDebug() << "Request finished with payload: " << reply->readAll();
    else
        qDebug() << "Request failed";

    // Don't forget to remove the reply
    reply->deleteLater();
}

void CoapHandler::onNotified(QCoapReply *reply, QCoapMessage message)
{
    Q_UNUSED(message)

    // You can alternatively use `message.payload();`
    qDebug() << "Received Observe notification with payload: " << reply->readAll();
}

void CoapHandler::onDiscovered(QCoapDiscoveryReply *reply, QVector<QCoapResource> resources)
{
    Q_UNUSED(reply)

    for (const QCoapResource &res : qAsConst(resources))
        qDebug() << "Discovered resource: " << res.path() << res.title();
}

void CoapHandler::onResponseToMulticast(QCoapReply *reply, const QCoapMessage& message)
{
    if (reply->errorReceived() == QtCoap::NoError)
        qDebug() << "Got a response for multicast request: " << message.payload();
    else
        qDebug() << "Multicast request failed";
}

void CoapHandler::onError(QCoapReply *reply, QtCoap::Error error)
{
    if (reply)
        qDebug() << "CoAP reply error: " << reply->errorString();
    else
        qDebug() << "CoAP error: " << error;
}
