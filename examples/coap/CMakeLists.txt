# Generated from coap.pro.

add_subdirectory(simplecoapclient)
add_subdirectory(consolecoapclient)
if(TARGET Qt::Quick)
    add_subdirectory(quicksecureclient)
    add_subdirectory(quickmulticastclient)
endif()
