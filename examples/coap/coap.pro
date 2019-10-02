TEMPLATE = subdirs

SUBDIRS += consolecoapclient

qtHaveModule(gui) SUBDIRS += simplecoapclient

qtHaveModule(quick) {
    SUBDIRS += \
        quicksecureclient \
        quickmulticastclient
}
