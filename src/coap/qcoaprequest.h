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

#ifndef QCOAPREQUEST_H
#define QCOAPREQUEST_H

#include <QtCoap/qcoapglobal.h>
#include <QtCoap/qcoapnamespace.h>
#include <QtCoap/qcoapmessage.h>

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>

QT_BEGIN_NAMESPACE

class QCoapInternalRequest;
class QCoapRequestPrivate;
class Q_COAP_EXPORT QCoapRequest : public QCoapMessage
{
public:
    explicit QCoapRequest(const QUrl &url = QUrl(),
                          Type type = Type::NonConfirmable,
                          const QUrl &proxyUrl = QUrl());
    explicit QCoapRequest(const char* url, Type type = Type::NonConfirmable);
    QCoapRequest(const QCoapRequest &other);
    ~QCoapRequest();

    QCoapRequest &operator=(const QCoapRequest &other);

    QUrl url() const;
    QUrl proxyUrl() const;
    QtCoap::Method method() const;
    bool isObserve() const;
    void setUrl(const QUrl &url);
    void setProxyUrl(const QUrl &proxyUrl);
    void enableObserve();

private:
    // Q_DECLARE_PRIVATE equivalent for shared data pointers
    inline QCoapRequestPrivate* d_func();
    const QCoapRequestPrivate* d_func() const
    { return reinterpret_cast<const QCoapRequestPrivate*>(d_ptr.constData()); }

    friend class QCoapRequestPrivate;
};

QT_END_NAMESPACE

#endif // QCOAPREQUEST_H
