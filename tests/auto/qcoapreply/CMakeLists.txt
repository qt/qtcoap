# Generated from qcoapreply.pro.

#####################################################################
## qcoapreply Test:
#####################################################################

add_qt_test(qcoapreply
    GUI
    SOURCES
        tst_qcoapreply.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
