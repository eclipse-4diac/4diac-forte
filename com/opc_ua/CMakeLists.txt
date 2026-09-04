# *******************************************************************************************

# Copyright (c) 2015 Florian Froschermeier
# <florian.froschermeier@tum.de>, fortiss GmbH, HR Agrartechnik GmbH. This
# program and the accompanying materials are made available under the terms
# of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Florian Froschermeier - initial integration of OPC Unified
# Architecture into FORTE Stefan Profanter - refactoring and adaptation to new concept;
# Franz Höpfinger - Add optional OPC UA over WebSocket (insecure and secure
# transports) server endpoint
# ******************************************************************************************

# ############################################################################
# OPC Unified Architecture (UA) integration. For integration the open-source
# OPC-UA communication stack/library from (open62541 - http://open62541.org/)
# was used.
# ############################################################################

# option to enable OPC UA with cmake
option(FORTE_COM_OPC_UA "Enable OPC UA communication layer" OFF)
if (NOT FORTE_COM_OPC_UA)
    return()
endif ()

if (FORTE_COM_OPC_UA_INCLUDE_DIR)
    get_filename_component(FORTE_COM_OPC_UA_INCLUDE_DIR_ABS "${FORTE_COM_OPC_UA_INCLUDE_DIR}" ABSOLUTE)
    if (NOT TARGET open62541::open62541)
        # FORTE_COM_OPC_UA_INCLUDE_DIR points at an amalgamated open62541 build
        # (open62541.c/.h, no compiled library yet): on ESP-IDF targets that
        # source is compiled and linked one level up, as the outer project's
        # own "open62541" component, so forte only ever needs the headers here
        # to type-check against - never a real library to link.
        add_library(open62541 INTERFACE)
        add_library(open62541::open62541 ALIAS open62541)
        # Also used as the INSTALL_INTERFACE (not just BUILD_INTERFACE): these are
        # absolute, build-machine-specific paths, so the exported target is only
        # usable by a consumer building against this same tree -- but that beats an
        # exported target with no include paths at all, which is what plain
        # BUILD_INTERFACE-only paths would leave for install(EXPORT) consumers.
        target_include_directories(open62541 INTERFACE
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/src_generated>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/include>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/plugins/include>
            $<INSTALL_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}>
            $<INSTALL_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/src_generated>
            $<INSTALL_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/include>
            $<INSTALL_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/plugins/include>
        )
        install(TARGETS open62541 EXPORT forte-export)
    endif ()
else ()
    find_package(open62541 1.5.4 REQUIRED)
endif ()

set(FORTE_COM_OPC_UA_PORT
        4840
        CACHE STRING "Port where the OPC_UA server will listen"
)
set(FORTE_COM_OPC_UA_SERVER_PUB_INTERVAL
        100.0
        CACHE STRING "OPC UA Server publishing interval in ms"
)
set(FORTE_COM_OPC_UA_CLIENT_PUB_INTERVAL
        100.0
        CACHE STRING
        "OPC UA Client requested publishing interval via OPC UA data subscription in ms"
)
set(FORTE_COM_OPC_UA_SERVER_MAX_ITERATION_INTERVAL
        10
        CACHE STRING
        "The maximum time in OPC UA server will wait to iterate again"
)

option(FORTE_COM_OPC_UA_MULTICAST
        "Enable multicast support for OPC UA and registering with LDS" OFF
)
# Enables the insecure and secure WebSocket endpoints alongside the plain opc.tcp
# endpoint, using the ports immediately above it (insecure WebSocket on port+1 and
# secure WebSocket on port+2). On platforms built with UA_ENABLE_LWS (libwebsockets,
# POSIX-only), open62541 provides the "websocket" ConnectionManager itself. On
# platforms without libwebsockets (RTOS targets such as FreeRTOS/Zephyr), that
# ConnectionManager has to be registered by the platform's own code instead.
option(FORTE_COM_OPC_UA_WEBSOCKET
        "Enable OPC UA over WebSocket server endpoints." OFF
)

# On platforms without UA_ENABLE_LWS (RTOS targets, see comment above), the
# "websocket" ConnectionManager has to come from the platform's own code.
# FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR points at the directory
# providing that implementation's header (ws_connection_manager.h); on
# ESP-IDF targets its .c is compiled and linked one level up, as the outer
# project's own "open62541" component, so forte only needs the header here
# to type-check against - never a real library to link, same rationale as
# FORTE_COM_OPC_UA_INCLUDE_DIR above.
# Declared here, ahead of the WEBSOCKET_INSECURE/_SECURE validation below,
# so that validation can check it.
set(FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR
        CACHE PATH
        "Directory containing ws_connection_manager.h, providing the \"websocket\" UA_ConnectionManager on platforms without UA_ENABLE_LWS"
)

