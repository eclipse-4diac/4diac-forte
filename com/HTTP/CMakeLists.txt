# *******************************************************************************
# *******************************************************************************
# Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Marc Jakobi
# *******************************************************************************/
# *******************************************************************************/
# ############################################################################
# HTTP Com Layer (currently implements an HTTP client)
# ############################################################################
option(FORTE_COM_HTTP "Enable an HTTP Com Layer" OFF)
if (NOT FORTE_COM_HTTP)
    return()
endif ()

set(FORTE_COM_HTTP_LISTENING_PORT
        80
        CACHE STRING "HTTP Listening Port"
)

configure_file(http_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/http_config.h)

add_library(forte-com-HTTP
        httplayer.cpp
        httplayer.h
        httpparser.h
        httpparser.cpp
        http_handler.h
        http_handler.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/http_config.h
)
target_include_directories(forte-com-HTTP PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(forte-com-HTTP PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-HTTP,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-HTTP>>)

install(TARGETS forte-com-HTTP EXPORT forte-export FILE_SET HEADERS)
