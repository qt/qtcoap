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

#ifndef QCOAPRESOURCE_H
#define QCOAPRESOURCE_H

#include <QtCoap/qcoapglobal.h>
#include <QtCore/qshareddata.h>
#include <QtNetwork/qhostaddress.h>

#ifdef interface
#  undef interface
#endif

QT_BEGIN_NAMESPACE

class QCoapResourcePrivate;

class Q_COAP_EXPORT QCoapResource
{
public:
    QCoapResource();
    QCoapResource(const QCoapResource &other);
    ~QCoapResource();
    QCoapResource &operator =(const QCoapResource &other);

    void swap(QCoapResource &other) noexcept;

    QHostAddress host() const;
    QString path() const;
    QString title() const;
    bool observable() const;
    QString resourceType() const;
    QString interface() const;
    int maximumSize() const;
    uint contentFormat() const;

    void setHost(const QHostAddress &host);
    void setPath(const QString &path);
    void setTitle(const QString &title);
    void setObservable(bool observable);
    void setResourceType(const QString &resourceType);
    void setInterface(const QString &interface);
    void setMaximumSize(int maximumSize);
    void setContentFormat(uint contentFormat);

private:
    QSharedDataPointer<QCoapResourcePrivate> d;
};

Q_DECLARE_SHARED(QCoapResource)

QT_END_NAMESPACE

#endif // QCOAPRESOURCE_H
