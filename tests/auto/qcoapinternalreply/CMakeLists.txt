# Generated from qcoapinternalreply.pro.

#####################################################################
## qcoapinternalreply Test:
#####################################################################

add_qt_test(qcoapinternalreply
    GUI
    SOURCES
        tst_qcoapinternalreply.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
