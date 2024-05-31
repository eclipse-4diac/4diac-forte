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
forte_add_sourcefile_hcpp(F_WSTRING_AS_UINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_BOOL)
forte_add_sourcefile_hcpp(F_WSTRING_AS_BYTE)
forte_add_sourcefile_hcpp(F_WSTRING_AS_DINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_DWORD)
forte_add_sourcefile_hcpp(F_WSTRING_AS_INT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_SINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_UDINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_USINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_WORD)
forte_add_sourcefile_hcpp(F_WSTRING_TO_STRING)
forte_add_sourcefile_hcpp(F_WSTRING_AS_TIME)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LWORD)
forte_add_sourcefile_hcpp(F_WSTRING_AS_ULINT)
forte_add_sourcefile_hcpp(F_WSTRING_AS_REAL)
forte_add_sourcefile_hcpp(F_WSTRING_AS_LREAL)
