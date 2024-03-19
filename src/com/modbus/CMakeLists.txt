#*******************************************************************************
# Copyright (c) 2012 -2014 AIT, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Modbus Com Layer
#############################################################################
forte_add_network_layer(MODBUS OFF "modbus" CModbusComLayer modbuslayer "Enable Modbus Com Layer")

if(FORTE_COM_MODBUS)
   SET(FORTE_COM_MODBUS_LIB_DIR "" CACHE PATH "Path to Modbus library directory (leave empty for installed source code)")

   forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   forte_add_sourcefile_hcpp( modbushandler 
                 modbusconnection 
                 modbusclientconnection
                 modbuspoll
                 modbusioblock
                 modbustimedevent )
                 
  forte_add_handler(CModbusHandler modbushandler)
  forte_add_include_directories( ${FORTE_COM_MODBUS_LIB_DIR}/include/modbus )
  forte_add_link_directories( ${FORTE_COM_MODBUS_LIB_DIR}/lib )
  if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    if(NOT EXISTS ${FORTE_COM_MODBUS_LIB_DIR})
      forte_add_include_directories( /usr/include/modbus )
      forte_add_include_directories( /usr/local/include/modbus )
    endif()
  endif()
  forte_add_link_library( modbus )

endif(FORTE_COM_MODBUS)
