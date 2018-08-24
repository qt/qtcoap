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

#include "qcoaprequest_p.h"
#include <QtCore/qmath.h>
#include <QtCore/qdatetime.h>

QT_BEGIN_NAMESPACE

QCoapRequestPrivate::QCoapRequestPrivate(const QUrl &url, QCoapMessage::MessageType type, const QUrl &proxyUrl) :
    QCoapMessagePrivate(type),
    proxyUri(proxyUrl)
{
    setUrl(url);
}

QCoapRequestPrivate::~QCoapRequestPrivate()
{
}

/*!
    \internal

    \brief Sets the url after adjusting it, and asserting its validity.
*/
void QCoapRequestPrivate::setUrl(const QUrl &url)
{
    // Print no warning when clearing URL
    if (url.isEmpty()) {
        uri = url;
        return;
    }

    // Make first checks before editing the URL, to avoid editing it
    // in a wrong way (e.g. when adding the scheme)
    if (!url.isValid()) {
        qWarning() << "QCoapRequest: Invalid CoAP url" << url.toString();
        return;
    }

    QUrl finalizedUrl = url;
    if (url.isRelative())
        finalizedUrl = url.toString().prepend(QLatin1String("coap://"));
    else if (url.scheme().isEmpty())
        finalizedUrl.setScheme(QLatin1String("coap"));

    if (url.port() == -1)
        finalizedUrl.setPort(QtCoap::DefaultPort);

    if (!QCoapRequest::isUrlValid(finalizedUrl)) {
        qWarning() << "QCoapRequest: Invalid CoAP url" << finalizedUrl.toString();
        return;
    }

    uri = finalizedUrl;
}

/*!
    \class QCoapRequest
    \brief The QCoapRequest class holds a CoAP request. This request
    can be sent with QCoapClient.

    \reentrant

    The QCoapRequest contains data needed to make CoAP frames that can be
    sent to the URL it holds.

    \sa QCoapClient, QCoapReply, QCoapDiscoveryReply
*/

/*!
    Constructs a QCoapRequest object with the target \a url,
    the proxy URL \a proxyUrl and the \a type of the message.

    If not indicated, the scheme of the URL will default to 'coap', and its
    port will default to 5683.
*/
QCoapRequest::QCoapRequest(const QUrl &url, MessageType type, const QUrl &proxyUrl) :
    QCoapMessage(*new QCoapRequestPrivate(url, type, proxyUrl))
{
}

/*!
    Constructs a QCoapRequest from a string literal
*/
QCoapRequest::QCoapRequest(const char *url, MessageType type) :
    QCoapMessage(*new QCoapRequestPrivate(QUrl(QString::fromUtf8(url)), type))
{
}

/*!
    Constructs a copy of the \a other QCoapRequest. Optionally allows to
    overwrite the QtCoap::Method of the request with the \a method
    argument.
*/
QCoapRequest::QCoapRequest(const QCoapRequest &other, QtCoap::Method method) :
    //! No private data sharing, as QCoapRequestPrivate!=QCoapMessagePrivate
    //! and the d_ptr is a QSharedDataPointer<QCoapMessagePrivate>
    QCoapMessage(*new QCoapRequestPrivate(*other.d_func()))
{
    if (method != QtCoap::Invalid)
        setMethod(method);
}

/*!
    Destroys the QCoapRequest.
*/
QCoapRequest::~QCoapRequest()
{
}

/*!
    Returns the target URI of the request.

    \sa setUrl()
*/
QUrl QCoapRequest::url() const
{
    Q_D(const QCoapRequest);
    return d->uri;
}

/*!
    Returns the proxy URI of the request.
    The request shall be sent directly if this is invalid.

    \sa setProxyUrl()
*/
QUrl QCoapRequest::proxyUrl() const
{
    Q_D(const QCoapRequest);
    return d->proxyUri;
}

/*!
    Returns the method of the request.

    \sa setMethod()
*/
QtCoap::Method QCoapRequest::method() const
{
    Q_D(const QCoapRequest);
    return d->method;
}

/*!
    Returns true if the request is an observe request.

    \sa enableObserve()
*/
bool QCoapRequest::isObserve() const
{
    return hasOption(QCoapOption::Observe);
}

/*!
    Sets the target URI of the request to the given \a url.

    If not indicated, the scheme of the URL will default to 'coap', and its
    port will default to 5683.

    \sa url()
*/
void QCoapRequest::setUrl(const QUrl &url)
{
    Q_D(QCoapRequest);
    d->setUrl(url);
}

/*!
    Sets the proxy URI of the request to the given \a proxyUrl.

    \sa proxyUrl()
*/
void QCoapRequest::setProxyUrl(const QUrl &proxyUrl)
{
    Q_D(QCoapRequest);
    d->proxyUri = proxyUrl;
}

/*!
    Sets the method of the request to the given \a method.

    \sa method()
*/
void QCoapRequest::setMethod(QtCoap::Method method)
{
    Q_D(QCoapRequest);
    d->method = method;
}

/*!
    Sets the observe to true to make an observe request.

    \sa isObserve()
*/
void QCoapRequest::enableObserve()
{
    if (isObserve())
        return;

    addOption(QCoapOption::Observe);
}

/*!
    Creates a copy of \a other.
*/
QCoapRequest &QCoapRequest::operator=(const QCoapRequest &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns \c true if the request is valid, \c false otherwise.
*/
bool QCoapRequest::isValid() const
{
    return isUrlValid(url()) && method() != QtCoap::Invalid;
}

/*!
    Returns true if the \a url is a valid CoAP URL.
*/
bool QCoapRequest::isUrlValid(const QUrl &url)
{
    return (url.isValid() && !url.isLocalFile() && !url.isRelative()
            && url.scheme() == QLatin1String("coap")
            && !url.hasFragment());
}

/*!
    \internal

    For QSharedDataPointer.
*/
QCoapRequestPrivate* QCoapRequest::d_func()
{
    return static_cast<QCoapRequestPrivate*>(d_ptr.data());
}

QT_END_NAMESPACE
