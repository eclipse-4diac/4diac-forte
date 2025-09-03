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
            factory.h
            hook.h
            singlet.h
            criticalregion.h
        devlog.h
        devlog.cpp
            fixed_string.h
            fortearray.h
            inplace_vector.h
        mainparam_utils.h
        mainparam_utils.cpp
        registry.h
            ringbuf.h
            string_utils.h
            string_utils.cpp
            parameterParser.h
            parameterParser.cpp
            configFileParser.h
            configFileParser.cpp
        $<IF:$<BOOL:${FORTE_LOGGER_READABLE_TIME}>,${CMAKE_CURRENT_SOURCE_DIR}/readableTimeString.cpp,${CMAKE_CURRENT_SOURCE_DIR}/plainTimeString.cpp>
)
