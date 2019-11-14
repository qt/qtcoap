# Generated from qcoapreply.pro.

#####################################################################
## qcoapreply Test:
#####################################################################

qt_add_test(qcoapreply
    SOURCES
        tst_qcoapreply.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
