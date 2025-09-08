#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-convert PRIVATE
        BOOL2BOOL_fbt.cpp
        BYTE2BYTE_fbt.cpp
        DINT2DINT_fbt.cpp
        DWORD2DWORD_fbt.cpp
        GEN_STRUCT_DEMUX_fbt.cpp
        GEN_STRUCT_MUX_fbt.cpp
        INT2INT_fbt.cpp
        LREAL2LREAL_fbt.cpp
        REAL2REAL_fbt.cpp
        SINT2SINT_fbt.cpp
        STRING2STRING_fbt.cpp
        TIME2TIME_fbt.cpp
        UDINT2UDINT_fbt.cpp
        UINT2UINT_fbt.cpp
        USINT2USINT_fbt.cpp
        WORD2WORD_fbt.cpp
        WSTRING2WSTRING_fbt.cpp
)
