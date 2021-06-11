# Generated from qtcoap.pro.

cmake_minimum_required(VERSION 3.15.0)

include(.cmake.conf)
project(QtCoap
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt Coap Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX # special case
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core Network)
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Gui Widgets Quick Qml)

qt_build_repo()
