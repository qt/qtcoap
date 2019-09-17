# Generated from qcoapresource.pro.

#####################################################################
## qcoapresource Test:
#####################################################################

add_qt_test(qcoapresource
    GUI
    SOURCES
        tst_qcoapresource.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
