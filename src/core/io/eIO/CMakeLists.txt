# *******************************************************************************
# Copyright (c) 2025 Maximilian Scharf This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Maximilian Scharf - Initial implementation.
# *******************************************************************************/

add_subdirectory(io)
add_library(forte-core-io-eIO OBJECT CeBuffer.h CeConfig.h CeSpec.h)
set_property(TARGET forte-core-io-eIO PROPERTY LINKER_LANGUAGE CXX)
target_link_libraries(forte PRIVATE forte-core-io-eIO)
