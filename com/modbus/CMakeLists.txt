# *******************************************************************************
# Copyright (c) 2012 -2014 AIT, fortiss GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Filip Andren, Alois Zoitl - initial API and implementation
# and/or initial documentation
# *******************************************************************************/
# ############################################################################
# Modbus Com Layer
# ############################################################################
option(FORTE_COM_MODBUS "Enable Modbus Com Layer" OFF)
if (NOT FORTE_COM_MODBUS)
    return()
endif ()

set(FORTE_COM_MODBUS_LIB_DIR
        ""
        CACHE PATH
        "Path to Modbus library directory (leave empty for installed source code)"
)

add_library(forte-com-modbus
        $<$<BOOL:${FORTE_COM_MODBUS}>:modbuslayer.cpp>
        $<$<BOOL:${FORTE_COM_MODBUS}>:modbuslayer.h>
        modbushandler.h
        modbushandler.cpp
        modbusconnection.h
        modbusconnection.cpp
        modbusclientconnection.h
        modbusclientconnection.cpp
        modbuspoll.h
        modbuspoll.cpp
        modbusioblock.h
        modbusioblock.cpp
        modbustimedevent.h
        modbustimedevent.cpp
)
target_link_libraries(forte-com-modbus PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-modbus,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-modbus>>)

if (FORTE_COM_MODBUS_LIB_DIR)
    target_include_directories(forte-com-modbus PRIVATE ${FORTE_COM_MODBUS_LIB_DIR}/include)
    target_link_directories(forte-com-modbus PUBLIC ${FORTE_COM_MODBUS_LIB_DIR}/lib)
endif ()
target_link_libraries(forte-com-modbus PRIVATE modbus)

install(TARGETS forte-com-modbus EXPORT forte-export FILE_SET HEADERS)
