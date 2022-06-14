// Copyright (C) 2017 Witekio.
// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "coaphandler.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QUrl>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt CoAP test application");
    parser.addHelpOption();
    parser.addPositionalArgument("url",
                                 "The targeted coap resource URL, e.g. 'coap://10.10.10.10/myresource'");
    parser.addOption({"get", "Use GET method for the request. This is the default method"});
    parser.addOption({"post", "Use POST method for the request"});
    parser.addOption({"put", "Use PUT method for the request"});
    parser.addOption({"delete", "Use DELETE method for the request"});
    parser.addOption({"observe", "Observe a resource"});
    parser.addOption({"discover", "Discover available resources"});

    parser.process(a);

    if (parser.positionalArguments().isEmpty()) {
        qWarning("Please provide an url for the request");
        parser.showHelp(1);
    }

    QUrl url = parser.positionalArguments().first();
    if (!url.isValid()) {
        qWarning("The url provided is not valid.");
        parser.showHelp(1);
    }

    CoapHandler handler;
    if (parser.isSet("get"))        handler.runGet(url);
    if (parser.isSet("post"))       handler.runPost(url);
    if (parser.isSet("put"))        handler.runPut(url);
    if (parser.isSet("delete"))     handler.runDelete(url);
    if (parser.isSet("observe"))    handler.runObserve(url);
    if (parser.isSet("discover"))   handler.runDiscover(url);

    return a.exec();
}
