# Generated from qcoapinternalreply.pro.

#####################################################################
## qcoapinternalreply Test:
#####################################################################

qt_add_test(qcoapinternalreply
    SOURCES
        tst_qcoapinternalreply.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
