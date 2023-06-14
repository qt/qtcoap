TEMPLATE = app

QT += qml quick coap

SOURCES += \
        main.cpp \
        qmlcoapmulticastclient.cpp

HEADERS += \
    qmlcoapmulticastclient.h

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/coap/quickmulticastclient
INSTALLS += target
