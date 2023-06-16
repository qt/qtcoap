TEMPLATE = app

QT += qml quick coap

HEADERS += \
    qmlcoapsecureclient.h

SOURCES += \
    main.cpp \
    qmlcoapsecureclient.cpp

DISTFILES += \
    FilePicker.qml

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/coap/quicksecureclient
INSTALLS += target
