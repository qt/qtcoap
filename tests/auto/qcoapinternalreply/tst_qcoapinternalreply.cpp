// Copyright (C) 2018 Witekio.
// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest>
#include <QCoreApplication>

#include <private/qcoapinternalreply_p.h>
#include <private/qcoapreply_p.h>

class tst_QCoapInternalReply : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void parseReplyPdu_data();
    void parseReplyPdu();
    void parseInvalidReplyPdu_data();
    void parseInvalidReplyPdu();
    void parseExtendedOption();
    void parseEmptyBlockOption();
    void nextBlockToSendWithEmptyBlock1();
    void updateReply_data();
    void updateReply();
};

void tst_QCoapInternalReply::parseReplyPdu_data()
{
    QTest::addColumn<QtCoap::ResponseCode>("responseCode");
    QTest::addColumn<QCoapMessage::Type>("type");
    QTest::addColumn<quint16>("messageId");
    QTest::addColumn<QByteArray>("token");
    QTest::addColumn<quint8>("tokenLength");
    QTest::addColumn<QList<QCoapOption::OptionName>>("optionsNames");
    QTest::addColumn<QList<quint8>>("optionsLengths");
    QTest::addColumn<QList<QByteArray>>("optionsValues");
    QTest::addColumn<QString>("payload");
    QTest::addColumn<QString>("pduHexa");

    QList<QCoapOption::OptionName> optionsNamesReply({QCoapOption::ContentFormat,
                                                      QCoapOption::MaxAge});
    QList<quint8> optionsLengthsReply({0, 1});
    QList<QByteArray> optionsValuesReply({"", QByteArray::fromHex("1e")});

    QList<QCoapOption::OptionName> bigOptionNameReply({QCoapOption::Size1});
    QList<quint8> bigOptionLengthReply({26});
    QList<QByteArray> bigOptionValueReply({QByteArray("abcdefghijklmnopqrstuvwxyz")});

    QTest::newRow("reply_with_options_and_payload")
            << QtCoap::ResponseCode::Content
            << QCoapMessage::Type::NonConfirmable
            << quint16(64463)
            << QByteArray("4647f09b")
            << quint8(4)
            << optionsNamesReply
            << optionsLengthsReply
            << optionsValuesReply
            << "Type: 1 (NON)\nCode: 1 (GET)\nMID: 56400\nToken: 4647f09b"
            << "5445fbcf4647f09bc0211eff547970653a203120284e4f4e290a436f64653a20"
               "312028474554290a4d49443a2035363430300a546f6b656e3a20343634376630"
               "3962";

    QTest::newRow("reply_with_payload")
            << QtCoap::ResponseCode::Content
            << QCoapMessage::Type::NonConfirmable
            << quint16(64463)
            << QByteArray("4647f09b")
            << quint8(4)
            << QList<QCoapOption::OptionName>()
            << QList<quint8>()
            << QList<QByteArray>()
            << "Type: 1 (NON)\nCode: 1 (GET)\nMID: 56400\nToken: 4647f09b"
            << "5445fbcf4647f09bff547970653a203120284e4f4e290a436f64653a20312028"
               "474554290a4d49443a2035363430300a546f6b656e3a203436343766303962";

    QTest::newRow("reply_with_options")
            << QtCoap::ResponseCode::Content
            << QCoapMessage::Type::NonConfirmable
            << quint16(64463)
            << QByteArray("4647f09b")
            << quint8(4)
            << optionsNamesReply
            << optionsLengthsReply
            << optionsValuesReply
            << ""
            << "5445fbcf4647f09bc0211e";

    QTest::newRow("reply_only")
            << QtCoap::ResponseCode::Content
            << QCoapMessage::Type::NonConfirmable
            << quint16(64463)
            << QByteArray("4647f09b")
            << quint8(4)
            << QList<QCoapOption::OptionName>()
            << QList<quint8>()
            << QList<QByteArray>()
            << ""
            << "5445fbcf4647f09b";

    QTest::newRow("reply_with_big_option")
            << QtCoap::ResponseCode::Content
            << QCoapMessage::Type::NonConfirmable
            << quint16(64463)
            << QByteArray("4647f09b")
            << quint8(4)
            << bigOptionNameReply
            << bigOptionLengthReply
            << bigOptionValueReply
            << ""
            << "5445fbcf4647f09bdd2f0d6162636465666768696a6b6c6d6e6f707172737475"
               "767778797a";
}

