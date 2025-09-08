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

target_sources(forte-convert PUBLIC
        FILE_SET HEADERS
        FILES
        BOOL2BOOL_fbt.h
        BYTE2BYTE_fbt.h
        DINT2DINT_fbt.h
        DWORD2DWORD_fbt.h
        GEN_STRUCT_DEMUX_fbt.h
        GEN_STRUCT_MUX_fbt.h
        INT2INT_fbt.h
        LREAL2LREAL_fbt.h
        REAL2REAL_fbt.h
        SINT2SINT_fbt.h
        STRING2STRING_fbt.h
        TIME2TIME_fbt.h
        UDINT2UDINT_fbt.h
        UINT2UINT_fbt.h
        USINT2USINT_fbt.h
        WORD2WORD_fbt.h
        WSTRING2WSTRING_fbt.h
)