if (FORTE_COM_OPC_UA_WEBSOCKET)
    # Intentional unencrypted-WebSocket support (builder-configurable, off when
    # INSECURE=OFF); it exists for development/test setups without a TLS
    # certificate, never on by default -- see the option description below.
    option(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
            "Enable unencrypted OPC UA over WebSocket (opc.ws://) endpoint" OFF # nosemgrep: javascript.lang.security.detect-insecure-websocket
    )
    option(FORTE_COM_OPC_UA_WEBSOCKET_SECURE
            "Enable secure OPC UA over WebSocket (opc.wss://) endpoint" ON
    )
    if (NOT FORTE_COM_OPC_UA_WEBSOCKET_INSECURE AND NOT FORTE_COM_OPC_UA_WEBSOCKET_SECURE)
        message(FATAL_ERROR "FORTE_COM_OPC_UA_WEBSOCKET is enabled, but neither FORTE_COM_OPC_UA_WEBSOCKET_INSECURE nor FORTE_COM_OPC_UA_WEBSOCKET_SECURE is selected.")
    endif ()

    # UA_ServerConfig::webSocketEnabled/webSocketAllowUnencrypted/webSocketCertificate/
    # webSocketPrivateKey and the UA_ENABLE_LWS macro are a patch on top of upstream
    # open62541, not present in stock releases such as the "1.5.4" pulled in by the
    # find_package() fallback below. Checked via plain CMAKE_REQUIRED_INCLUDES rather
    # than linking open62541::open62541: FORTE_COM_OPC_UA_INCLUDE_DIR mode is
    # headers-only by design (see comment above) -- the library is compiled and linked
    # one level up (e.g. as its own ESP-IDF component), so there is nothing to link
    # against yet here, and a link-based check would always spuriously fail.
    if (FORTE_COM_OPC_UA_INCLUDE_DIR)
        # Same paths used to build the open62541::open62541 INTERFACE target above --
        # read directly from that source variable instead of extracting them back out of
        # the target's (BUILD_INTERFACE-wrapped) INTERFACE_INCLUDE_DIRECTORIES via generator-
        # expression string-munging, so there's nothing to keep in sync if that target's
        # include list ever changes shape.
        set(FORTE_COM_OPC_UA_WS_CHECK_INCLUDES
                "${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}"
                "${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/src_generated"
                "${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/include"
                "${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/plugins/include"
        )
    else ()
        # find_package()-imported targets expose plain, already-resolved paths here (no
        # BUILD_INTERFACE wrapping), so no extra handling is needed for this branch.
        get_target_property(FORTE_COM_OPC_UA_WS_CHECK_INCLUDES open62541::open62541 INTERFACE_INCLUDE_DIRECTORIES)
        if (NOT FORTE_COM_OPC_UA_WS_CHECK_INCLUDES)
            set(FORTE_COM_OPC_UA_WS_CHECK_INCLUDES "")
        endif ()
    endif ()

    include(CheckStructHasMember)
    include(CheckSymbolExists)
    include(CMakePushCheckState)
    cmake_push_check_state(RESET)
    set(CMAKE_REQUIRED_INCLUDES ${FORTE_COM_OPC_UA_WS_CHECK_INCLUDES})
    check_struct_has_member("UA_ServerConfig" webSocketEnabled "open62541/server.h"
            FORTE_COM_OPC_UA_HAS_WS_ENABLED LANGUAGE C)
    check_struct_has_member("UA_ServerConfig" webSocketAllowUnencrypted "open62541/server.h"
            FORTE_COM_OPC_UA_HAS_WS_ALLOW_UNENCRYPTED LANGUAGE C)
    check_struct_has_member("UA_ServerConfig" webSocketCertificate "open62541/server.h"
            FORTE_COM_OPC_UA_HAS_WS_CERTIFICATE LANGUAGE C)
    check_struct_has_member("UA_ServerConfig" webSocketPrivateKey "open62541/server.h"
            FORTE_COM_OPC_UA_HAS_WS_PRIVATE_KEY LANGUAGE C)
    check_symbol_exists(UA_ENABLE_LWS "open62541/config.h" FORTE_COM_OPC_UA_HAS_UA_ENABLE_LWS)
    cmake_pop_check_state()
    unset(FORTE_COM_OPC_UA_WS_CHECK_INCLUDES)

    if (NOT (FORTE_COM_OPC_UA_HAS_WS_ENABLED AND FORTE_COM_OPC_UA_HAS_WS_ALLOW_UNENCRYPTED AND
             FORTE_COM_OPC_UA_HAS_WS_CERTIFICATE AND FORTE_COM_OPC_UA_HAS_WS_PRIVATE_KEY))
        message(FATAL_ERROR
                "FORTE_COM_OPC_UA_WEBSOCKET requires an open62541 build with WebSocket support on "
                "UA_ServerConfig (webSocketEnabled, webSocketAllowUnencrypted, webSocketCertificate, "
                "webSocketPrivateKey), which stock upstream open62541 does not have. Point "
                "FORTE_COM_OPC_UA_INCLUDE_DIR at a patched open62541 checkout instead of relying on "
                "find_package(open62541).")
    endif ()

    # The secure transport (opc.wss://) is only ever provided by open62541's LWS-based
    # ConnectionManager (see opcua_local_handler.cpp) -- the platform-ConnectionManager
    # fallback for platforms without libwebsockets only ever supports the plain,
    # unencrypted WebSocket transport. So with the default SECURE=ON/INSECURE=OFF, a
    # UA_ENABLE_LWS-less open62541 build would silently register no WebSocket endpoint
    # at all despite FORTE_COM_OPC_UA_WEBSOCKET being on.
    if (FORTE_COM_OPC_UA_WEBSOCKET_SECURE AND NOT FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
            AND NOT FORTE_COM_OPC_UA_HAS_UA_ENABLE_LWS)
        message(FATAL_ERROR
                "FORTE_COM_OPC_UA_WEBSOCKET_SECURE (opc.wss://) requires an open62541 build with "
                "UA_ENABLE_LWS -- it is the only backend that supports the secure transport; the "
                "platform-ConnectionManager fallback (FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR) "
                "only ever supports opc.ws://. Either use an open62541 build with UA_ENABLE_LWS, or set " # nosemgrep: javascript.lang.security.detect-insecure-websocket
                "FORTE_COM_OPC_UA_WEBSOCKET_INSECURE=ON to get opc.ws:// via the platform " # nosemgrep: javascript.lang.security.detect-insecure-websocket
                "ConnectionManager instead.")
    endif ()

    # Mirrors the SECURE check above: without UA_ENABLE_LWS, the insecure transport can
    # only come from a platform ConnectionManager (see the #elif
    # FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER chain in opcua_local_handler.cpp),
    # which requires FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR. Without either,
    # configureUAServer() silently registers no WebSocket endpoint at all.
    if (FORTE_COM_OPC_UA_WEBSOCKET_INSECURE AND NOT FORTE_COM_OPC_UA_HAS_UA_ENABLE_LWS
            AND NOT FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR)
        message(FATAL_ERROR
                "FORTE_COM_OPC_UA_WEBSOCKET_INSECURE (opc.ws://) requires either an open62541 " # nosemgrep: javascript.lang.security.detect-insecure-websocket
                "build with UA_ENABLE_LWS or FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR "
                "pointing at a platform-provided ws_connection_manager.h; otherwise no "
                "WebSocket endpoint is registered at runtime.")
    endif ()
