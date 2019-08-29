# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral - initial implementation
# *******************************************************************************

#############################################################################
# Common Arrowhead specific functional blocks 
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#datatype
forte_add_sourcefile_hcpp(forte_arrowheadservice forte_arrowheadsystem forte_arrowheadcloud)

#helper FBs
forte_add_sourcefile_hcpp(FieldsToArrowheadService FieldsToArrowheadSystem FieldsToArrowheadCloud)

add_subdirectory(JSON)
  