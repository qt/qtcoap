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

#include "qcoapmessage_p.h"

QT_BEGIN_NAMESPACE

QCoapMessagePrivate::QCoapMessagePrivate(QCoapMessage::MessageType _type) :
    type(_type)
{
}

QCoapMessagePrivate::QCoapMessagePrivate(const QCoapMessagePrivate &other) :
    QSharedData(other),
    version(other.version), type(other.type), messageId(other.messageId),
    token(other.token), options(other.options), payload(other.payload)
{
}

QCoapMessagePrivate::~QCoapMessagePrivate()
{
}

/*!
    \class QCoapMessage
    \brief The QCoapMessage class holds information about a CoAP message that
    can be a request or a reply.

    \reentrant

    It holds information such as the message type, message id, token and other
    ancillary data.

    \sa QCoapOption, QCoapReply, QCoapRequest
*/

/*!
    \enum QCoapMessage::QCoapMessageType

    Indicates the type of the message.

    \value ConfirmableCoapMessage       A Confirmable message. The destination
                                        endpoint needs to acknowledge the
                                        message.
    \value NonConfirmableCoapMessage    A Non-Confirmable message. The
                                        destination endpoint does not need to
                                        acknowledge the message.
    \value AcknowledgmentCoapMessage    An Acknowledgment message. A message
                                        sent or received in reply to a
                                        Confirmable message.
    \value ResetCoapMessage             A Reset message. A message sent
                                        sometimes in case of errors or to stop
                                        transmissions. (For example, it is used
                                        to cancel an observation).
*/

/*!
    Constructs a new QCoapMessage.
*/
QCoapMessage::QCoapMessage() :
    d_ptr(new QCoapMessagePrivate)
{
}

/*!
    Destroys the QCoapMessage.
*/
QCoapMessage::~QCoapMessage()
{
}

/*!
    Constructs a shallow copy of \a other.
*/
QCoapMessage::QCoapMessage(const QCoapMessage &other) :
    d_ptr(other.d_ptr)
{
}

/*!
    \internal
    Constructs a new QCoapMessage with \a dd as the d_ptr.
    This constructor must be used internally when subclassing
    the QCoapMessage class.
*/
QCoapMessage::QCoapMessage(QCoapMessagePrivate &dd) :
    d_ptr(&dd)
{
}

/*!
    \overload

    Adds the CoAP option with the given \a name and \a value.
*/
void QCoapMessage::addOption(QCoapOption::OptionName name, const QByteArray &value)
{
    QCoapOption option(name, value);
    addOption(option);
}

/*!
    Adds the given CoAP \a option.
*/
void QCoapMessage::addOption(const QCoapOption &option)
{
    Q_D(QCoapMessage);

    // Sort options by ascending order while inserting
    d->options.insert(
                std::upper_bound(d->options.begin(), d->options.end(), option,
                                 [](const QCoapOption &a, const QCoapOption &b) -> bool {
                                     return a.name() < b.name();
                                 }),
                option
            );
}

/*!
    Removes the given \a option.
*/
void QCoapMessage::removeOption(const QCoapOption &option)
{
    Q_D(QCoapMessage);
    d->options.removeOne(option);
}

/*!
    Removes all options with the given \a name.
    The CoAP protocol allows for the same option to repeat.
*/
void QCoapMessage::removeOption(QCoapOption::OptionName name)
{
    Q_D(QCoapMessage);
    auto namesMatch = [name](const QCoapOption &option) {
        return option.name() == name;
    };

    auto &options = d->options;
    options.erase(std::remove_if(options.begin(), options.end(), namesMatch),
                  options.end());
}

/*!
    Removes all options.
*/
void QCoapMessage::removeAllOptions()
{
    Q_D(QCoapMessage);
    d->options.clear();
}

/*!
    Returns the CoAP version.

    \sa setVersion()
*/
quint8 QCoapMessage::version() const
{
    Q_D(const QCoapMessage);
    return d->version;
}

