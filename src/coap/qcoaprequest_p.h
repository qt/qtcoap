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

#ifndef QCOAPREQUEST_P_H
#define QCOAPREQUEST_P_H

#include <QtCoap/qcoapnamespace.h>
#include <QtCoap/qcoaprequest.h>
#include <private/qcoapmessage_p.h>

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

class Q_AUTOTEST_EXPORT QCoapRequestPrivate : public QCoapMessagePrivate
{
public:
    QCoapRequestPrivate(const QUrl &url = QUrl(),
                        QCoapMessage::Type type = QCoapMessage::Type::NonConfirmable,
                        const QUrl &proxyUrl = QUrl());
    ~QCoapRequestPrivate();

    QCoapRequestPrivate *clone() const override;

    void setUrl(const QUrl &url);
    void adjustUrl(bool secure);

    static QCoapRequest createRequest(const QCoapRequest &other, QtCoap::Method method,
                                      bool isSecure = false);
    static QUrl adjustedUrl(const QUrl &url, bool secure);
    static bool isUrlValid(const QUrl &url);

    QUrl uri;
    QUrl proxyUri;
    QtCoap::Method method = QtCoap::Method::Invalid;

protected:
    QCoapRequestPrivate(const QCoapRequestPrivate &other) = default;
};

QT_END_NAMESPACE

#endif // QCOAPREQUEST_P_H
