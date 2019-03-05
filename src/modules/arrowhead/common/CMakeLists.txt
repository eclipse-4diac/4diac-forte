# *******************************************************************************
# * Copyright (c) 2018, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral - initial implementation
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
  