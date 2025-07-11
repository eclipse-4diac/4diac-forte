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
forte_register_com_layer(MODBUS OFF "modbus" CModbusComLayer modbuslayer.h
                         "Enable Modbus Com Layer"
)

if (NOT FORTE_COM_MODBUS)
    return()
endif ()

set(FORTE_COM_MODBUS_LIB_DIR
    ""
    CACHE PATH
          "Path to Modbus library directory (leave empty for installed source code)"
)

forte_register_handler(CModbusHandler modbushandler.h)
target_link_libraries(forte PUBLIC modbus)
add_library(forte-com-modbus OBJECT
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
target_link_libraries(forte PRIVATE forte-com-modbus)
target_include_directories(forte-com-modbus PUBLIC /usr/include/modbus)
target_include_directories(forte-com-modbus PUBLIC /usr/local/include/modbus)
target_include_directories(forte-com-modbus
                           PUBLIC ${FORTE_COM_MODBUS_LIB_DIR}/include/modbus
)
target_link_directories(forte-com-modbus PUBLIC
                        ${FORTE_COM_MODBUS_LIB_DIR}/lib
)
