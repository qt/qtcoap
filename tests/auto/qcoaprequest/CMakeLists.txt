# Generated from qcoaprequest.pro.

#####################################################################
## qcoaprequest Test:
#####################################################################

qt_add_test(qcoaprequest
    SOURCES
        tst_qcoaprequest.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
