# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

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
