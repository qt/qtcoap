# Generated from qcoapqudpconnection.pro.

#####################################################################
## qcoapqudpconnection Test:
#####################################################################

add_qt_test(qcoapqudpconnection
    GUI
    SOURCES
        ../coapnetworksettings.h
        tst_qcoapqudpconnection.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)

#### Keys ignored in scope 1:.:.:qcoapqudpconnection.pro:<TRUE>:
# QT_TEST_SERVER_LIST = "californium"

## Scopes:
#####################################################################

# special case begin
extend_target(qcoapqudpconnection CONDITION DEFINED ENV{COAP_TEST_SERVER_IP}
    DEFINES
        COAP_TEST_SERVER_IP=\\\"$ENV{COAP_TEST_SERVER_IP}\\\"
)
# special case end
