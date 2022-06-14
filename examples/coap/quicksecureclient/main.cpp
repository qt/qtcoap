// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "qmlcoapsecureclient.h"

#include <QGuiApplication>
#include <QNetworkInterface>
#include <QQmlApplicationEngine>
#include <QQmlContext>

static QStringList availableHosts()
{
    QStringList hosts;

    const auto networkInterfaces = QNetworkInterface::allInterfaces();
    for (const auto &interface : networkInterfaces)
        for (const auto &address : interface.addressEntries())
            hosts.push_back(address.ip().toString());

    return hosts;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Qt CoAP Example");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<QmlCoapSecureClient>("CoapSecureClient", 1, 0, "CoapSecureClient");

    // Register the QtCoap namespace
    qmlRegisterUncreatableMetaObject(QtCoap::staticMetaObject, "qtcoap.example.namespace", 1, 0,
                                     "QtCoap", "Access to enums is read-only");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        // Exit with error, if the object for main.qml could not be loaded.
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("hostsModel", availableHosts());

    engine.load(url);

    return app.exec();
}
