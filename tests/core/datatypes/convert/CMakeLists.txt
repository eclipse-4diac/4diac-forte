#*******************************************************************************
# Copyright (c) 2019 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik-Merkumians  - adds package layer for convert tests
# *******************************************************************************/

forte_test_add_sourcefile_cpp(convert_functionstests.cpp)

# convert functions for TO_(W)STRING conversions
  forte_test_add_sourcefile_cpp(convert_to_STRING_test.cpp)

IF(FORTE_USE_WSTRING_DATATYPE)
  forte_test_add_sourcefile_cpp(convert_to_WSTRING_test.cpp)
ENDIF(FORTE_USE_WSTRING_DATATYPE)
