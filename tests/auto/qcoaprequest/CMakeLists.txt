# Generated from qcoaprequest.pro.

#####################################################################
## qcoaprequest Test:
#####################################################################

add_qt_test(qcoaprequest
    GUI
    SOURCES
        tst_qcoaprequest.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
