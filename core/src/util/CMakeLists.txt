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

set(FORTE_LOGLEVEL "$<IF:$<CONFIG:Debug>,LOGINFO,LOGERROR>" CACHE STRING "Loglevel to use")
set_property(CACHE FORTE_LOGLEVEL PROPERTY STRINGS LOGDEBUG LOGERROR LOGWARNING LOGINFO NOLOG)

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
