#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
# *    Jose Cabral - Add modular IOs
# *******************************************************************************/ 
#############################################################################
# WAGO KBUS SIFBs
#############################################################################
if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
forte_add_module(WagoKbus OFF "Wago Kbus interface")

if(FORTE_MODULE_WagoKbus)
    # All Wago depedencies are added here
    if(FORTE_IO)
      add_subdirectory(modular)
      message("Building New Modular wago IOs")
    else(FORTE_IO)
      message("Building Old wago IOs")
      forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
      forte_add_sourcefile_hcpp(processinterface)
      forte_set_process_interface("Wago Kbus" IX QX IW QW)
      forte_add_handler(WagoPFCProcessInterface::CKBusHandler processinterface) 
    endif(FORTE_IO)
  
    SET(FORTE_WAGO_INCLUDE_DIR "" CACHE STRING "Path where the include for wago is found") 
    SET(FORTE_WAGO_LIB_DIR "" CACHE STRING "Path where the  libraries for wago is found") 
  
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR})
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/dal)
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/OsLinux)
    forte_add_link_directories(${FORTE_WAGO_LIB_DIR})
    
    forte_add_link_library(dal libloader pthread ffi dbus-glib-1 glib-2.0)
    forte_add_link_library(typelabel oslinux dbuskbuscommon)
  
    #TODO: Further dependencies are to be checked and added later

endif(FORTE_MODULE_WagoKbus)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
