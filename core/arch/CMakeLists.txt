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

macro(forte_register_architecture arch)
    set_property(TARGET forte-core APPEND PROPERTY forte_architectures ${arch})
    if (NOT FORTE_ARCHITECTURE STREQUAL "${arch}")
        return()
    endif ()
endmacro()

if (WIN32)
	set(default_arch "Win32")
elseif (APPLE)
	set(default_arch "MacOs")
elseif (UNIX)
	set(default_arch "Posix")
else ()
	set(default_arch "None")
endif ()

set(FORTE_ARCHITECTURE "${default_arch}" CACHE STRING "Architecture for which to build 4diac FORTE")

if (FORTE_ARCHITECTURE STREQUAL "None")
    message(FATAL_ERROR "No FORTE_ARCHITECTURE selected. Use one of ${architectures}")
endif ()

add_subdirectory(common)
add_subdirectory(c_interface)

# add_subdirectory(be_m1)
# add_subdirectory(ecos)
add_subdirectory(freeRTOS)
# add_subdirectory(macos)
# add_subdirectory(netos)
# add_subdirectory(pikeos_posix)
# add_subdirectory(plcnext)
add_subdirectory(posix)
# add_subdirectory(rcX)
# add_subdirectory(vxworks)
add_subdirectory(fake_time)
add_subdirectory(win32)
# add_subdirectory(zephyr)

###############################################################################
# Configuration Options

get_property(architectures TARGET forte-core PROPERTY forte_architectures)
list(SORT architectures)
set_property(CACHE FORTE_ARCHITECTURE PROPERTY STRINGS None ${architectures})
