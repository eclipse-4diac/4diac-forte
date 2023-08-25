#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

###############################################################
# PiFace Raspberry Pi expansion board
###############################################################
if ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
forte_add_module(MLPI OFF "Process interface using the MLPI functions for Bosch Rexroth PLCs")

if(FORTE_MODULE_MLPI)
  SET(FORTE_COM_MLPI_DIR "" CACHE PATH "Path to MLPI src directory")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_sourcefile_hcpp(processinterface)
  forte_set_process_interface("MLPI" IX QX)
  
  forte_add_handler(CMLPIFaceProcessInterface::CIOHandler processinterface)
  
  forte_add_include_system_directories( ${FORTE_COM_MLPI_DIR}/mlpiCore/include )
          
endif()
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
