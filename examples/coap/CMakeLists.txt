# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_example(consolecoapclient)
if(TARGET Qt::Widgets)
    qt_internal_add_example(simplecoapclient)
endif()
if(TARGET Qt::Quick)
    qt_internal_add_example(quicksecureclient)
    qt_internal_add_example(quickmulticastclient)
endif()
