# *******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, Profactor GmbH, 2018 TU Vienna/ACIN This
# program and the accompanying materials are made available under the terms of
# the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger, Matthias Plasch *   - initial API and
# implementation and/or initial documentation *   Martin Melik-Merkumians -
# removing illegal casts
# *******************************************************************************/

# ############################################################################
# WORD_TO_xxx Conversion
# ############################################################################

target_sources(forte-modules-IEC61131-3 PRIVATE
            F_WORD_TO_BYTE_fbt.h
            F_WORD_TO_BYTE_fbt.cpp
            F_WORD_TO_DINT_fbt.h
            F_WORD_TO_DINT_fbt.cpp
            F_WORD_TO_DWORD_fbt.h
            F_WORD_TO_DWORD_fbt.cpp
            F_WORD_TO_INT_fbt.h
            F_WORD_TO_INT_fbt.cpp
            F_WORD_TO_SINT_fbt.h
            F_WORD_TO_SINT_fbt.cpp
            F_WORD_TO_UDINT_fbt.h
            F_WORD_TO_UDINT_fbt.cpp
            F_WORD_TO_UINT_fbt.h
            F_WORD_TO_UINT_fbt.cpp
            F_WORD_TO_USINT_fbt.h
            F_WORD_TO_USINT_fbt.cpp
            F_WORD_AS_STRING_fbt.h
            F_WORD_AS_STRING_fbt.cpp
            F_WORD_TO_LINT_fbt.h
            F_WORD_TO_LINT_fbt.cpp
            F_WORD_TO_LWORD_fbt.h
            F_WORD_TO_LWORD_fbt.cpp
            F_WORD_TO_ULINT_fbt.h
            F_WORD_TO_ULINT_fbt.cpp
            F_WORD_AS_WSTRING_fbt.h
            F_WORD_AS_WSTRING_fbt.cpp
)
