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

SET(FORTE_COM_PAHOMQTT_DIR "" CACHE PATH "Path to Paho MQTT src directory (leave empty for installed source code)")

if(FORTE_COM_PAHOMQTT)
	forte_add_sourcefile_hcpp( MQTTComLayer MQTTParameterParser MQTTHandler )
	if(EXISTS ${FORTE_COM_PAHOMQTT_DIR})
	    if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
			message("including mqtt")
    		forte_add_include_directories( ${FORTE_COM_PAHOMQTT_DIR}/src )
    		forte_add_link_directories( ${FORTE_COM_PAHOMQTT_DIR}/build/src)
	       	forte_add_link_library( paho-mqtt3a.a.dll.a paho-mqtt3c.a.dll.a paho-mqtt3as.so  paho-mqtt3cs.so)
	    elseif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
		    #forte_add_include_directories( ${FORTE_COM_PAHOMQTT_DIR}/include )
		    #forte_add_link_directories( ${FORTE_COM_PAHOMQTT_DIR}/lib )
		elseif("${FORTE_ARCHITECTURE}" STREQUAL "VxWorks")
		    message("including mqtt")
            forte_add_include_directories( ${FORTE_COM_PAHOMQTT_DIR}/src )
            forte_add_link_directories( ${FORTE_COM_PAHOMQTT_DIR}/build/src)
            forte_add_link_library( paho-mqtt3a.a.dll.a paho-mqtt3c.a.dll.a)
	    endif()
	else(EXISTS ${FORTE_COM_PAHOMQTT_DIR})
		message("No Paho MQTT directory given")
    endif(EXISTS ${FORTE_COM_PAHOMQTT_DIR})
endif(FORTE_COM_PAHOMQTT)
