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

add_library(forte-core-util OBJECT
            singlet.h
            criticalregion.h
            fortearray.h
            inplace_vector.h
            ringbuf.h
            string_utils.h
            string_utils.cpp
            parameterParser.h
            parameterParser.cpp
            configFileParser.h
            configFileParser.cpp
)
target_link_libraries(forte PRIVATE forte-core-util)
