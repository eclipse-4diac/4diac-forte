#*******************************************************************************
# Copyright (c) 2026 Thomas Oellinger
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Thomas Oellinger - initial implementation
# *******************************************************************************/

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_MCCDAQHAT "Support for Measurement Computing DAQ HATs (MCC 128, MCC 152) on Raspberry Pi" OFF)

if (NOT FORTE_MODULE_MCCDAQHAT)
    return()
endif ()

find_path(MCCDAQHAT_INCLUDE_DIR daqhats/daqhats.h)
find_library(MCCDAQHAT_LIBRARY daqhats)
if (NOT MCCDAQHAT_INCLUDE_DIR OR NOT MCCDAQHAT_LIBRARY)
    message(FATAL_ERROR "FORTE_MODULE_MCCDAQHAT requires the daqhats library (https://github.com/mccdaq/daqhats). "
                        "Set MCCDAQHAT_INCLUDE_DIR and MCCDAQHAT_LIBRARY if it is not installed in a default location.")
endif ()

add_library(forte-mccdaqhat
            mccdaqhat_api.h
            mccdaqhat_lib_api.cpp
            mccdaqhat_config.h
            mccdaqhat_config.cpp
            boards/MCCBoard.h
            boards/MCCBoard.cpp
            boards/MCC128Board.h
            boards/MCC128Board.cpp
            boards/MCC152Board.h
            boards/MCC152Board.cpp
            handles/MCCHandles.h
            handles/MCCHandles.cpp
            MCCDaqHatController.h
            MCCDaqHatController.cpp
)
# Only mccdaqhat_lib_api.cpp sees the vendor header; everything else uses IMCCDaqHatApi.
target_include_directories(forte-mccdaqhat PRIVATE ${MCCDAQHAT_INCLUDE_DIR})
target_link_libraries(forte-mccdaqhat PUBLIC forte-core PRIVATE ${MCCDAQHAT_LIBRARY})
# A static libdaqhats carries no dependency info; it needs libgpiod and libm at link time.
# The shared library already pulls them in.
if (MCCDAQHAT_LIBRARY MATCHES "\\.a$")
    find_library(MCCDAQHAT_GPIOD_LIBRARY gpiod)
    if (NOT MCCDAQHAT_GPIOD_LIBRARY)
        message(FATAL_ERROR "A static daqhats library needs libgpiod at link time. "
                            "Set MCCDAQHAT_GPIOD_LIBRARY to the libgpiod library.")
    endif ()
    target_link_libraries(forte-mccdaqhat PRIVATE ${MCCDAQHAT_GPIOD_LIBRARY} m)
endif ()
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-mccdaqhat,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-mccdaqhat>>)
install(TARGETS forte-mccdaqhat EXPORT forte-export FILE_SET HEADERS)
