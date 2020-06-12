# Generated from qcoapinternalrequest.pro.

#####################################################################
## qcoapinternalrequest Test:
#####################################################################

qt_add_test(qcoapinternalrequest
    SOURCES
        tst_qcoapinternalrequest.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
