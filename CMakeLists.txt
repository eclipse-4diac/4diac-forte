#********************************************************************************
# Copyright (c) 2010 - 2015 Profactor GmbH, AIT, fortiss GmbH
#               2010-2015, 2020 TU Wien/ACIN
#               2022 Martin Erich Jobst
#               2024 Jose Cabral
#               2025 Jörg Walter
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Michael Hofmann, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Thomas Strasser,
#    Martin Melik Merkumians
#      - initial API and implementation and/or initial documentation
#    Martin Melik Merkumians - adds compiler feature check for nullptr
#    Martin Jobst
#      - add CTF tracing support
#    Jörg Walter
#      - complete rewrite
# *******************************************************************************/

cmake_minimum_required(VERSION 3.25)
project(FORTE C CXX)

set(CMAKE_CXX_STANDARD 20)

#######################################################################################
# Global configuration
#######################################################################################
option(FORTE_BUILD_EXECUTABLE "Build 4diac FORTE as an executable, otherwise build a library" ON)
mark_as_advanced(FORTE_BUILD_EXECUTABLE)

# TODO
#option(FORTE_C_INTERFACE "Build C interface to 4diac FORTE" OFF)
#mark_as_advanced(FORTE_C_INTERFACE)

option(FORTE_RTTI_AND_EXCEPTIONS "Enable RTTI and Exceptions" OFF)
mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)

add_library(forte-global INTERFACE)
link_libraries(forte-global)

#######################################################################################
# Determine log level
#######################################################################################
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)
if (BUILD_TYPE_LOWERCASE STREQUAL "debug")
    if (FORTE_LOGLEVEL MATCHES "NOLOG")
        message(WARNING "Log Level is set to NOLOG and CMAKE_BUILD_TYPE is set to Debug. Are sure you don't want to log anything?")
    endif()
    if (NOT DEFINED FORTE_LOGLEVEL)
    	message(STATUS "No Log Level was specified. Setting Log Level to Debug default LOGINFO.")
        set(FORTE_LOGLEVEL "LOGINFO" CACHE STRING "Loglevel to use" FORCE)
    endif()
else()
    if (NOT DEFINED FORTE_LOGLEVEL)
        message(STATUS "No Log Level was specified. Setting Log Level to Release default LOGERROR.")
        set(FORTE_LOGLEVEL "LOGERROR" CACHE STRING "Loglevel to use" FORCE)
    endif()
endif()
set_property(CACHE FORTE_LOGLEVEL PROPERTY STRINGS LOGDEBUG LOGERROR LOGWARNING LOGINFO NOLOG)

target_compile_definitions(forte-global INTERFACE ${FORTE_LOGLEVEL})

#############################################################################
# Configure stacktrace
#############################################################################
# if (FORTE_STACKTRACE)
#     if (FORTE_STACKTRACE_IMPL MATCHES "boost")
#         find_package(Boost REQUIRED)
#         forte_add_link_library(backtrace)
#         forte_add_custom_configuration("#define BOOST_STACKTRACE_USE_BACKTRACE")

#         if (WIN32 OR MINGW)
#             forte_add_link_flags(-no-pie)
#         elseif (UNIX)
#             forte_add_link_flags(-rdynamic)
#             forte_add_link_library(dl)
#         endif ()
#     endif ()

#     if (FORTE_STACKTRACE_IMPL MATCHES "cxx23")
#         if (CMAKE_COMPILER_IS_GNUCXX)
#             forte_add_link_library(-lstdc++_libbacktrace)
#         endif ()
#     endif ()
# endif ()

#############################################################################
# Whole archive
#############################################################################
if (NOT CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE_SUPPORTED)
    message(FATAL_ERROR
            "Enable WHOLE_ARCHIVE linker feature in your CMake toolchain file, for example:\n"
            "  set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE\n"
            "      \"-Wl,--whole-archive\" \"<LINK_ITEM>\" \"-Wl,--no-whole-archive\"\n"
            "  )\n"
            "  set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE_SUPPORTED TRUE)\n"
    )
endif ()

#######################################################################################
# Main build
#######################################################################################

if (FORTE_BUILD_EXECUTABLE)
    add_executable(forte)
    install(TARGETS forte RUNTIME DESTINATION bin)
    message(STATUS "Building executable")
else ()
    add_library(forte)
    install(TARGETS forte RUNTIME DESTINATION lib)
    message(STATUS "Building library")
endif ()

add_subdirectory(core)
add_subdirectory(arch)
add_subdirectory(stdfblib)
add_subdirectory(com)

add_subdirectory(src)

enable_testing()
add_subdirectory(systemtests)
add_subdirectory(tests)

# set this late as it may be overridden by tests
if (NOT MSVC)
    target_compile_options(forte-global INTERFACE
        $<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-frtti,-fno-rtti>
        $<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-fexceptions,-fno-exceptions>
    )
endif()
