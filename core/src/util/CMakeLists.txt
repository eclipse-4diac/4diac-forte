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

option(FORTE_LOGGER_READABLE_TIME "Logger time in IEC 61131-3 date time literal" OFF)

target_sources(forte-core PRIVATE
        configFileParser.cpp
        configFileParser.h
        devlog.cpp
        mainparam_utils.cpp
        parameterParser.cpp
        string_utils.cpp
        $<IF:$<BOOL:${FORTE_LOGGER_READABLE_TIME}>,${CMAKE_CURRENT_SOURCE_DIR}/readableTimeString.cpp,${CMAKE_CURRENT_SOURCE_DIR}/plainTimeString.cpp>
)
