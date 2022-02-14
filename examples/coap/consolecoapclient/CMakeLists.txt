cmake_minimum_required(VERSION 3.16)
project(consolecoapclient LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/coap/consolecoapclient")

find_package(Qt6 REQUIRED COMPONENTS Coap Core Network)

qt_add_executable(consolecoapclient
    coaphandler.cpp coaphandler.h
    main.cpp
)

set_target_properties(consolecoapclient PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(consolecoapclient PUBLIC
    Qt::Coap
    Qt::Core
    Qt::Network
)

install(TARGETS consolecoapclient
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
