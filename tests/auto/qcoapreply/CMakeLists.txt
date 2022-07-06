# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

# Generated from qcoapreply.pro.

#####################################################################
## qcoapreply Test:
#####################################################################

qt_internal_add_test(qcoapreply
    SOURCES
        tst_qcoapreply.cpp
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::CoapPrivate
        Qt::Network
)