void tst_QCoapInternalReply::parseReplyPdu()
{
    QFETCH(QtCoap::ResponseCode, responseCode);
    QFETCH(QCoapMessage::Type, type);
    QFETCH(quint16, messageId);
    QFETCH(QByteArray, token);
    QFETCH(quint8, tokenLength);
    QFETCH(QList<QCoapOption::OptionName>, optionsNames);
    QFETCH(QList<quint8>, optionsLengths);
    QFETCH(QList<QByteArray>, optionsValues);
    QFETCH(QString, payload);
    QFETCH(QString, pduHexa);

    QScopedPointer<QCoapInternalReply>
            reply(QCoapInternalReply::createFromFrame(QByteArray::fromHex(pduHexa.toUtf8())));

    QCOMPARE(reply->message()->type(), type);
    QCOMPARE(reply->message()->tokenLength(), tokenLength);
    QCOMPARE(reply->responseCode(), responseCode);
    QCOMPARE(reply->message()->messageId(), messageId);
    QCOMPARE(reply->message()->token().toHex(), token);
    QCOMPARE(reply->message()->optionCount(), optionsNames.size());
    for (int i = 0; i < reply->message()->optionCount(); ++i) {
        QCoapOption option = reply->message()->optionAt(i);
        QCOMPARE(option.name(), optionsNames.at(i));
        QCOMPARE(option.length(), optionsLengths.at(i));
        QCOMPARE(option.opaqueValue(), optionsValues.at(i));
    }
    QCOMPARE(reply->message()->payload(), payload.toUtf8());
}

void tst_QCoapInternalReply::parseInvalidReplyPdu_data()
{
    QTest::addColumn<QByteArray>("frame");

    // A CoAP message is at least 4 bytes long (the fixed header). Frames shorter
    // than that must be rejected before the header is dereferenced;
    // createFromFrame() signals rejection by returning nullptr.
    QTest::newRow("empty")       << QByteArray();
    QTest::newRow("one_byte")    << QByteArray::fromHex("50");
    QTest::newRow("two_bytes")   << QByteArray::fromHex("5045");
    QTest::newRow("three_bytes") << QByteArray::fromHex("5045fb");

    // Option-parser cases: each frame has a valid 4-byte header (no token)
    // followed by a single option byte whose encoding demands bytes that are
    // not present. createFromFrame() must reject the frame rather than read
    // past the buffer or deliver a silently truncated option.

    // 0x09: option value length 9, but no value bytes follow it.
    QTest::newRow("option_value_overruns_buffer")   << QByteArray::fromHex("50450001" "09");
    // 0xd0: delta nibble 13 promises one extended delta byte that is missing.
    QTest::newRow("missing_option_delta_ext_byte")  << QByteArray::fromHex("50450001" "d0");
    // 0x0d: length nibble 13 promises one extended length byte that is missing.
    QTest::newRow("missing_option_length_ext_byte") << QByteArray::fromHex("50450001" "0d");
    // 0x0d + 0xff: extended length yields optionLength 268, far past the buffer.
    QTest::newRow("extended_option_length_overruns") << QByteArray::fromHex("50450001" "0d" "ff");
    // 0xe0 + 0xffff: extended delta 0xffff -> option number 65535 + 269 = 65804,
    // which exceeds the 65535 maximum, so the frame is rejected.
    QTest::newRow("option_number_exceeds_max") << QByteArray::fromHex("50450001" "e0" "ffff");

    // Token-length cases.
    // TKL 9 is reserved (RFC 7252 §3) and must be rejected even when the 9 token
    // bytes are present, so this frame includes them.
    QTest::newRow("reserved_token_length") << QByteArray::fromHex("59450001" "aabbccddeeff001122");
    // TKL 4 is valid, but no token bytes follow the 4-byte header.
    QTest::newRow("truncated_token")       << QByteArray::fromHex("54450001");

    // RFC 7959 §2.2: SZX value 7 (block size 2048) is reserved. Reject a block
    // option whose last byte has SZX 7 (low 3 bits set). Value byte 0x07 encodes
    // NUM=0, M=0, SZX=7. Option header 0xd1 = delta 13 (extended), length 1.
    // Block2 (number 23): extended delta byte 0x0a -> 10 + 13 = 23.
    QTest::newRow("reserved_block2_szx7") << QByteArray::fromHex("50450001" "d10a" "07");
    // Block1 (number 27): extended delta byte 0x0e -> 14 + 13 = 27.
    QTest::newRow("reserved_block1_szx7") << QByteArray::fromHex("50450001" "d10e" "07");
}

