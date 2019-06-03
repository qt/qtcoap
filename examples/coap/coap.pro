TEMPLATE = subdirs

SUBDIRS += \
    simplecoapclient \
    consolecoapclient

qtHaveModule(quick) {
    SUBDIRS += \
        quicksecureclient \
        quickmulticastclient
}
