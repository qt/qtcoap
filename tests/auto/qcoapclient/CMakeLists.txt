# Generated from qcoapclient.pro.

#####################################################################
## qcoapclient Test:
#####################################################################

qt_add_test(qcoapclient
    EXCEPTIONS # special case
    GUI
    SOURCES
        ../coapnetworksettings.h
        tst_qcoapclient.cpp
    PUBLIC_LIBRARIES
        Qt::Coap
        Qt::CoapPrivate
        Qt::Network
)

#### Keys ignored in scope 1:.:.:qcoapclient.pro:<TRUE>:
# QT_TEST_SERVER_LIST = "californium" "freecoap"

## Scopes:
#####################################################################

# special case begin
qt_extend_target(qcoapclient CONDITION DEFINED ENV{COAP_TEST_SERVER_IP}
    DEFINES
        COAP_TEST_SERVER_IP=\\\"$ENV{COAP_TEST_SERVER_IP}\\\"
)
# special case end