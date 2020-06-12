# Generated from coap.pro.

add_subdirectory(consolecoapclient)
if(TARGET Qt::Widgets)
    add_subdirectory(simplecoapclient)
endif()
if(TARGET Qt::Quick)
    add_subdirectory(quicksecureclient)
    add_subdirectory(quickmulticastclient)
endif()
