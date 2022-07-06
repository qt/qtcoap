# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

# Generated from qcoapresource.pro.

#####################################################################
## qcoapresource Test:
#####################################################################

qt_internal_add_test(qcoapresource
    SOURCES
        tst_qcoapresource.cpp
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::CoapPrivate
        Qt::Network
)
