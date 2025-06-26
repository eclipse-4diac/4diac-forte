#*******************************************************************************
# Copyright (c) 2010 - 2024 ACIN, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Alois Zoitl, Patrick Smejkal, Martin Melik-Merkumians
# *    - initial API and implementation and/or initial documentation
# *    - added utils directory
# *******************************************************************************/

add_library(forte-arch OBJECT
    timerha.cpp
    devlog.cpp
    forte_architecture.cpp
    $<IF:$<BOOL:${FORTE_LOGGER_READABLE_TIME}>,readableTimeString.cpp,plainTimeString.cpp>
)

target_link_libraries(forte PRIVATE forte-arch)

###############################################################################
# Architectures

macro(forte_register_architecture arch)
    set_property(TARGET forte-arch APPEND PROPERTY forte_architectures ${arch})
    if (NOT FORTE_ARCHITECTURE STREQUAL "${arch}")
        return()
    endif()
endmacro()

# add_subdirectory(be_m1)
# add_subdirectory(ecos)
# add_subdirectory(freeRTOS)
# add_subdirectory(macos)
# add_subdirectory(netos)
# add_subdirectory(pikeos_posix)
# add_subdirectory(plcnext)
add_subdirectory(posix)
# add_subdirectory(rcX)
# add_subdirectory(vxworks)
# add_subdirectory(fake_time)
add_subdirectory(win32)
# add_subdirectory(zephyr)

add_subdirectory(utils)

###############################################################################
# Configuration Options

set(FORTE_ARCHITECTURE "None" CACHE STRING "Architecture for which to build 4diac FORTE")
get_property(architectures TARGET forte-arch PROPERTY forte_architectures)
list(SORT architectures)
set_property(CACHE FORTE_ARCHITECTURE PROPERTY STRINGS None ${architectures})

if (FORTE_ARCHITECTURE STREQUAL "None")
    message(FATAL_ERROR "No FORTE_ARCHITECTURE selected. Use one of ${architectures}")
endif()

set(FORTE_LOGGER_BUFFER_SIZE "300" CACHE STRING "Buffer length of the logger")
mark_as_advanced(FORTE_LOGGER_BUFFER_SIZE)
target_compile_definitions(forte-arch PRIVATE FORTE_LOGGER_BUFFER_SIZE=${FORTE_LOGGER_BUFFER_SIZE})

option(FORTE_LOGGER_READABLE_TIME "Logger time in IEC 61131-3 date time literal" OFF)
