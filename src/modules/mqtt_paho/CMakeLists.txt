#############################################################################
# Copyright (c) 2013, 2014 ACIN
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
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
	forte_add_sourcefile_hcpp( MQTTComLayer MQTTParameterParser MQTTHandler )
	
	SET(FORTE_COM_PAHOMQTT_INCLUDE_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT src directory")
    SET(FORTE_COM_PAHOMQTT_LIB_DIR "" CACHE PATH "ABSOLUTE path to Paho MQTT library")
	
    if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
	SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a.so" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte")
    elseif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
       SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a.dll" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte. Static library is not working for windows")
    elseif("${FORTE_ARCHITECTURE}" STREQUAL "VxWorks")
       SET(FORTE_COM_PAHOMQTT_LIB "libpaho-mqtt3a-static.a" CACHE STRING "MQTT Library file in FORTE_COM_PAHOMQTT_LIB_DIR to be linked to forte")
    endif()
    forte_add_include_directories(${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
    forte_add_link_directories(${FORTE_COM_PAHOMQTT_LIB_DIR})
    forte_add_link_library(${FORTE_COM_PAHOMQTT_LIB})
	
	if(NOT EXISTS ${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
		message(STATUS "WARNING: The Paho MQTT directory for source given does not exist. This will probably make the compilation fail.")
    endif(NOT EXISTS ${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
    if(NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB_DIR})
        message(STATUS "WARNING: The Paho MQTT directory for library FORTE_COM_PAHOMQTT_LIB given does not exist. This will probably make the linking fail.")
    endif(NOT EXISTS ${FORTE_COM_PAHOMQTT_LIB_DIR})
    
endif(FORTE_COM_PAHOMQTT)
