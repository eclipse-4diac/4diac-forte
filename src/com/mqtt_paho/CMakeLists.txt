#############################################################################
# Copyright (c) 2013, 2014 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
# Martin Melik Merkumians - initial API and implementation and/or initial documentation
#############################################################################


#############################################################################
# PAHO MQTT Com Layer
#############################################################################
forte_add_network_layer(PAHOMQTT OFF "mqtt" MQTTComLayer MQTTComLayer "Enable Paho MQTT Com Layer")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(FORTE_COM_PAHOMQTT)
  forte_add_sourcefile_hcpp( MQTTComLayer MQTTHandler MQTTClient MQTTClientConfigParser)
  
  forte_add_handler(MQTTHandler MQTTHandler)
  
  SET(FORTE_COM_PAHOMQTT_INCLUDE_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT src directory")
    SET(FORTE_COM_PAHOMQTT_LIB_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT library")
    SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a" CACHE STRING "MQTT Library name to be linked to forte")
  
    if(FORTE_ARCHITECTURE STREQUAL "Win32")
        forte_add_link_library(crypt32)
    endif ()
    forte_add_include_system_directories(${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
    forte_add_link_directories(${FORTE_COM_PAHOMQTT_LIB_DIR})
    forte_add_link_library_beginning(${FORTE_COM_PAHOMQTT_LIB})
  
endif()
