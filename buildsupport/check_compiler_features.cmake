#********************************************************************************
# Copyright (c) 2020 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik Merkumians
#      - initial API and implementation and/or initial documentation
# *******************************************************************************/
include(CheckCXXSourceCompiles)

MACRO(TEST_NEEDED_FORTE_COMPILER_FEATURES)
CHECK_CXX_SOURCE_COMPILES("int main() { void* p = nullptr; }"
    NULLPTR_SUPPORTED
    FAIL_REGEX "use of undeclared identifier 'nullptr'")
if(NOT NULLPTR_SUPPORTED)
    message(WARNING "`nullptr` not supported, replacing it with 0 via precprocessor")
    forte_add_definition("-Dnullptr=0")
endif()
ENDMACRO(TEST_NEEDED_FORTE_COMPILER_FEATURES)