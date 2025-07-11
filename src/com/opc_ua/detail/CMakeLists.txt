if (NOT FORTE_COM_OPC_UA_MULTICAST)
    return()
endif ()
add_library(forte-com-opc_ua-detail OBJECT lds_me_handler.h
                                           lds_me_handler.cpp
)
target_link_libraries(forte PRIVATE forte-com-opc_ua-detail)
