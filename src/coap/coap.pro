TARGET = QtCoap

QT = core-private
QT_FOR_PRIVATE = network

PUBLIC_HEADERS += \
    qcoapclient.h \
    qcoapconnection.h \
    qcoapmessage.h \
    qcoapoption.h \
    qcoapreply.h \
    qcoaprequest.h \
    qcoapresource.h \
    qcoapprotocol.h \
    qcoapinternalmessage.h \
    qcoapglobal.h \
    qcoapdiscoveryreply.h \
    qcoapnamespace.h

PRIVATE_HEADERS += \
    qcoapmessage_p.h \
    qcoapreply_p.h \
    qcoaprequest_p.h \
    qcoapoption_p.h \
    qcoapconnection_p.h \
    qcoapclient_p.h \
    qcoapresource_p.h \
    qcoapprotocol_p.h \
    qcoapinternalmessage_p.h \
    qcoapinternalrequest_p.h \
    qcoapinternalreply_p.h \
    qcoapdiscoveryreply_p.h

SOURCES += \
    qcoapclient.cpp \
    qcoapconnection.cpp \
    qcoapmessage.cpp \
    qcoapoption.cpp \
    qcoapreply.cpp \
    qcoaprequest.cpp \
    qcoapresource.cpp \
    qcoapprotocol.cpp \
    qcoapinternalmessage.cpp \
    qcoapinternalreply.cpp \
    qcoapinternalrequest.cpp \
    qcoapdiscoveryreply.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

load(qt_module)
