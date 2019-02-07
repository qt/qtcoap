QT = testlib core-private network core coap coap-private
CONFIG += testcase

include(../coaptestserver.pri)

HEADERS += ../coapnetworksettings.h

SOURCES +=  tst_qcoapclient.cpp

CONFIG += unsupported/testserver
QT_TEST_SERVER_LIST = californium
