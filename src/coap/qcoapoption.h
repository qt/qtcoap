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

#ifndef QCOAPOPTION_H
#define QCOAPOPTION_H

#include <QtCore/qglobal.h>
#include <QtCoap/qcoapglobal.h>
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QCoapOptionPrivate;
class Q_COAP_EXPORT QCoapOption
{
public:
    enum OptionName {
        Invalid         = 0,
        IfMatch         = 1,
        UriHost         = 3,
        Etag            = 4,
        IfNoneMatch     = 5,
        Observe         = 6,
        UriPort         = 7,
        LocationPath    = 8,
        UriPath         = 11,
        ContentFormat   = 12,
        MaxAge          = 14,
        UriQuery        = 15,
        Accept          = 17,
        LocationQuery   = 20,
        Block2          = 23,
        Block1          = 27,
        Size2           = 28,
        ProxyUri        = 35,
        ProxyScheme     = 39,
        Size1           = 60
    };

    QCoapOption(OptionName name = Invalid, const QByteArray &opaqueValue = QByteArray());
    QCoapOption(OptionName name, const QString &stringValue);
    QCoapOption(OptionName name, quint32 intValue);
    QCoapOption(const QCoapOption &other);
    QCoapOption(QCoapOption &&other);
    ~QCoapOption();

    QCoapOption &operator=(const QCoapOption &other);
    QCoapOption &operator=(QCoapOption &&other) noexcept;
    void swap(QCoapOption &other) noexcept;

    QByteArray opaqueValue() const;
    quint32 uintValue() const;
    QString stringValue() const;
    int length() const;
    OptionName name() const;
    bool isValid() const;

    bool operator==(const QCoapOption &other) const;
    bool operator!=(const QCoapOption &other) const;

private:
    QCoapOptionPrivate *d_ptr;

    // Q_DECLARE_PRIVATE equivalent for shared data pointers
    inline QCoapOptionPrivate *d_func();
    const QCoapOptionPrivate *d_func() const { return d_ptr; }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QCoapOption::OptionName)
Q_DECLARE_METATYPE(QCoapOption)

#endif // QCOAPOPTION_H
