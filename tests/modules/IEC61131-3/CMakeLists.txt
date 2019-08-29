#*******************************************************************************
# Copyright (c) 2018 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik-Merkumians  - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Tests for the IEC 61131-3 function FBs
#############################################################################
forte_test_add_sourcefile_cpp(F_DIV_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_S_TO_LINT_tester.cpp F_TIME_IN_MS_TO_LINT_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_US_TO_LINT_tester.cpp F_TIME_IN_NS_TO_LINT_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_S_TO_ULINT_tester.cpp F_TIME_IN_MS_TO_ULINT_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_US_TO_ULINT_tester.cpp F_TIME_IN_NS_TO_ULINT_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_S_TO_LREAL_tester.cpp F_TIME_IN_MS_TO_LREAL_tester.cpp)
forte_test_add_sourcefile_cpp(F_TIME_IN_US_TO_LREAL_tester.cpp F_TIME_IN_NS_TO_LREAL_tester.cpp)