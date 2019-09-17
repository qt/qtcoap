# Generated from tests.pro.

if(NOT TARGET Qt::Test)
    cmake_minimum_required(VERSION 3.15.0)
    project(QtCoapTests VERSION 6.0.0 LANGUAGES C CXX)
    # special case begin
    find_package(Qt6 ${PROJECT_VERSION} REQUIRED COMPONENTS BuildInternals Core Network Test Coap)
    # find_package(Qt6 ${PROJECT_VERSION} OPTIONAL_COMPONENTS SET_ME_TO_SOMETHING_USEFUL)
    # special case end
    qt_set_up_standalone_tests_build()
endif()

qt_build_tests()
