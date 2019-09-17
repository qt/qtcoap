# Generated from qcoapmessage.pro.

#####################################################################
## qcoapmessage Test:
#####################################################################

add_qt_test(qcoapmessage
    GUI
    SOURCES
        tst_qcoapmessage.cpp
    LIBRARIES
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)
