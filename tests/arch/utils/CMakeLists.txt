#*******************************************************************************
# * Copyright (c) 2018 ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Martin Melik-Merkumians - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src/arch/utils)
 
if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  forte_test_add_sourcefile_cpp(../../../src/arch/utils/timespec_utils.cpp)
 
  forte_test_add_sourcefile_cpp(timespec_utils_tests.cpp)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")