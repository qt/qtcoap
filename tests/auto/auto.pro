TEMPLATE = subdirs

SUBDIRS += \
    cmake \
#    TODO: enable the tests below, when CI is configured properly
#    qcoapclient \
#    qcoapconnection \
    qcoapinternalreply \
    qcoapinternalrequest \
    qcoapmessage \
    qcoapoption \
    qcoapreply \
    qcoaprequest \
    qcoapresource
