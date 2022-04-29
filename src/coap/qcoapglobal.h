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

#ifndef QCOAPGLOBAL_H
#define QCOAPGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

typedef QByteArray QCoapToken;
typedef quint16 QCoapMessageId;

#ifndef QT_STATIC
#  ifdef QT_BUILD_COAP_LIB
#    define Q_COAP_EXPORT Q_DECL_EXPORT
#  else
#    define Q_COAP_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_COAP_EXPORT
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QCoapToken)
Q_DECLARE_METATYPE(QCoapMessageId)

#endif // QCOAPGLOBAL_H
