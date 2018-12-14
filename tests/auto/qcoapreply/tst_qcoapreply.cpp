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

#include <QtCoap/qcoapreply.h>
#include <private/qcoapreply_p.h>

class tst_QCoapReply : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void updateReply_data();
    void updateReply();
    void requestData();
    void abortRequest();
};

class QCoapReplyForTests : public QCoapReply
{
public:
    QCoapReplyForTests(const QCoapRequest &req) : QCoapReply (req) {}

    void setRunning(const QCoapToken &token, QCoapMessageId messageId)
    {
        Q_D(QCoapReply);
        d->_q_setRunning(token, messageId);
    }
};

void tst_QCoapReply::updateReply_data()
{
    QTest::addColumn<QByteArray>("payload");
    QTest::addColumn<QtCoap::ResponseCode>("responseCode");
    QTest::addColumn<QtCoap::Error>("error");

    QTest::newRow("success")
            << QByteArray("Some data")
            << QtCoap::Content
            << QtCoap::NoError;
    QTest::newRow("content error")
            << QByteArray("Error")
            << QtCoap::BadRequest
            << QtCoap::NoError;
    QTest::newRow("finished error")
            << QByteArray("Error")
            << QtCoap::Content
            << QtCoap::BadRequestError;
    QTest::newRow("content & finished errors")
            << QByteArray("2Errors")
            << QtCoap::BadGateway
            << QtCoap::BadRequestError;
}

void tst_QCoapReply::updateReply()
{
    QFETCH(QByteArray, payload);
    QFETCH(QtCoap::ResponseCode, responseCode);
    QFETCH(QtCoap::Error, error);

    const QByteArray token = "\xAF\x01\xC2";
    const quint16 id = 645;

    QCoapReply reply(QCoapRequest{});
    QCoapMessage message;
    message.setToken(token);
    message.setMessageId(id);
    message.setPayload(payload);

    QSignalSpy spyReplyFinished(&reply, &QCoapReply::finished);
    QSignalSpy spyReplyNotified(&reply, &QCoapReply::notified);
    QSignalSpy spyReplyError(&reply, &QCoapReply::error);
    QSignalSpy spyReplyAborted(&reply, &QCoapReply::aborted);

    QMetaObject::invokeMethod(&reply, "_q_setContent",
                              Q_ARG(QHostAddress, QHostAddress()),
                              Q_ARG(QCoapMessage, message),
                              Q_ARG(QtCoap::ResponseCode, responseCode));
    QMetaObject::invokeMethod(&reply, "_q_setFinished",
                              Q_ARG(QtCoap::Error, error));

    QCOMPARE(spyReplyFinished.count(), 1);
    QCOMPARE(spyReplyNotified.count(), 0);
    QCOMPARE(spyReplyAborted.count(), 0);
    if (error != QtCoap::NoError || QtCoap::isError(responseCode)) {
        QVERIFY(spyReplyError.count() > 0);
        QCOMPARE(reply.isSuccessful(), false);
    } else {
        QCOMPARE(spyReplyError.count(), 0);
        QCOMPARE(reply.isSuccessful(), true);
    }

    QCOMPARE(reply.readAll(), payload);
    QCOMPARE(reply.readAll(), QByteArray());
    QCOMPARE(reply.responseCode(), responseCode);
    QCOMPARE(reply.message().token(), token);
    QCOMPARE(reply.message().messageId(), id);
}

void tst_QCoapReply::requestData()
{
#ifdef QT_BUILD_INTERNAL
    QCoapReplyForTests reply((QCoapRequest()));
    reply.setRunning("token", 543);

    QCOMPARE(reply.request().token(), QByteArray("token"));
    QCOMPARE(reply.request().messageId(), 543);
#else
    QSKIP("Not an internal build, skipping this test");
#endif
}

void tst_QCoapReply::abortRequest()
{
#ifdef QT_BUILD_INTERNAL
    QCoapReplyForTests reply((QCoapRequest()));
    reply.setRunning("token", 543);

    QSignalSpy spyAborted(&reply, &QCoapReply::aborted);
    QSignalSpy spyFinished(&reply, &QCoapReply::finished);
    reply.abortRequest();

    QTRY_COMPARE_WITH_TIMEOUT(spyAborted.count(), 1, 1000);
    QList<QVariant> arguments = spyAborted.takeFirst();
    QTRY_COMPARE_WITH_TIMEOUT(spyFinished.count(), 1, 1000);
    QVERIFY(arguments.at(0).toByteArray() == "token");
    QCOMPARE(reply.isSuccessful(), false);
#else
    QSKIP("Not an internal build, skipping this test");
#endif
}

QTEST_MAIN(tst_QCoapReply)

#include "tst_qcoapreply.moc"
