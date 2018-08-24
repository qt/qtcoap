QT = testlib core-private network core coap coap-private
CONFIG += testcase

include(../coaptestserver.pri)

HEADERS += ../coapnetworksettings.h

SOURCES +=  tst_qcoapclient.cpp
