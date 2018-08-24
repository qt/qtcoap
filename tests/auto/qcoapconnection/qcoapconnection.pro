QT = testlib network core-private core coap coap-private
CONFIG += testcase

include(../coaptestserver.pri)

HEADERS += ../coapnetworksettings.h

SOURCES += tst_qcoapconnection.cpp
