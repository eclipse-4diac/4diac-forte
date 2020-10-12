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
forte_add_network_layer(HTTP OFF "http" CHttpComLayer httplayer "Enable an HTTP Com Layer")
if(FORTE_COM_HTTP)

  forte_add_sourcefile_hcpp(httpparser)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_handler(CHTTP_Handler http_handler)
  forte_add_sourcefile_hcpp(http_handler)
  
      
  SET(FORTE_COM_HTTP_LISTENING_PORT "80" CACHE STRING "HTTP Listening Port")
  
  forte_add_custom_configuration("#define FORTE_COM_HTTP_LISTENING_PORT ${FORTE_COM_HTTP_LISTENING_PORT}")
  forte_add_custom_configuration("extern TForteUInt16 gHTTPServerPort\;")
  
  forte_add_custom_configuration("#cmakedefine FORTE_COM_HTTP")
  
endif(FORTE_COM_HTTP)
