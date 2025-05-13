#*******************************************************************************
# *******************************************************************************
# Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Marc Jakobi
# *******************************************************************************/
# *******************************************************************************/

#############################################################################
# HTTP Com Layer (currently implements an HTTP client)
#############################################################################
forte_register_com_layer(HTTP OFF "http" CHttpComLayer httplayer.h "Enable an HTTP Com Layer")

if (NOT FORTE_COM_HTTP)
    return()
endif()

add_library(forte-com-http OBJECT
  httpparser.cpp
  http_handler.cpp
  httplayer.cpp
)

target_link_libraries(forte PRIVATE forte-com-http)

forte_register_handler(CHTTP_Handler http_handler.h)

set(FORTE_COM_HTTP_LISTENING_PORT "80" CACHE STRING "HTTP Listening Port")
file(GENERATE
  OUTPUT "${FORTE_GENERATED_FILES_DIR}/com_HTTP_defines.h"
  CONTENT "#define FORTE_COM_HTTP_LISTENING_PORT ${FORTE_COM_HTTP_LISTENING_PORT}"
)
