#*******************************************************************************
# Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
#     Jose Cabral - Add modular IOs
#     Monika Wenger - all files start with capital letters
# *******************************************************************************/ 
#############################################################################
# WAGO KBUS SIFBs
#############################################################################
if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
# FIXME: shouldn't this be io, not module?
forte_add_module(WagoKbus OFF "Wago Kbus interface" FORTE_IO)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
forte_add_sourcefile_hcpp(WagoHandle WagoDeviceController)
forte_add_sourcefile_hcpp(types/WagoMaster types/WagoBusAdapter types/WagoSlaveBase)
forte_add_sourcefile_hcpp(types/Wago1506 types/Wago1405_6 types/Wago1504_5 types/Wago459 types/Wago636_fbt)
  
SET(FORTE_WAGO_INCLUDE_DIR "" CACHE STRING "Path where the include for wago is found") 
SET(FORTE_WAGO_LIB_DIR "" CACHE STRING "Path where the  libraries for wago is found") 

forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR})
forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/dal)
forte_add_include_system_directories(${FORTE_WAGO_INCLUDE_DIR}/OsLinux)
forte_add_link_directories(${FORTE_WAGO_LIB_DIR})

forte_add_link_library(dal libloader pthread ffi dbus-glib-1 glib-2.0)
forte_add_link_library(typelabel oslinux dbuskbuscommon)

#TODO: Further dependencies are to be checked and added later

endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
