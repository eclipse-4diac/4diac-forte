if (NOT FORTE_COM_OPC_UA_MULTICAST)
    return()
endif ()
target_sources(forte-com-opc_ua PRIVATE
        lds_me_handler.h
        lds_me_handler.cpp
)
