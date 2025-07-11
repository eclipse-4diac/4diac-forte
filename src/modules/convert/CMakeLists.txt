# *******************************************************************************
# Copyright (c) 2010, 2011 Profactor GmbH, ACIN This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Micheal Hofmann, Alois Zoitl *   - initial API and
# implementation and/or initial documentation
# *******************************************************************************/
forte_register_module(CONVERT OFF "Convert Function Blocks")

# ############################################################################
# Convert FB
# ############################################################################

add_library(forte-modules-convert OBJECT
            BOOL2BOOL_fbt.h
            BOOL2BOOL_fbt.cpp
            BYTE2BYTE_fbt.h
            BYTE2BYTE_fbt.cpp
            DINT2DINT_fbt.h
            DINT2DINT_fbt.cpp
            DWORD2DWORD_fbt.h
            DWORD2DWORD_fbt.cpp
            INT2INT_fbt.h
            INT2INT_fbt.cpp
            SINT2SINT_fbt.h
            SINT2SINT_fbt.cpp
            STRING2STRING_fbt.h
            STRING2STRING_fbt.cpp
            TIME2TIME_fbt.h
            TIME2TIME_fbt.cpp
            UINT2UINT_fbt.h
            UINT2UINT_fbt.cpp
            UDINT2UDINT_fbt.h
            UDINT2UDINT_fbt.cpp
            USINT2USINT_fbt.h
            USINT2USINT_fbt.cpp
            WORD2WORD_fbt.h
            WORD2WORD_fbt.cpp
            WSTRING2WSTRING_fbt.h
            WSTRING2WSTRING_fbt.cpp
            GEN_STRUCT_MUX_fbt.h
            GEN_STRUCT_MUX_fbt.cpp
            GEN_STRUCT_DEMUX_fbt.h
            GEN_STRUCT_DEMUX_fbt.cpp
            REAL2REAL_fbt.h
            REAL2REAL_fbt.cpp
            LREAL2LREAL_fbt.h
            LREAL2LREAL_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-convert)
