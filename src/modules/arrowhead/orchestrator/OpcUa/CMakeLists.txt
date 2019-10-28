# *******************************************************************************
# Copyright (c) 2019, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Jose Cabral - initial implementation
# *******************************************************************************

#############################################################################
# Arrowhead specific functional blocks for OpcUa for the orchestrator
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(RequestOrchestrationOpcUa)
