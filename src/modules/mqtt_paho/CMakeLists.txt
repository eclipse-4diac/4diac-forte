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
  forte_add_sourcefile_hcpp( MQTTComLayer MQTTHandler MQTTClientConfigParser)
  
  forte_add_handler(MQTTHandler MQTTHandler)
  
  SET(FORTE_COM_PAHOMQTT_INCLUDE_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT src directory")
    SET(FORTE_COM_PAHOMQTT_LIB_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT library")
  
    IF("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a.so" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte")
    ELSEIF("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
      SET(FORTE_COM_PAHOMQTT_LIB "paho-mqtt3a.dll" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte. Static library is not working for windows")
      string(FIND ${FORTE_COM_PAHOMQTT_LIB} "static" DETECT)
      IF(NOT "${DETECT}" STREQUAL "-1")
        MESSAGE(WARNING "WARNING: Paho library doesn't support static libraries in Windows")
      ENDIF(NOT "${DETECT}" STREQUAL "-1")
    ELSE()
       SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a-static.a" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte")
    ENDIF()
    forte_add_include_system_directories(${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
    forte_add_link_directories(${FORTE_COM_PAHOMQTT_LIB_DIR})
    forte_add_link_library_beginning(${FORTE_COM_PAHOMQTT_LIB})
    forte_add_custom_configuration("#include <string>")
    forte_add_custom_configuration("extern std::string gMqttClientConfigFile\;")
    forte_add_custom_configuration("#cmakedefine FORTE_COM_PAHOMQTT")
  
    IF(NOT EXISTS ${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
      MESSAGE(WARNING "WARNING: FORTE_COM_PAHOMQTT_INCLUDE_DIR not set or does not exist. This will probably make the compilation fail.")
    ENDIF(NOT EXISTS ${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
    
    file(TO_CMAKE_PATH "${FORTE_COM_PAHOMQTT_LIB_DIR}/${FORTE_COM_PAHOMQTT_LIB}" FORTE_COM_PAHOMQTT_LIB_PATH)
    IF(NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB_DIR}/${FORTE_COM_PAHOMQTT_LIB} AND NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB})
      MESSAGE(WARNING "WARNING: PAHO MQTT library not found '${FORTE_COM_PAHOMQTT_LIB}'. Check the value of FORTE_COM_PAHOMQTT_LIB_DIR or define an absolute path for FORTE_COM_PAHOMQTT_LIB.")
    ENDIF(NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB_DIR}/${FORTE_COM_PAHOMQTT_LIB} AND NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB})
    
endif(FORTE_COM_PAHOMQTT)
