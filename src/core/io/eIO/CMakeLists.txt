#*******************************************************************************
# Copyright (c) 2025 Maximilian Scharf
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Maximilian Scharf - Initial implementation.
# *******************************************************************************/

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCE_GROUP ${SOURCE_GROUP}\\eventTrigger)

forte_add_sourcefile_h(CeBuffer.h)
forte_add_sourcefile_h(CeConfig.h)
forte_add_sourcefile_h(CeSpec.h)

add_subdirectory(io)