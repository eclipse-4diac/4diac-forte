# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial implementation
# *******************************************************************************

# ############################################################################
# Arrowhead specific functional blocks
# ############################################################################

forte_register_module(Arrowhead OFF "Arrowhead Function Blocks")

add_library(forte-modules-arrowhead OBJECT)
target_link_libraries(forte PRIVATE forte-modules-arrowhead)

add_subdirectory(common)
add_subdirectory(orchestrator)
add_subdirectory(serviceRegistry)
add_subdirectory(eventHandler)
