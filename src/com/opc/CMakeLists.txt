#*******************************************************************************
# Copyright (c) 2012, 2021 AIT, ACIN, fortiss GmbH, Hit robot group
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
#    Tibalt Zhao - Ease the workload to compile OPC DA
# *******************************************************************************/
#############################################################################
# OPC Com Layer
#############################################################################

forte_add_network_layer(OPC OFF "opc" COpcComLayer opccomlayer "Enable OPC communication")
SET(FORTE_COM_OPC_LIB_ROOT "" CACHE PATH "Path to OPC library root directory")
SET(FORTE_COM_OPC_BOOST_ROOT "" CACHE PATH "Path to Boost library root directory")

if(FORTE_COM_OPC)  
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_add_handler(COpcEventHandler opceventhandler)
  
  if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    forte_add_sourcefile_hcpp( opcconnection opcconnectionhandler opceventhandler 
                   opcconnectionimpl opcprocessvar Cmd_AddConnection 
                   Cmd_AddOPCProcessVar Cmd_SetProcessVarValue )
  
    forte_add_include_directories( ${FORTE_COM_OPC_BOOST_ROOT} )
    
    forte_add_include_directories( ${FORTE_COM_OPC_LIB_ROOT}/include )
    forte_add_link_directories( ${FORTE_COM_OPC_LIB_ROOT}/lib )

    forte_add_link_library( OPCClientToolKit.lib )
    
  else()
    # Generate error since OPC only works with Windows
    MESSAGE(SEND_ERROR "Win32 has to be slected for FORTE_ARCHITECTURE for the OPC Com Layer")
  endif()  
endif(FORTE_COM_OPC)
