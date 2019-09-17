# Generated from quicksecureclient.pro.

cmake_minimum_required(VERSION 3.14)
project(quicksecureclient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS Coap)

add_qt_gui_executable(quicksecureclient
    main.cpp
    qmlcoapsecureclient.cpp qmlcoapsecureclient.h
)
target_compile_definitions(quicksecureclient PUBLIC
    QT_DEPRECATED_WARNINGS
)
target_link_libraries(quicksecureclient PUBLIC
    Qt::Coap
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

# Resources:
set(qml_resource_files
    "FilePicker.qml"
    "main.qml"
)

QT6_ADD_RESOURCES(quicksecureclient "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)


install(TARGETS quicksecureclient
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
