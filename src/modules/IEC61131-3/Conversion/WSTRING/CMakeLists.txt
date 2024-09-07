#*******************************************************************************
# Copyright (c) 2012 - 2013 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# WSTRING_AS_xxx Conversion
#############################################################################
forte_add_sourcefile_hcpp(F_WSTRING_AS_UINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_BOOL_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_BYTE_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_DINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_DWORD_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_INT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_SINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_UDINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_USINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_WORD_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_TO_STRING_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_TIME_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LWORD_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_ULINT_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_REAL_fbt)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LREAL_fbt)
