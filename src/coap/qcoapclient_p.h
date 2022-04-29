/****************************************************************************
**
** Copyright (C) 2017 Witekio.
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

#ifndef QCOAPCLIENT_P_H
#define QCOAPCLIENT_P_H

#include <QtCoap/qcoapclient.h>
#include <QtCore/qthread.h>
#include <QtCore/qpointer.h>
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

class Q_AUTOTEST_EXPORT QCoapClientPrivate : public QObjectPrivate
{
public:
    QCoapClientPrivate(QCoapProtocol *protocol, QCoapConnection *connection);
    ~QCoapClientPrivate();

    QCoapProtocol *protocol = nullptr;
    QCoapConnection *connection = nullptr;
    QThread *workerThread = nullptr;

    QCoapReply *sendRequest(const QCoapRequest &request);
    QCoapResourceDiscoveryReply *sendDiscovery(const QCoapRequest &request);
    bool send(QCoapReply *reply);

    void setConnection(QCoapConnection *customConnection);

    Q_DECLARE_PUBLIC(QCoapClient)
};

QT_END_NAMESPACE

#endif // QCOAPCLIENT_P_H
