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
forte_add_module(WagoKbus OFF "Wago Kbus interface")

if(FORTE_MODULE_WagoKbus)
  if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

    # All Wago depedencies are added here
    if(FORTE_IO)
      add_subdirectory(modular)
      message("Building New Modular wago IOs")
    else(FORTE_IO)
      message("Building Old wago IOs")
      forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
      forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
      forte_add_sourcefile_hcpp( ../../stdfblib/io/QX) 
      forte_add_sourcefile_hcpp( ../../stdfblib/io/QW) 
      forte_add_sourcefile_hcpp( ../../stdfblib/io/IW) 
      forte_add_sourcefile_hcpp(processinterface)
      forte_add_handler(WagoPFCProcessInterface::CKBusHandler processinterface) 
    endif(FORTE_IO)
  
    SET(FORTE_WAGO_INCLUDE_DIR "" CACHE STRING "Path where the include for wago is found") 
    SET(FORTE_WAGO_LIB_DIR "" CACHE STRING "Path where the  libraries for wago is found") 
  
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR})
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/dal)
    forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/OsLinux)
    forte_add_link_directories(${FORTE_WAGO_LIB_DIR})
    
    forte_add_link_library(libdal.a liblibloader.so libpthread.so libffi.so libdbus-glib-1.so libglib-2.0.so librt.so)
    forte_add_link_library(libtypelabel.so liboslinux.so libdbuskbuscommon.so)    
    forte_add_link_flags("-Wall") 
  
    #TODO: Further dependencies are to be checked and added later

  else("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    MESSAGE(FATAL_ERROR "The WAGO_KBUS IO system is only supported for FORTE on Linux platforms!")
  endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

endif(FORTE_MODULE_WagoKbus)
