# *******************************************************************************
# Copyright (c) 2011 - 2015 ACIN, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
# ############################################################################
# core/util
# ############################################################################

include(CMakeDependentOption)

set(FORTE_LOGLEVEL "$<IF:$<CONFIG:Debug,RelWithDebInfo>,LOGINFO,LOGERROR>" CACHE STRING "Loglevel to use")
set_property(CACHE FORTE_LOGLEVEL PROPERTY STRINGS LOGDEBUG LOGERROR LOGWARNING LOGINFO NOLOG)

cmake_dependent_option(FORTE_STACKTRACE "Include stacktrace when logging errors" OFF "NOT FORTE_LOGLEVEL STREQUAL NOLOG" OFF)
if (FORTE_STACKTRACE)
    set(FORTE_STACKTRACE_IMPL "boost" CACHE STRING "Stacktrace implementation to use")
endif ()

option(FORTE_LOGGER_READABLE_TIME "Logger time in IEC 61131-3 date time literal" OFF)

target_sources(forte-core PRIVATE
        configFileParser.cpp
        configFileParser.h
        mainparam_utils.cpp
        parameterParser.cpp
        string_utils.cpp
        $<$<NOT:$<OR:$<STREQUAL:${FORTE_LOGLEVEL},NOLOG>,$<BOOL:${FORTE_EXTERNAL_LOG_HANDLER}>>>:${CMAKE_CURRENT_SOURCE_DIR}/devlog.cpp>
        $<IF:$<BOOL:${FORTE_LOGGER_READABLE_TIME}>,${CMAKE_CURRENT_SOURCE_DIR}/readableTimeString.cpp,${CMAKE_CURRENT_SOURCE_DIR}/plainTimeString.cpp>
)
target_compile_definitions(forte-core PUBLIC "FORTE_${FORTE_LOGLEVEL}")

if (FORTE_STACKTRACE)
    target_compile_definitions(forte-core PRIVATE "FORTE_STACKTRACE")
    if (FORTE_STACKTRACE_IMPL MATCHES "boost")
        find_package(Boost REQUIRED)
        target_link_libraries(forte-core PRIVATE backtrace)
        target_compile_definitions(forte-core PRIVATE "FORTE_STACKTRACE_BOOST" "BOOST_STACKTRACE_USE_BACKTRACE")

        if (WIN32 OR MINGW)
            target_link_options(forte-core PUBLIC -no-pie)
        elseif (UNIX)
            target_link_options(forte-core PUBLIC -rdynamic)
            target_link_libraries(forte-core PRIVATE dl)
        endif ()
    endif ()

    if (FORTE_STACKTRACE_IMPL MATCHES "cxx23")
        target_compile_features(forte-core PRIVATE cxx_std_23)
        target_compile_definitions(forte-core PRIVATE "FORTE_STACKTRACE_CXX23")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_link_libraries(forte-core PRIVATE stdc++exp)
        endif ()
    endif ()
endif ()
