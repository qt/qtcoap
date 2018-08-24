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

#include <QtTest>
#include <QCoreApplication>

#include <QtCoap/qcoapmessage.h>

class tst_QCoapMessage : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void copyAndDetach();
    void setMessageType_data();
    void setMessageType();
    void addOption_string();
    void addOption_uint_data();
    void addOption_uint();
    void removeOption();
    void urlOptions();
};

void tst_QCoapMessage::copyAndDetach()
{
    QCoapMessage a;
    a.setMessageId(3);
    a.setPayload("payload");
    a.setToken("token");
    a.setType(QCoapMessage::Acknowledgment);
    a.setVersion(5);

    // Test the copy
    QCoapMessage b(a);
    QVERIFY2(b.messageId() == 3, "Message not copied correctly");
    QVERIFY2(b.payload() == "payload", "Message not copied correctly");
    QVERIFY2(b.token() == "token", "Message not copied correctly");
    QVERIFY2(b.type() == QCoapMessage::Acknowledgment, "Message not copied correctly");
    QVERIFY2(b.version() == 5, "Message not copied correctly");

    // Detach
    b.setMessageId(9);
    QCOMPARE(b.messageId(), 9);
    QCOMPARE(a.messageId(), 3);
}

void tst_QCoapMessage::setMessageType_data()
{
    QTest::addColumn<QCoapMessage::MessageType>("type");

    QTest::newRow("acknowledgment") << QCoapMessage::Acknowledgment;
    QTest::newRow("confirmable") << QCoapMessage::Confirmable;
    QTest::newRow("non-confirmable") << QCoapMessage::NonConfirmable;
    QTest::newRow("reset") << QCoapMessage::Reset;
}

void tst_QCoapMessage::setMessageType()
{
    QFETCH(QCoapMessage::MessageType, type);
    QCoapMessage message;
    message.setType(type);
    QCOMPARE(message.type(), type);

    //! TODO extend QCoapMessage tests
}

void tst_QCoapMessage::addOption_string()
{
    //! TODO with one and more than one identical options
}

void tst_QCoapMessage::addOption_uint_data()
{
    QTest::addColumn<quint32>("value");
    QTest::addColumn<int>("size");

    QTest::newRow("4 bytes") << (quint32)0xF0aF0010 << 4;
    QTest::newRow("3 bytes") << (quint32)0x300010 << 3;
    QTest::newRow("2 bytes") << (quint32)0x5010 << 2;
    QTest::newRow("1 byte")  << (quint32)0x80 << 1;
}

void tst_QCoapMessage::addOption_uint()
{
    QFETCH(quint32, value);
    QFETCH(int, size);

    QCoapOption option(QCoapOption::Block1, value);

    QCOMPARE(option.valueToInt(), value);
    QCOMPARE(option.value().size(), size);
}

void tst_QCoapMessage::removeOption()
{
    //! TODO with one and more than one identical options
}

void tst_QCoapMessage::urlOptions()
{
    //! TODO Test the following from the RFC:
    // For example, the following three URIs are equivalent and cause the
    // same options and option values to appear in the CoAP messages:
    // coap://example.com:5683/~sensors/temp.xml
    // coap://EXAMPLE.com/%7Esensors/temp.xml
    // coap://EXAMPLE.com:/%7esensors/temp.xml
}

QTEST_APPLESS_MAIN(tst_QCoapMessage)

#include "tst_qcoapmessage.moc"
