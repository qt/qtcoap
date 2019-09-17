# Generated from qcoapinternalrequest.pro.

#####################################################################
## qcoapinternalrequest Test:
#####################################################################

add_qt_test(qcoapinternalrequest
    GUI
    SOURCES
        tst_qcoapinternalrequest.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
