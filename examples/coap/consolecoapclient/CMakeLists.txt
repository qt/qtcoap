# Generated from consolecoapclient.pro.

cmake_minimum_required(VERSION 3.14)
project(testapp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_EXAMPLEDIR "examples")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Network)
find_package(Qt6 COMPONENTS Coap)

add_qt_gui_executable(testapp
    coaphandler.cpp coaphandler.h
    main.cpp
)
target_link_libraries(testapp PUBLIC
    Qt::Coap
    Qt::Core
    Qt::Network
)

install(TARGETS testapp
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
