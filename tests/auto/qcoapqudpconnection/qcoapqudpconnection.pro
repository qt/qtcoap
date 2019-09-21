QT = testlib network core coap coap-private
CONFIG += testcase

include(../coaptestserver.pri)

HEADERS += ../coapnetworksettings.h

SOURCES += \
    tst_qcoapqudpconnection.cpp

CONFIG += unsupported/testserver
QT_TEST_SERVER_LIST = californium
