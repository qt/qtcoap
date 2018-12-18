/****************************************************************************
**
** Copyright (C) 2018 Witekio.
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

#include <QtCoap/qcoaprequest.h>
#include <private/qcoapinternalrequest_p.h>

#ifdef QT_BUILD_INTERNAL

class tst_QCoapInternalRequest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void requestToFrame_data();
    void requestToFrame();
    void parseUri_data();
    void parseUri();
    void urlOptions_data();
    void urlOptions();
    void invalidUrls_data();
    void invalidUrls();
};

void tst_QCoapInternalRequest::requestToFrame_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<QtCoap::Method>("method");
    QTest::addColumn<QCoapMessage::MessageType>("type");
    QTest::addColumn<quint16>("messageId");
    QTest::addColumn<QByteArray>("token");
    QTest::addColumn<QString>("pduHeader");
    QTest::addColumn<QString>("pduPayload");

    QTest::newRow("request_with_option_and_payload")
        << QUrl("coap://10.20.30.40:5683/test")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09bb474657374ff"
        << "Some payload";

    QTest::newRow("request_domain")
        << QUrl("coap://domain.com:5683/test")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09b3a646f6d61696e2e636f6d8474657374ff"
        << "Some payload";

    QTest::newRow("request_ipv6")
        << QUrl("coap://[::ffff:ac11:3]:5683/test")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09bb474657374ff"
        << "Some payload";

    QTest::newRow("request_without_payload")
        << QUrl("coap://10.20.30.40:5683/test")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09bb474657374"
        << "";

    QTest::newRow("request_without_option")
        << QUrl("coap://10.20.30.40:5683/")
        << QtCoap::Put
        << QCoapRequest::Confirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "4403dc504647f09bff"
        << "Some payload";

    QTest::newRow("request_only")
        << QUrl("coap://10.20.30.40:5683/")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09b"
        << "";

    QTest::newRow("request_with_multiple_options")
        << QUrl("coap://10.20.30.40:5683/test/oui")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09bb474657374036f7569"
        << "";

    QTest::newRow("request_with_big_option_number")
        << QUrl("coap://10.20.30.40:5683/test")
        << QtCoap::Get
        << QCoapRequest::NonConfirmable
        << quint16(56400)
        << QByteArray::fromHex("4647f09b")
        << "5401dc504647f09bb474657374dd240d6162636465666768696a6b6c6d6e6f70"
           "7172737475767778797aff"
        << "Some payload";
}

void tst_QCoapInternalRequest::requestToFrame()
{
    QFETCH(QUrl, url);
    QFETCH(QtCoap::Method, method);
    QFETCH(QCoapMessage::MessageType, type);
    QFETCH(quint16, messageId);
    QFETCH(QByteArray, token);
    QFETCH(QString, pduHeader);
    QFETCH(QString, pduPayload);

    QCoapRequest request(url);
    request.setType(type);
    request.setMethod(method);
    request.setPayload(pduPayload.toUtf8());
    request.setMessageId(messageId);
    request.setToken(token);
    if (qstrcmp(QTest::currentDataTag(), "request_with_big_option_number") == 0)
        request.addOption(QCoapOption::Size1, QByteArray("abcdefghijklmnopqrstuvwxyz"));

    QByteArray pdu;
    pdu.append(pduHeader);
    if (!pduPayload.isEmpty())
        pdu.append(pduPayload.toUtf8().toHex());

    QCoapInternalRequest internalRequest(request);
    QCOMPARE(internalRequest.toQByteArray().toHex(), pdu);
}

void tst_QCoapInternalRequest::parseUri_data()
{
    qRegisterMetaType<QVector<QCoapOption>>();
    QTest::addColumn<QUrl>("uri");
    QTest::addColumn<QUrl>("proxyUri");
    QTest::addColumn<QVector<QCoapOption>>("options");

    QTest::newRow("port_path")
                        << QUrl("coap://10.20.30.40:1234/test/path1")
                        << QUrl()
                        << QVector<QCoapOption>({
                            QCoapOption(QCoapOption::UriPort, 1234),
                            QCoapOption(QCoapOption::UriPath, "test"),
                            QCoapOption(QCoapOption::UriPath, "path1") });

    QTest::newRow("path_query")
                        << QUrl("coap://10.20.30.40/test/path1/?rd=25&nd=4")
                        << QUrl()
                        << QVector<QCoapOption>({
                            QCoapOption(QCoapOption::UriPath, "test"),
                            QCoapOption(QCoapOption::UriPath, "path1"),
                            QCoapOption(QCoapOption::UriQuery, "rd=25"),
                            QCoapOption(QCoapOption::UriQuery, "nd=4") });

    QTest::newRow("host_path_query")
                        << QUrl("coap://aa.bb.cc.com:5683/test/path1/?rd=25&nd=4")
                        << QUrl()
                        << QVector<QCoapOption>({
                            QCoapOption(QCoapOption::UriHost, "aa.bb.cc.com"),
                            QCoapOption(QCoapOption::UriPath, "test"),
                            QCoapOption(QCoapOption::UriPath, "path1"),
                            QCoapOption(QCoapOption::UriQuery, "rd=25"),
                            QCoapOption(QCoapOption::UriQuery, "nd=4") });

    QTest::newRow("proxy_url")
                        << QUrl("coap://aa.bb.cc.com:5683/test/path1/?rd=25&nd=4")
                        << QUrl("coap://10.20.30.40/test:5684/othertest/path")
                        << QVector<QCoapOption>({
                            QCoapOption(QCoapOption::ProxyUri, "coap://10.20.30.40/test:5684/othertest/path") });
}

void tst_QCoapInternalRequest::parseUri()
{
    QFETCH(QUrl, uri);
    QFETCH(QUrl, proxyUri);
    QFETCH(QVector<QCoapOption>, options);

    QCoapRequest request(uri, QCoapMessage::NonConfirmable, proxyUri);
    QCoapInternalRequest internalRequest(request);

    for (QCoapOption opt : options)
        QVERIFY2(internalRequest.message()->options().contains(opt), "Missing option");

    QCOMPARE(options.count(), internalRequest.message()->optionCount());
}

void tst_QCoapInternalRequest::urlOptions_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<QVector<QCoapOption>>("options");

    QVector<QCoapOption> options = {
        { QCoapOption::UriHost, "example.com" },
        { QCoapOption::UriPath, "~sensors" },
        { QCoapOption::UriPath, "temp.xml" }
    };

    QTest::newRow("url_with_default_port")
            << "coap://example.com:5683/~sensors/temp.xml"
            << options;

    QTest::newRow("url_percent_encoding_uppercase")
            << "coap://EXAMPLE.com/%7Esensors/temp.xml"
            << options;

    QTest::newRow("url_with_no_port_uppercase")
            << "coap://EXAMPLE.com:/%7esensors/temp.xml"
            << options;

    QTest::newRow("url_with_dot_segments")
            << "coap://exaMPLE.com/%7esensors/../%7esensors//./temp.xml"
            << options;

    //! TODO Add more test URLs
}

void tst_QCoapInternalRequest::urlOptions()
{
    QFETCH(QString, url);
    QFETCH(QVector<QCoapOption>, options);

    const QCoapRequest request(url);
    const QCoapInternalRequest internalRequest(request);

    auto requestOptions = internalRequest.message()->options();
    for (const auto& option : options)
        QVERIFY2(requestOptions.removeAll(option) > 0, "Missing option");

    QVERIFY2(requestOptions.isEmpty(), "Fewer options were expected");
}

void tst_QCoapInternalRequest::invalidUrls_data()
{
    QTest::addColumn<QString>("url");

    QTest::newRow("url_with_non_ascii")
            << QString("coap://example.com:5683/~sensors/%1temp.xml").arg(QChar(0x00A3));

    QTest::newRow("url_no_scheme")
            << "example.com:5683/~sensors/temp.xml";

    QTest::newRow("url_wrong_scheme")
            << "http://example.com:5683/~sensors/temp.xml";

    //! TODO Add more test URLs
}

void tst_QCoapInternalRequest::invalidUrls()
{
    QFETCH(QString, url);
    const QCoapRequest request(url);
    const QCoapInternalRequest internalRequest(request);

    QVERIFY(!internalRequest.isValid());
    QVERIFY(internalRequest.message()->options().empty());
}

#else

class tst_QCoapInternalRequest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSKIP("Not an internal build, nothing to test");
    }
};

#endif

QTEST_APPLESS_MAIN(tst_QCoapInternalRequest)

#include "tst_qcoapinternalrequest.moc"
