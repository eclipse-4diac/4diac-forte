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

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-frtti HAVE_RTTI_FLAG)
check_cxx_compiler_flag(-fexceptions HAVE_EXCEPTIONS_FLAG)

cmake_dependent_option(FORTE_RTTI_AND_EXCEPTIONS "Enable RTTI and Exceptions" OFF "HAVE_RTTI_FLAG AND HAVE_EXCEPTIONS_FLAG AND NOT FORTE_TESTS" ON)
mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)

target_compile_options(forte-core PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:$<$<BOOL:${HAVE_RTTI_FLAG}>:$<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-frtti,-fno-rtti>>>
        $<$<COMPILE_LANGUAGE:CXX>:$<$<BOOL:${HAVE_EXCEPTIONS_FLAG}>:$<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-fexceptions,-fno-exceptions>>>
)

###############################################################################
# Architectures

set(FORTE_ARCHITECTURE ${CMAKE_SYSTEM_NAME} CACHE STRING "Architecture for which to build 4diac FORTE")

macro(forte_register_architecture canonical_name)
    set(archs "${canonical_name}" "${ARGN}")
    foreach (arch IN LISTS archs)
        set_property(TARGET forte-core APPEND PROPERTY forte_architectures ${arch})
    endforeach ()
    if (NOT FORTE_ARCHITECTURE IN_LIST archs)
        return()
    endif ()
    if (NOT FORTE_ARCHITECTURE STREQUAL canonical_name)
        message(STATUS "Using ${canonical_name} as FORTE_ARCHITECTURE")
        set(FORTE_ARCHITECTURE "${canonical_name}")
    endif ()
endmacro()

add_subdirectory(common)
add_subdirectory(c_interface)

# add_subdirectory(be_m1)
# add_subdirectory(ecos)
add_subdirectory(freeRTOS)
# add_subdirectory(netos)
# add_subdirectory(pikeos_posix)
# add_subdirectory(plcnext)
add_subdirectory(posix)
# add_subdirectory(rcX)
# add_subdirectory(vxworks)
add_subdirectory(fake_time)
add_subdirectory(win32)
# add_subdirectory(zephyr)

get_property(architectures TARGET forte-core PROPERTY forte_architectures)
list(SORT architectures)
set_property(CACHE FORTE_ARCHITECTURE PROPERTY STRINGS None ${architectures})

if (NOT FORTE_ARCHITECTURE IN_LIST architectures)
    message(FATAL_ERROR "Invalid FORTE_ARCHITECTURE selected. Use one of ${architectures}")
endif ()