/*!
    Returns the message type.

    \sa setType()
*/
QCoapMessage::MessageType QCoapMessage::type() const
{
    Q_D(const QCoapMessage);
    return d->type;
}

/*!
    Returns the message token.

    \sa setToken()
*/
QByteArray QCoapMessage::token() const
{
    Q_D(const QCoapMessage);
    return d->token;
}

/*!
    Returns the token length.
*/
quint8 QCoapMessage::tokenLength() const
{
    Q_D(const QCoapMessage);
    return static_cast<quint8>(d->token.length());
}

/*!
    Returns the message id.

    \sa setMessageId()
*/
quint16 QCoapMessage::messageId() const
{
    Q_D(const QCoapMessage);
    return d->messageId;
}

/*!
    Returns the payload.

    \sa setPayload()
*/
QByteArray QCoapMessage::payload() const
{
    Q_D(const QCoapMessage);
    return d->payload;
}

/*!
    Returns the option at \a index position.
*/
QCoapOption QCoapMessage::option(int index) const
{
    Q_D(const QCoapMessage);
    return d->options.at(index);
}

/*!
    Finds and returns the first option with the given \a name.
    If there is no such option, returns an Invalid CoapOption with an empty value.
*/
QCoapOption QCoapMessage::option(QCoapOption::OptionName name) const
{
    Q_D(const QCoapMessage);

    auto it = findOption(name);
    return it != d->options.end() ? *it : QCoapOption();
}

/*!
    Finds and returns a constant iterator to the first option
    with the given \a name.
    If there is no such option, returns \c d->options.end().
*/
QVector<QCoapOption>::const_iterator QCoapMessage::findOption(QCoapOption::OptionName name) const
{
    Q_D(const QCoapMessage);
    return std::find_if(d->options.begin(), d->options.end(), [name](const QCoapOption &option) {
        return option.name() == name;
    });
}

/*!
    Returns \c true if option is present at least once.
*/
bool QCoapMessage::hasOption(QCoapOption::OptionName name) const
{
    Q_D(const QCoapMessage);
    return findOption(name) != d->options.end();
}

/*!
    Returns the list of options.
*/
const QVector<QCoapOption> &QCoapMessage::options() const
{
    Q_D(const QCoapMessage);
    return d->options;
}

/*!
    Returns the number of options.
*/
int QCoapMessage::optionCount() const
{
    Q_D(const QCoapMessage);
    return d->options.length();
}

/*!
    Sets the CoAP version.

    \sa version()
*/
void QCoapMessage::setVersion(quint8 version)
{
    Q_D(QCoapMessage);
    d->version = version;
}

/*!
    Sets the message type.

    \sa type()
*/
void QCoapMessage::setType(const MessageType &type)
{
    Q_D(QCoapMessage);
    d->type = type;
}

/*!
    Sets the token.

    \sa token()
*/
void QCoapMessage::setToken(const QByteArray &token)
{
    Q_D(QCoapMessage);
    d->token = token;
}

/*!
    Sets the message id.

    \sa messageId()
*/
void QCoapMessage::setMessageId(quint16 id)
{
    Q_D(QCoapMessage);
    d->messageId = id;
}

/*!
    Sets the payload.

    \sa payload()
*/
void QCoapMessage::setPayload(const QByteArray &payload)
{
    Q_D(QCoapMessage);
    d->payload = payload;
}

void QCoapMessage::swap(QCoapMessage &other) Q_DECL_NOTHROW
{
    qSwap(d_ptr, other.d_ptr);
}

/*!
    Move assignment operator.
 */
QCoapMessage &QCoapMessage::operator=(QCoapMessage &&other) Q_DECL_NOTHROW
{
    swap(other);
    return *this;
}

/*!
    Assignment operator.
 */
QCoapMessage &QCoapMessage::operator=(const QCoapMessage &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    \internal

    For QSharedDataPointer.
*/
QCoapMessagePrivate* QCoapMessage::d_func()
{
    return d_ptr.data();
}

QT_END_NAMESPACE
