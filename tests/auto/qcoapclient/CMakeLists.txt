# Generated from qcoapclient.pro.

#####################################################################
## qcoapclient Test:
#####################################################################

add_qt_test(qcoapclient
    EXCEPTIONS # special case
    GUI
    SOURCES
        ../coapnetworksettings.h
        tst_qcoapclient.cpp
    LIBRARIES
        Qt::CoapPrivate
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::Network
)

#### Keys ignored in scope 1:.:.:qcoapclient.pro:<TRUE>:
# QT_TEST_SERVER_LIST = "californium" "freecoap"

## Scopes:
#####################################################################

# special case begin
extend_target(qcoapclient CONDITION DEFINED ENV{COAP_TEST_SERVER_IP}
    DEFINES
        COAP_TEST_SERVER_IP=\\\"$ENV{COAP_TEST_SERVER_IP}\\\"
)
# special case end
