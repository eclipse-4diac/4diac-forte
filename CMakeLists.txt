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

option(FORTE_C_INTERFACE "Build C interface to 4diac FORTE" OFF)
mark_as_advanced(FORTE_C_INTERFACE)

option(FORTE_USE_WSTRING_DATATYPE "Enable 4diac FORTE WSTRING Datatypes" ON)
mark_as_advanced(FORTE_USE_WSTRING_DATATYPE)

option(FORTE_UNICODE_SUPPORT "Enable unicode support for STRING and WSTRING. WSTRING needs unicode support enabled." ON)
mark_as_advanced(FORTE_UNICODE_SUPPORT)

option(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES "Enable 4diac FORTE Custom Serializeable Datatypes" ON)
mark_as_advanced(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES)

option(FORTE_RTTI_AND_EXCEPTIONS "Enable RTTI and Exceptions" OFF)
mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)

option(FORTE_SUPPORT_MONITORING "Enable FORTE monitoring functionalities" ON)
mark_as_advanced(FORTE_SUPPORT_MONITORING)

option(FORTE_SUPPORT_BOOT_FILE "Enable 4diac FORTE boot file" ON)
mark_as_advanced(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES)

set(FORTE_BOOT_FILE_LOCATION "forte.fboot" CACHE STRING "Path to boot file")
mark_as_advanced(FORTE_BOOT_FILE_LOCATION)

option(FORTE_TRACE_CTF "Enable Common Trace Format (CTF) tracing" OFF)

set(FORTE_TRACE_CTF_BUFFER_SIZE 4096 CACHE STRING "Common Trace Format (CTF) trace buffer size")
mark_as_advanced(FORTE_TRACE_CTF_BUFFER_SIZE)

set(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY "30" CACHE STRING "Max supported hierarchy that can be provided in a management commands")
mark_as_advanced(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY)

add_library(forte-global INTERFACE)
link_libraries(forte-global)

target_compile_definitions(forte-global INTERFACE
    $<$<BOOL:${FORTE_USE_WSTRING_DATATYPE}>:FORTE_USE_WSTRING_DATATYPE>
    $<$<BOOL:${FORTE_UNICODE_SUPPORT}>:FORTE_UNICODE_SUPPORT>
    $<$<BOOL:${FORTE_TRACE_CTF}>:FORTE_TRACE_CTF>
    $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:FORTE_SUPPORT_BOOT_FILE>
    $<$<BOOL:${FORTE_SUPPORT_MONITORING}>:FORTE_SUPPORT_MONITORING>
    $<$<BOOL:${FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES}>:FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES>
    FORTE_TRACE_CTF_BUFFER_SIZE=${FORTE_TRACE_CTF_BUFFER_SIZE}
    "FORTE_BOOT_FILE_LOCATION=\"${FORTE_BOOT_FILE_LOCATION}\""
    FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY=${FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY}
)

if (NOT MSVC)
    target_compile_options(forte-global INTERFACE
        $<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-frtti,-fno-rtti>
        $<IF:$<BOOL:${FORTE_RTTI_AND_EXCEPTIONS}>,-fexceptions,-fno-exceptions>
    )
endif()

#######################################################################################
# Determine log level
#######################################################################################
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)
if (BUILD_TYPE_LOWERCASE STREQUAL "debug")
    if (FORTE_LOGLEVEL MATCHES "NOLOG")
        message("Log Level is set to NOLOG and CMAKE_BUILD_TYPE is set to Debug. Are sure you don't want to log anything?")
    endif()
    if (NOT DEFINED FORTE_LOGLEVEL)
    	message("No Log Level was specified. Setting Log Level to Debug default LOGINFO.")
        set(FORTE_LOGLEVEL "LOGINFO" CACHE STRING "Loglevel to use" FORCE)
    endif()
else()
    if (NOT DEFINED FORTE_LOGLEVEL)
        message("No Log Level was specified. Setting Log Level to Release default LOGERROR.")
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

#######################################################################################
# Main build
#######################################################################################
add_subdirectory(src)
