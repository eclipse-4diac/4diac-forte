# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Kirill Dorofeev - initial implementation
# *******************************************************************************

#############################################################################
# Arrowhead specific functional blocks for OPC UA for service registry
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(RegisterServiceOpcUa)
forte_add_sourcefile_hcpp(QueryServiceOpcUa)

  