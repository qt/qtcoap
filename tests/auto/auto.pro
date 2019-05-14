TEMPLATE = subdirs

# TODO: enable disabled tests, when CI is configured properly

SUBDIRS += \
    cmake \
#    qcoapclient \
    qcoapmessage \
    qcoapoption \
    qcoaprequest \
    qcoapresource

qtConfig(private_tests): SUBDIRS += \
#    qcoapqudpconnection \
    qcoapinternalrequest \
    qcoapinternalreply \
    qcoapreply
