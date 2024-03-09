#********************************************************************************
# Copyright (c) 2020 TU Wien/ACIN, OFFIS e.V.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik Merkumians
#      - initial API and implementation and/or initial documentation
#   Jörg Walter - fix detection to honor C++ standard settings
# *******************************************************************************/
cmake_policy(PUSH)
cmake_policy(SET CMP0067 NEW)

include(CheckCXXSourceCompiles)

function(TEST_NEEDED_FORTE_COMPILER_FEATURES)
  check_cxx_source_compiles("int main() { void* p = nullptr; }"
    NULLPTR_SUPPORTED
    FAIL_REGEX "use of undeclared identifier 'nullptr'")
  if(NOT NULLPTR_SUPPORTED)
    message(WARNING "`nullptr` not supported, replacing it with 0 via precprocessor")
    forte_add_definition("-Dnullptr=0")
  endif()
endfunction()

cmake_policy(POP)
