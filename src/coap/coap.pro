TARGET = QtCoap

QT = core-private
QT_FOR_PRIVATE = network

QMAKE_DOCS = $$PWD/doc/qtcoap.qdocconf

PUBLIC_HEADERS += \
    qcoapclient.h \
    qcoapdiscoveryreply.h \
    qcoapglobal.h \
    qcoapmessage.h \
    qcoapnamespace.h \
    qcoapoption.h \
    qcoapreply.h \
    qcoaprequest.h \
    qcoapresource.h \
    qcoapsecurityconfiguration.h

PRIVATE_HEADERS += \
    qcoapclient_p.h \
    qcoapconnection_p.h \
    qcoapdiscoveryreply_p.h \
    qcoapinternalmessage_p.h \
    qcoapinternalreply_p.h \
    qcoapinternalrequest_p.h \
    qcoapmessage_p.h \
    qcoapoption_p.h \
    qcoapprotocol_p.h \
    qcoapqudpconnection_p.h \
    qcoapreply_p.h \
    qcoaprequest_p.h \
    qcoapresource_p.h

SOURCES += \
    qcoapclient.cpp \
    qcoapconnection.cpp \
    qcoapdiscoveryreply.cpp \
    qcoapinternalmessage.cpp \
    qcoapinternalreply.cpp \
    qcoapinternalrequest.cpp \
    qcoapmessage.cpp \
    qcoapnamespace.cpp \
    qcoapoption.cpp \
    qcoapprotocol.cpp \
    qcoapqudpconnection.cpp \
    qcoapreply.cpp \
    qcoaprequest.cpp \
    qcoapresource.cpp \
    qcoapsecurityconfiguration.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
