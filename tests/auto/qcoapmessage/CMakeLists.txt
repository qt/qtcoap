# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

# Generated from qcoapmessage.pro.

#####################################################################
## qcoapmessage Test:
#####################################################################

qt_internal_add_test(qcoapmessage
    SOURCES
        tst_qcoapmessage.cpp
    LIBRARIES
        Qt::Coap
        Qt::Network
)
