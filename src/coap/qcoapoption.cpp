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

#include <QtCore/qdebug.h>
#include "qcoapoption_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCoapOption
    \brief The QCoapOption class holds data about CoAP options.

    \reentrant

    An option contains a name, related to an option id, and a value.
    The name is one of the values from the OptionName enumeration.
*/

/*!
    \enum QCoapOption::OptionName

    Indicates the name of an option.
    The value of each ID is as specified by the CoAP standard, with the
    exception of Invalid. You can refer to
    \l{https://tools.ietf.org/html/rfc7252#section-5.10}{RFC 7252} and
    \l{https://tools.ietf.org/html/rfc7959#section-2.1}{RFC 7959} for more details.

    \value IfMatchCoapOption        If-Match
    \value UriHostCoapOption        Uri-Host
    \value EtagCoapOption           Etag
    \value IfNoneMatchCoapOption    If-None-Match
    \value ObserveCoapOption        Observe
    \value UriPortCoapOption        Uri-Port
    \value LocationPathCoapOption   Location-path
    \value UriPathCoapOption        Uri-Path
    \value ContentFormatCoapOption  Content-Format
    \value MaxAgeCoapOption         Max-Age
    \value UriQueryCoapOption       Uri-Query
    \value AcceptCoapOption         Accept
    \value LocationQueryCoapOption  Location-Query
    \value Block2CoapOption         Block2
    \value Block1CoapOption         Block1
    \value Size2CoapOption          Size2
    \value ProxyUriCoapOption       Proxy-Uri
    \value ProxySchemeCoapOption    Proxy-Scheme
    \value Size1CoapOption          Size1
*/

/*!
    Constructs a QCoapOption object with the given \a name
    and QByteArray \a value.
    If no parameters are passed, constructs an Invalid object.

    \sa isValid()
 */
QCoapOption::QCoapOption(OptionName name, const QByteArray &value) :
    d_ptr(new QCoapOptionPrivate)
{
    Q_D(QCoapOption);
    d->name = name;
    setValue(value);
}

/*!
    Constructs a QCoapOption object with the given \a name
    and the QStringView \a value.

    \sa isValid()
 */
QCoapOption::QCoapOption(OptionName name, QStringView value) :
    d_ptr(new QCoapOptionPrivate)
{
    Q_D(QCoapOption);
    d->name = name;
    setValue(value);
}

/*!
    Constructs a QCoapOption object with the given \a name
    and the string \a value.

    \sa isValid()
 */
QCoapOption::QCoapOption(OptionName name, const char *value) :
    d_ptr(new QCoapOptionPrivate)
{
    Q_D(QCoapOption);
    d->name = name;
    setValue(value);
}

/*!
    Constructs a QCoapOption object with the given \a name
    and the unsigned integer \a value.

    \sa isValid()
 */
QCoapOption::QCoapOption(OptionName name, quint32 value) :
    d_ptr(new QCoapOptionPrivate)
{
    Q_D(QCoapOption);
    d->name = name;
    setValue(value);
}

/*!
    Constructs a QCoapOption object by copy from an \a other QCoapOption.

    \sa isValid()
 */
QCoapOption::QCoapOption(const QCoapOption &other) :
    d_ptr(new QCoapOptionPrivate(*other.d_ptr))
{
}

/*!
    QCoapOption move constructor.
 */
QCoapOption::QCoapOption(QCoapOption &&other) :
    d_ptr(other.d_ptr)
{
    other.d_ptr = nullptr;
}

/*!
    Destroys QCoapOption object.
 */
QCoapOption::~QCoapOption()
{
    delete d_ptr;
}

/*!
    Assignment operator.
 */
QCoapOption &QCoapOption::operator=(const QCoapOption &other)
{
    d_ptr->name = other.d_ptr->name;
    d_ptr->value = other.d_ptr->value;
    return *this;
}

/*!
    Move assignment operator.
 */
QCoapOption &QCoapOption::operator=(QCoapOption &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Swap object with another.
 */
void QCoapOption::swap(QCoapOption &other) Q_DECL_NOTHROW
{
    qSwap(d_ptr, other.d_ptr);
}

/*!
    Returns the value of the option.
 */
QByteArray QCoapOption::value() const
{
    Q_D(const QCoapOption);
    return d->value;
}

/*!
    Returns the integer value of the option.
 */
quint32 QCoapOption::valueToInt() const
{
    Q_D(const QCoapOption);

    quint32 intValue = 0;
    for (int i = 0; i < d->value.length(); i++)
        intValue |= static_cast<quint8>(d->value.at(i)) << (8 * i);

    return intValue;
}

/*!
    Returns the length of the value of the option.
 */
int QCoapOption::length() const
{
    Q_D(const QCoapOption);
    return d->value.length();
}

/*!
    Returns the name of the option.
 */
QCoapOption::OptionName QCoapOption::name() const
{
    Q_D(const QCoapOption);
    return d->name;
}

/*!
    Returns \c true if the option is valid.
 */
bool QCoapOption::isValid() const
{
    Q_D(const QCoapOption);
    return d->name != QCoapOption::Invalid;
}

/*!
    Returns \c true if this QCoapOption and \a other are equals.
 */
bool QCoapOption::operator==(const QCoapOption &other) const
{
    Q_D(const QCoapOption);
    return (d->name == other.d_ptr->name
            && d->value == other.d_ptr->value);
}

/*!
    Returns \c true if this QCoapOption and \a other are different.
 */
bool QCoapOption::operator!=(const QCoapOption &other) const
{
    return !(*this == other);
}

/*!
    Sets the \a value for the option.
 */
void QCoapOption::setValue(const QByteArray &value)
{
    Q_D(QCoapOption);
    bool oversized = false;

    // Check for value maximum size, according to section 5.10 of RFC 7252
    // https://tools.ietf.org/html/rfc7252#section-5.10
    switch (d_ptr->name) {
    case IfNoneMatch:
        if (value.size() > 0)
            oversized = true;
        break;

    case UriPort:
    case ContentFormat:
    case Accept:
        if (value.size() > 2)
            oversized = true;
        break;

    case MaxAge:
    case Size1:
        if (value.size() > 4)
            oversized = true;
        break;

    case IfMatch:
    case Etag:
        if (value.size() > 8)
            oversized = true;
        break;

    case UriHost:
    case LocationPath:
    case UriPath:
    case UriQuery:
    case LocationQuery:
    case ProxyScheme:
        if (value.size() > 255)
            oversized = true;
        break;

    case ProxyUri:
        if (value.size() > 1034)
            oversized = true;
        break;

    case Observe:
    case Block2:
    case Block1:
    case Size2:
    default:
        break;
    }

    if (oversized)
        qWarning() << "QCoapOption::setValue: value is probably too big for option" << d->name;

    d->value = value;
}

/*!
    \overload

    Sets the \a value for the option.
 */
void QCoapOption::setValue(QStringView value)
{
    setValue(value.toUtf8());
}

/*!
    \overload

    Sets the \a value for the option.
 */
void QCoapOption::setValue(const char *value)
{
    setValue(QByteArray(value, static_cast<int>(strlen(value))));
}

/*!
    \overload

    Sets the \a value for the option.
 */
void QCoapOption::setValue(quint32 value)
{
    QByteArray data;
    for (; value; value >>= 8)
        data.append(static_cast<qint8>(value & 0xFF));

    setValue(data);
}

/*!
    \internal

    For QSharedDataPointer.
*/
QCoapOptionPrivate *QCoapOption::d_func()
{
    return d_ptr;
}

QT_END_NAMESPACE
