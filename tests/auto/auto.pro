TEMPLATE = subdirs

SUBDIRS += \
    cmake \
    qcoapclient \
    qcoapmessage \
    qcoapoption \
    qcoaprequest \
    qcoapresource

qtConfig(private_tests): SUBDIRS += \
    qcoapqudpconnection \
    qcoapinternalrequest \
    qcoapinternalreply \
    qcoapreply
