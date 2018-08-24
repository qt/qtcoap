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
#include <QtCore/qstring.h>
#include <QtNetwork/qhostinfo.h>

/*!
    \internal

    This namespace provides URL and settings used in QtCoap tests.

    Tests require a Californium plugtest server, accessible with
    "coap-plugtest-server" host name. You create such server with Docker and
    the following command line:
    \code
        docker run -d --rm -p 5683:5683/udp aleravat/coap-test-server:latest
    \endcode

    For more details, see
    \l{https://github.com/Pixep/coap-testserver-docker}{https://github.com/Pixep/coap-testserver-docker}.
*/
namespace QtCoapNetworkSettings
{
    QString testServerHost()
    {
#if defined(COAP_TEST_SERVER_IP)
        return QStringLiteral(COAP_TEST_SERVER_IP);
#else
        static_assert(false, "COAP_TEST_SERVER_IP variable must be set");
#endif
    }

    QString testServerUrl()
    {
        return QStringLiteral("coap://") + testServerHost() + QStringLiteral(":")
                + QString::number(QtCoap::DefaultPort);
    }

    QString testServerResource()
    {
        return testServerUrl() + QStringLiteral("/test");
    }
}
