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

#ifndef QCOAPINTERNALMESSAGE_P_H
#define QCOAPINTERNALMESSAGE_P_H

#include <private/qcoapmessage_p.h>
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

class QCoapInternalMessagePrivate;
class Q_AUTOTEST_EXPORT QCoapInternalMessage : public QObject
{
    Q_OBJECT
public:
    explicit QCoapInternalMessage(QObject *parent = nullptr);
    explicit QCoapInternalMessage(const QCoapMessage &message, QObject *parent = nullptr);
    virtual ~QCoapInternalMessage() {}

    void addOption(QCoapOption::OptionName name, const QByteArray &value);
    void addOption(QCoapOption::OptionName name, quint32 value);
    virtual void addOption(const QCoapOption &option);
    void removeOption(QCoapOption::OptionName name);

    QCoapMessage *message();
    const QCoapMessage *message() const;

    uint currentBlockNumber() const;
    bool hasMoreBlocksToReceive() const;
    uint blockSize() const;

    virtual bool isValid() const;
    static bool isUrlValid(const QUrl &url);

protected:
    explicit QCoapInternalMessage(QCoapInternalMessagePrivate &dd, QObject *parent = nullptr);

    void setFromDescriptiveBlockOption(const QCoapOption &option);

    Q_DECLARE_PRIVATE(QCoapInternalMessage)
};

class Q_AUTOTEST_EXPORT QCoapInternalMessagePrivate : public QObjectPrivate
{
public:
    QCoapInternalMessagePrivate() = default;
    ~QCoapInternalMessagePrivate();

    QCoapMessage message;

    uint currentBlockNumber = 0;
    bool hasNextBlock = false;
    uint blockSize = 0;
};

QT_END_NAMESPACE

#endif // QCOAPINTERNALMESSAGE_P_H