else ()
    set(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE OFF)
    set(FORTE_COM_OPC_UA_WEBSOCKET_SECURE OFF)
endif ()

set(FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
        CACHE STRING
        "Custom hostname which is used for the OPC UA app name and app uri"
)

# OPC UA Event Layer
option(FORTE_COM_OPC_UA_Events "Enable OPC UA Event Support" OFF)
option(FORTE_COM_OPC_UA_AC "Enable OPC UA A&C Support" OFF)

add_library(forte-com-opc_ua)
target_link_libraries(forte-com-opc_ua PUBLIC forte-core open62541::open62541)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-opc_ua,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-opc_ua>>)

if (FORTE_COM_OPC_UA_WEBSOCKET AND FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR)
    if (FORTE_COM_OPC_UA_HAS_UA_ENABLE_LWS)
        # The open62541 build already has libwebsockets support, which
        # configureUAServer() always prefers over the platform ConnectionManager
        # (see the #ifdef UA_ENABLE_LWS / #elif FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER
        # chain in opcua_local_handler.cpp). Defining the latter here too would be not
        # just redundant but actively dangerous: it would make
        # WsConnectionManager_poll() reachable while mWsConnectionManager is really an
        # LWS-backed UA_ConnectionManager, which is undefined behavior.
        message(STATUS
                "FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR is set, but this open62541 "
                "build already has UA_ENABLE_LWS; ignoring it since the LWS backend takes "
                "priority.")
    else ()
        get_filename_component(FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR_ABS
                "${FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR}" ABSOLUTE)
        target_include_directories(forte-com-opc_ua PRIVATE
                $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER_DIR_ABS}>
        )
        target_compile_definitions(forte-com-opc_ua PRIVATE FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER)
    endif ()
endif ()

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-com-opc_ua EXPORT forte-export FILE_SET HEADERS)
