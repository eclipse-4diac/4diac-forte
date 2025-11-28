# ############################################################################
# Copyright (c) 2013, 2014 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Melik Merkumians - initial API and implementation
# and/or initial documentation
# ############################################################################
# ############################################################################
# PAHO MQTT Com Layer
# ############################################################################
option(FORTE_COM_PAHOMQTT "Enable Paho MQTT Com Layer" OFF)
if (NOT FORTE_COM_PAHOMQTT)
    return()
endif ()

set(FORTE_COM_PAHOMQTT_INCLUDE_DIR
        ""
        CACHE PATH "ABSOLUTE path to Paho MQTT src directory"
)
set(FORTE_COM_PAHOMQTT_LIB_DIR
        ""
        CACHE PATH "ABSOLUTE path to Paho MQTT library"
)
set(FORTE_COM_PAHOMQTT_LIB
        "libpaho-mqtt3a"
        CACHE STRING "MQTT Library name to be linked to forte"
)

add_library(forte-com-mqtt_paho
        $<$<BOOL:${FORTE_COM_PAHOMQTT}>:MQTTComLayer.cpp>
        $<$<BOOL:${FORTE_COM_PAHOMQTT}>:MQTTComLayer.h>
        MQTTComLayer.h
        MQTTComLayer.cpp
        MQTTHandler.h
        MQTTHandler.cpp
        MQTTClient.h
        MQTTClient.cpp
        MQTTClientConfigParser.h
        MQTTClientConfigParser.cpp
)
target_link_libraries(forte-com-mqtt_paho PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-mqtt_paho,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-mqtt_paho>>)

target_include_directories(forte-com-mqtt_paho PRIVATE ${FORTE_COM_PAHOMQTT_INCLUDE_DIR})
target_link_directories(forte-com-mqtt_paho PUBLIC ${FORTE_COM_PAHOMQTT_LIB_DIR})
if (WIN32)
    target_link_libraries(forte-com-mqtt_paho PRIVATE crypt32)
endif ()
target_link_libraries(forte-com-mqtt_paho PRIVATE ${FORTE_COM_PAHOMQTT_LIB})

install(TARGETS forte-com-mqtt_paho EXPORT forte-export FILE_SET HEADERS)
