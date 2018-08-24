COAP_TEST_SERVER_IP = $$(COAP_TEST_SERVER_IP)
isEmpty( COAP_TEST_SERVER_IP ) {
    error(No IP set for CoAP plugtest server. Please set COAP_TEST_SERVER_IP environment variable to run this test.)
}

DEFINES += COAP_TEST_SERVER_IP=\\\"$${COAP_TEST_SERVER_IP}\\\"
message(CoAP plugtest server IP set to $$COAP_TEST_SERVER_IP)
