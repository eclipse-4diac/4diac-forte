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
if (NOT LINUX)
  return ()
endif ()

option(FORTE_MODULE_WagoKbus "Wago Kbus interface" OFF)
          
if (NOT FORTE_MODULE_WagoKbus)
  return()
endif ()

SET(FORTE_WAGO_INCLUDE_DIR "" CACHE STRING "Path where the include for wago is found") 
SET(FORTE_WAGO_LIB_DIR "" CACHE STRING "Path where the  libraries for wago is found") 


add_library(forte-wagoKbus
            WagoHandle.h
            WagoHandle.cpp
            WagoDeviceController.h
            WagoDeviceController.cpp
            types/WagoMaster.h
            types/WagoMaster.cpp
            types/WagoBusAdapter.h
            types/WagoBusAdapter.cpp
            types/WagoSlaveBase.h
            types/WagoSlaveBase.cpp
            types/Wago1506.h
            types/Wago1506.cpp
            types/Wago1405_6.h
            types/Wago1405_6.cpp
            types/Wago1504_5.h
            types/Wago1504_5.cpp
            types/Wago459.h
            types/Wago459.cpp
            types/Wago636_fbt.h
            types/Wago636_fbt.cpp
)

target_link_libraries(forte-wagoKbus PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<LINK_LIBRARY:WHOLE_ARCHIVE,forte-wagoKbus>)

target_include_directories(forte-wagoKbus PRIVATE
            ${FORTE_WAGO_INCLUDE_DIR}
            ${FORTE_WAGO_INCLUDE_DIR}/dal
            ${FORTE_WAGO_INCLUDE_DIR}/OsLinux
)

target_link_directories(forte-wagoKbus PUBLIC ${FORTE_WAGO_LIB_DIR})
target_link_libraries(forte-wagoKbus PRIVATE
            dal
            libloader
            pthread
            ffi
            dbus-glib-1
            glib-2.0
            typelabel
            oslinux
            dbuskbuscommon
)

#TODO: Further dependencies are to be checked and added later
