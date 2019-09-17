# Generated from qcoapoption.pro.

#####################################################################
## qcoapoption Test:
#####################################################################

add_qt_test(qcoapoption
    GUI
    SOURCES
        tst_qcoapoption.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
)
