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

#include "qcoapresource_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCoapResource
    \brief The QCoapResource class holds information about a discovered
    resource.

    \reentrant

    The QCoapRequest contains data as the path and title of the resource
    and other ancillary information.

    \sa QCoapDiscoveryReply
*/

/*!
    Constructs a new QCoapResource.
 */
QCoapResource::QCoapResource() :
    d(new QCoapResourcePrivate)
{
}

/*!
    Copy constructs a new QCoapResource.
 */
QCoapResource::QCoapResource(const QCoapResource &other) :
    d(other.d)
{
}

/*!
    Destroy the QCoapResource.
 */
QCoapResource::~QCoapResource()
{
}

/*!
    Assignment operator.
 */
QCoapResource &QCoapResource::operator =(const QCoapResource &other)
{
    d = other.d;
    return *this;
}

/*!
    Swap function for Q_DECLARE_SHARED
 */
void QCoapResource::swap(QCoapResource &other) Q_DECL_NOTHROW
{
    d.swap(other.d);
}

/*!
    Returns the host of the resource.

    \sa setHost()
 */
QHostAddress QCoapResource::host() const
{
    return d->host;
}

/*!
    Returns the path of the resource.

    \sa setPath()
 */
QString QCoapResource::path() const
{
    return d->path;
}

/*!
    Returns the title of the resource.

    \sa setTitle()
 */
QString QCoapResource::title() const
{
    return d->title;
}

/*!
    Returns true if the resource is observable

    \sa setObservable()
 */
bool QCoapResource::observable() const
{
    return d->observable;
}

/*!
    Returns the type of the resource.

    \sa setResourceType()
 */
QString QCoapResource::resourceType() const
{
    return d->resourceType;
}

/*!
    Returns the interface description of the resource.

    The Interface Description 'if' attribute is an opaque string used to
    provide a name or URI indicating a specific interface definition used
    to interact with the target resource. It is specified in
    \l{https://tools.ietf.org/html/rfc6690#section-3.2}{RFC 6690}.

    \sa setInterface()
 */
QString QCoapResource::interface() const
{
    return d->interface;
}

/*!
    Returns the maximum size of the resource.

    The maximum size estimate attribute 'sz' gives an indication of the
    maximum size of the resource representation returned by performing a
    GET on the target URI. It is specified in
    \l{https://tools.ietf.org/html/rfc6690#section-3.3}{RFC 6690}.

    \sa setMaximumSize()
 */
int QCoapResource::maximumSize() const
{
    return d->maximumSize;
}

/*!
    Returns the Content-Format code of the resource.

    The Content-Format code corresponds to the 'ct' attribute and provides a
    hint about the Content-Formats this resource returns. It is specified
    in \l{https://tools.ietf.org/html/rfc7252#section-7.2.1}{RFC 7252}.

    \sa setContentFormat()
 */
uint QCoapResource::contentFormat() const
{
    return d->contentFormat;
}

/*!
    Sets the host of the resource.

    \sa host()
 */
void QCoapResource::setHost(const QHostAddress &host)
{
    d->host = host;
}

/*!
    Sets the path of the resource.

    \sa path()
 */
void QCoapResource::setPath(const QString &path)
{
    d->path = path;
}

/*!
    Sets the title of the resource.

    \sa title()
 */
void QCoapResource::setTitle(const QString &title)
{
    d->title = title;
}

/*!
    Set it to true to indicate that the resource
    is observable.

    \sa observable()
 */
void QCoapResource::setObservable(bool observable)
{
    d->observable = observable;
}

/*!
    Sets the resource type.

    \sa resourceType()
 */
void QCoapResource::setResourceType(const QString &resourceType)
{
    d->resourceType = resourceType;
}

/*!
    Sets the interface of the resource.

    \sa interface()
 */
void QCoapResource::setInterface(const QString &interface)
{
    d->interface = interface;
}

/*!
    Sets the maximum size of the resource.

    \sa maximumSize()
 */
void QCoapResource::setMaximumSize(int maximumSize)
{
    d->maximumSize = maximumSize;
}

/*!
    Sets the content format of the resource.

    \sa contentFormat()
 */
void QCoapResource::setContentFormat(uint contentFormat)
{
    d->contentFormat = contentFormat;
}

QT_END_NAMESPACE
