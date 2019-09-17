# Generated from coap.pro.

#####################################################################
## Coap Module:
#####################################################################

add_qt_module(Coap
    SOURCES
        qcoapclient.cpp qcoapclient.h qcoapclient_p.h
        qcoapconnection.cpp qcoapconnection_p.h
        qcoapglobal.h
        qcoapinternalmessage.cpp qcoapinternalmessage_p.h
        qcoapinternalreply.cpp qcoapinternalreply_p.h
        qcoapinternalrequest.cpp qcoapinternalrequest_p.h
        qcoapmessage.cpp qcoapmessage.h qcoapmessage_p.h
        qcoapnamespace.cpp qcoapnamespace.h qcoapnamespace_p.h
        qcoapoption.cpp qcoapoption.h qcoapoption_p.h
        qcoapprotocol.cpp qcoapprotocol_p.h
        qcoapqudpconnection.cpp qcoapqudpconnection_p.h
        qcoapreply.cpp qcoapreply.h qcoapreply_p.h
        qcoaprequest.cpp qcoaprequest.h qcoaprequest_p.h
        qcoapresource.cpp qcoapresource.h qcoapresource_p.h
        qcoapresourcediscoveryreply.cpp qcoapresourcediscoveryreply.h qcoapresourcediscoveryreply_p.h
        qcoapsecurityconfiguration.cpp qcoapsecurityconfiguration.h
    LIBRARIES
        Qt::CorePrivate
        Qt::Network
    PUBLIC_LIBRARIES
        Qt::Core
)

#### Keys ignored in scope 1:.:.:coap.pro:<TRUE>:
# _LOADED = "qt_module"
add_qt_docs(
    doc/qtcoap.qdocconf
)

