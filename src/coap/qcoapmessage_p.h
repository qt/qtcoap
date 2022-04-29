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

#ifndef QCOAPMESSAGE_P_H
#define QCOAPMESSAGE_P_H

#include <QtCoap/qcoapmessage.h>
#include <QtCore/qshareddata.h>
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

class Q_AUTOTEST_EXPORT QCoapMessagePrivate : public QSharedData
{
public:
    QCoapMessagePrivate(QCoapMessage::Type type = QCoapMessage::Type::NonConfirmable);
    QCoapMessagePrivate(const QCoapMessagePrivate &other);
    ~QCoapMessagePrivate();

    QList<QCoapOption>::const_iterator findOption(QCoapOption::OptionName name) const;

    quint8 version = 1;
    QCoapMessage::Type type = QCoapMessage::Type::NonConfirmable;
    quint16 messageId = 0;
    QByteArray token;
    QList<QCoapOption> options;
    QByteArray payload;
};

QT_END_NAMESPACE

#endif // QCOAPMESSAGE_P_H
