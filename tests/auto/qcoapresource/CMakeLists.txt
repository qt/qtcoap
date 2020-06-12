# Generated from qcoapresource.pro.

#####################################################################
## qcoapresource Test:
#####################################################################

qt_add_test(qcoapresource
    SOURCES
        tst_qcoapresource.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
