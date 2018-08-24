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

#include "qcoapdiscoveryreply_p.h"
#include "qcoapinternalreply_p.h"

QT_BEGIN_NAMESPACE

QCoapDiscoveryReplyPrivate::QCoapDiscoveryReplyPrivate(const QCoapRequest &request) :
    QCoapReplyPrivate(request)
{
}

/*!
    \internal

    Updates the QCoapDiscoveryReply object, its message and list of resources
    with data of the internal reply \a internalReply.
*/
void QCoapDiscoveryReplyPrivate::_q_setContent(const QHostAddress &sender, const QCoapMessage &msg,
                                               QtCoap::ResponseCode code)
{
    Q_Q(QCoapDiscoveryReply);

    if (q->isFinished())
        return;

    message = msg;
    responseCode = code;

    if (QtCoap::isError(responseCode)) {
        _q_setError(responseCode);
    } else {
        auto res = QCoapProtocol::resourcesFromCoreLinkList(sender, message.payload());
        resources.append(res);
        emit q->discovered(q, res);
    }
}

/*!
    \class QCoapDiscoveryReply
    \brief A QCoapDiscoveryReply object is a QCoapReply that stores also a
    list of QCoapResources.

    \reentrant

    This class is used for discovery requests, and emits the discovered()
    signal if and when resources are discovered. When using a multicast
    address for discovery, the discovered() signal will be emitted once
    for each response received.

    \sa QCoapClient, QCoapRequest, QCoapReply, QCoapResource
*/

/*!
    Constructs a new QCoapDiscoveryReply and sets \a parent as parent object.
*/
QCoapDiscoveryReply::QCoapDiscoveryReply(const QCoapRequest &request, QObject *parent) :
    QCoapReply(*new QCoapDiscoveryReplyPrivate(request), parent)
{
}

/*!
    Returns the list of resources.
*/
QVector<QCoapResource> QCoapDiscoveryReply::resources() const
{
    Q_D(const QCoapDiscoveryReply);
    return d->resources;
}

QT_END_NAMESPACE
