# *******************************************************************************
# Copyright (c) 2012 - 2013 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# WSTRING_AS_xxx Conversion
# ############################################################################

target_sources(forte-modules-IEC61131-3 PRIVATE
            F_WSTRING_AS_UINT_fbt.h
            F_WSTRING_AS_UINT_fbt.cpp
            F_WSTRING_AS_BOOL_fbt.h
            F_WSTRING_AS_BOOL_fbt.cpp
            F_WSTRING_AS_BYTE_fbt.h
            F_WSTRING_AS_BYTE_fbt.cpp
            F_WSTRING_AS_DINT_fbt.h
            F_WSTRING_AS_DINT_fbt.cpp
            F_WSTRING_AS_DWORD_fbt.h
            F_WSTRING_AS_DWORD_fbt.cpp
            F_WSTRING_AS_INT_fbt.h
            F_WSTRING_AS_INT_fbt.cpp
            F_WSTRING_AS_SINT_fbt.h
            F_WSTRING_AS_SINT_fbt.cpp
            F_WSTRING_AS_UDINT_fbt.h
            F_WSTRING_AS_UDINT_fbt.cpp
            F_WSTRING_AS_USINT_fbt.h
            F_WSTRING_AS_USINT_fbt.cpp
            F_WSTRING_AS_WORD_fbt.h
            F_WSTRING_AS_WORD_fbt.cpp
            F_WSTRING_TO_STRING_fbt.h
            F_WSTRING_TO_STRING_fbt.cpp
            F_WSTRING_AS_TIME_fbt.h
            F_WSTRING_AS_TIME_fbt.cpp
            F_WSTRING_AS_LINT_fbt.h
            F_WSTRING_AS_LINT_fbt.cpp
            F_WSTRING_AS_LWORD_fbt.h
            F_WSTRING_AS_LWORD_fbt.cpp
            F_WSTRING_AS_ULINT_fbt.h
            F_WSTRING_AS_ULINT_fbt.cpp
            F_WSTRING_AS_REAL_fbt.h
            F_WSTRING_AS_REAL_fbt.cpp
            F_WSTRING_AS_LREAL_fbt.h
            F_WSTRING_AS_LREAL_fbt.cpp
)
