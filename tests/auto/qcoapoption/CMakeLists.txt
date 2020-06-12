# Generated from qcoapoption.pro.

#####################################################################
## qcoapoption Test:
#####################################################################

qt_add_test(qcoapoption
    SOURCES
        tst_qcoapoption.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
)
