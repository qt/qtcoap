// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef QMLCOAPMULTICASTCLIENT_H
#define QMLCOAPMULTICASTCLIENT_H

#include <QtCoap/qcoapnamespace.h>
#include <QCoapClient>
#include <QCoapResource>
#include <QCoapResourceDiscoveryReply>

#include <QtQml/qqmlregistration.h>

class QmlCoapResource : public QCoapResource
{
    Q_GADGET
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString host READ hostStr)
    Q_PROPERTY(QString path READ path)

    QML_ANONYMOUS
public:
    QmlCoapResource() : QCoapResource() {}
    QmlCoapResource(const QCoapResource &resource)
        : QCoapResource(resource) {}

    QString hostStr() const { return host().toString(); }
};
Q_DECLARE_METATYPE(QmlCoapResource)

class QmlCoapMulticastClient : public QCoapClient
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CoapMulticastClient)
public:
    QmlCoapMulticastClient(QObject *parent = nullptr);

    Q_INVOKABLE void discover(const QString &host, int port, const QString &discoveryPath);
    Q_INVOKABLE void discover(QtCoap::MulticastGroup group, int port, const QString &discoveryPath);

Q_SIGNALS:
    void discovered(const QmlCoapResource &resource);
    void finished(int error);

public slots:
    void onDiscovered(QCoapResourceDiscoveryReply *reply, const QList<QCoapResource> &resources);
};

namespace QCoapForeignNamespace
{
    Q_NAMESPACE
    QML_FOREIGN_NAMESPACE(QtCoap)
    QML_NAMED_ELEMENT(QtCoap)
}

#endif // QMLCOAPMULTICASTCLIENT_H