void tst_QCoapInternalReply::parseInvalidReplyPdu()
{
    QFETCH(QByteArray, frame);

    QScopedPointer<QCoapInternalReply> reply(QCoapInternalReply::createFromFrame(frame));
    QVERIFY2(reply.isNull(),
             "createFromFrame() must reject a malformed frame by returning nullptr");
}

void tst_QCoapInternalReply::parseExtendedOption()
{
    // RFC 7252 §3.1: a delta/length nibble of 14 is followed by a 16-bit
    // unsigned integer in network byte order (the value minus 269). Build one
    // option that uses the 16-bit form for BOTH its delta and its length, with
    // distinct first/second bytes so the test also verifies the byte order:
    //   0xee       -> delta nibble 14, length nibble 14
    //   0x01 0x02  -> extended delta  = 0x0102 = 258 -> option number 258 + 269 = 527
    //   0x00 0x03  -> extended length = 0x0003 =   3 -> value length     3 + 269 = 272
    //   272 value bytes
    // A decoder that read one byte instead of two, or used the wrong byte order,
    // would compute different numbers and misalign the rest of the frame.
    const QByteArray value(272, 'x');
    const QByteArray frame = QByteArray::fromHex("50450001" "ee" "0102" "0003") + value;

    QScopedPointer<QCoapInternalReply> reply(QCoapInternalReply::createFromFrame(frame));

    QVERIFY(!reply.isNull());
    QCOMPARE(reply->message()->optionCount(), 1);
    const QCoapOption option = reply->message()->options().constFirst();
    QCOMPARE(static_cast<int>(option.name()), 527);
    QCOMPARE(option.opaqueValue(), value);
}

void tst_QCoapInternalReply::parseEmptyBlockOption()
{
    // RFC 7959 §2.2: a zero-length Block option is valid and encodes
    // NUM=0, M=0, SZX=0. Frame: 4-byte header (no token), then a Block2
    // option (number 23) carrying an empty value.
    //   0xd0 -> option delta nibble 13 (extended), length nibble 0
    //   0x0a -> extended delta byte, giving option number 13 + 10 = 23 (Block2)
    const QByteArray frame = QByteArray::fromHex("50450001" "d0" "0a");
    QScopedPointer<QCoapInternalReply> reply(QCoapInternalReply::createFromFrame(frame));

    QVERIFY(!reply.isNull());
    QCOMPARE(reply->currentBlockNumber(), 0u);
    QVERIFY(!reply->hasMoreBlocksToReceive());
    QCOMPARE(reply->blockSize(), 16u);
}

void tst_QCoapInternalReply::nextBlockToSendWithEmptyBlock1()
{
    // A zero-length Block1 option is valid (RFC 7959 §2.2), encoding M=0, so
    // there is no next block to send. nextBlockToSend() must return -1 rather
    // than read optionData[-1]. Frame: 4-byte header, then a Block1 option
    // (number 27) with an empty value.
    //   0xd0 -> option delta nibble 13 (extended), length nibble 0
    //   0x0e -> extended delta byte, giving option number 13 + 14 = 27 (Block1)
    const QByteArray frame = QByteArray::fromHex("50450001" "d0" "0e");
    QScopedPointer<QCoapInternalReply> reply(QCoapInternalReply::createFromFrame(frame));

    QVERIFY(!reply.isNull());
    QCOMPARE(reply->nextBlockToSend(), -1);
}

void tst_QCoapInternalReply::updateReply_data()
{
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("success") << QByteArray("Data for the updating test");
}

void tst_QCoapInternalReply::updateReply()
{
    QFETCH(QByteArray, data);

    QScopedPointer<QCoapReply> reply(QCoapReplyPrivate::createCoapReply(QCoapRequest()));
    QCoapInternalReply internalReply;
    internalReply.message()->setPayload(data);
    QSignalSpy spyReplyFinished(reply.data(), &QCoapReply::finished);

    QMetaObject::invokeMethod(reply.data(), "_q_setContent",
                              Q_ARG(QHostAddress, internalReply.senderAddress()),
                              Q_ARG(QCoapMessage, *internalReply.message()),
                              Q_ARG(QtCoap::ResponseCode, internalReply.responseCode()));
    QMetaObject::invokeMethod(reply.data(), "_q_setFinished", Q_ARG(QtCoap::Error, QtCoap::Error::Ok));

    QTRY_COMPARE_WITH_TIMEOUT(spyReplyFinished.size(), 1, 1000);
    QCOMPARE(reply->readAll(), data);
}

QTEST_MAIN(tst_QCoapInternalReply)

#include "tst_qcoapinternalreply.moc"
