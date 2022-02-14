cmake_minimum_required(VERSION 3.16)
project(quicksecureclient LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/coap/quicksecureclient")

find_package(Qt6 REQUIRED COMPONENTS Coap Core Gui Qml Quick)

qt_add_executable(quicksecureclient
    main.cpp
    qmlcoapsecureclient.cpp qmlcoapsecureclient.h
)

set_target_properties(quicksecureclient PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
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

qt6_add_resources(quicksecureclient "qml"
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
