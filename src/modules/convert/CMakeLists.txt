#*******************************************************************************
# Copyright (c) 2010, 2011 Profactor GmbH, ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Micheal Hofmann, Alois Zoitl  
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_module(CONVERT OFF "Convert Function Blocks")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# Convert FB
#############################################################################
forte_add_sourcefile_hcpp( BOOL2BOOL_fbt     BYTE2BYTE_fbt   DINT2DINT_fbt         DWORD2DWORD_fbt )
forte_add_sourcefile_hcpp( INT2INT_fbt       SINT2SINT_fbt )
forte_add_sourcefile_hcpp( STRING2STRING_fbt  TIME2TIME_fbt   UINT2UINT_fbt         UDINT2UDINT_fbt )
forte_add_sourcefile_hcpp( USINT2USINT_fbt     WORD2WORD_fbt     WSTRING2WSTRING_fbt )
forte_add_sourcefile_hcpp( GEN_STRUCT_MUX_fbt GEN_STRUCT_DEMUX_fbt )
forte_add_sourcefile_hcpp(REAL2REAL_fbt)
forte_add_sourcefile_hcpp(LREAL2LREAL_fbt)


