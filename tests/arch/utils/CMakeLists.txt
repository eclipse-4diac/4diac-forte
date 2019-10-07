#*******************************************************************************
# Copyright (c) 2018 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik-Merkumians - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
 
if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  forte_test_add_sourcefile_cpp(timespec_utils_tests.cpp)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")