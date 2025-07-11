# *******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, 2018 TU Vienna/ACIN This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation *   Martin Melik Merkumians - Removes invalid casts
# *******************************************************************************/

# ############################################################################
# BOOL_TO_xxx Conversion
# ############################################################################

add_library(forte-modules-IEC61131-3-Conversion-BOOL OBJECT
            F_BOOL_TO_BYTE_fbt.h
            F_BOOL_TO_BYTE_fbt.cpp
            F_BOOL_TO_DINT_fbt.h
            F_BOOL_TO_DINT_fbt.cpp
            F_BOOL_TO_DWORD_fbt.h
            F_BOOL_TO_DWORD_fbt.cpp
            F_BOOL_TO_INT_fbt.h
            F_BOOL_TO_INT_fbt.cpp
            F_BOOL_TO_SINT_fbt.h
            F_BOOL_TO_SINT_fbt.cpp
            F_BOOL_TO_UDINT_fbt.h
            F_BOOL_TO_UDINT_fbt.cpp
            F_BOOL_TO_UINT_fbt.h
            F_BOOL_TO_UINT_fbt.cpp
            F_BOOL_TO_USINT_fbt.h
            F_BOOL_TO_USINT_fbt.cpp
            F_BOOL_TO_WORD_fbt.h
            F_BOOL_TO_WORD_fbt.cpp
            F_BOOL_AS_STRING_fbt.h
            F_BOOL_AS_STRING_fbt.cpp
            F_BOOL_TO_LINT_fbt.h
            F_BOOL_TO_LINT_fbt.cpp
            F_BOOL_TO_LWORD_fbt.h
            F_BOOL_TO_LWORD_fbt.cpp
            F_BOOL_TO_ULINT_fbt.h
            F_BOOL_TO_ULINT_fbt.cpp
            F_BOOL_AS_WSTRING_fbt.h
            F_BOOL_AS_WSTRING_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Conversion-BOOL)
