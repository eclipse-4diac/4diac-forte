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

#############################################################################
# Convert FB
#############################################################################
forte_add_sourcefile_hcpp( BOOL2BOOL BYTE2BYTE )
forte_add_sourcefile_hcpp( WORD2WORD LWORD2LWORD_fbt DWORD2DWORD )
forte_add_sourcefile_hcpp( CHAR2CHAR_fbt WCHAR2WCHAR_fbt )
forte_add_sourcefile_hcpp( INT2INT SINT2SINT DINT2DINT LINT2LINT_fbt )
forte_add_sourcefile_hcpp( USINT2USINT UINT2UINT UDINT2UDINT ULINT2ULINT_fbt )
forte_add_sourcefile_hcpp( STRING2STRING  WSTRING2WSTRING )
forte_add_sourcefile_hcpp( TIME2TIME LTIME2LTIME_fbt )
forte_add_sourcefile_hcpp( DATE2DATE_fbt LDATE2LDATE_fbt )
forte_add_sourcefile_hcpp( DT2DT_fbt LDT2LDT_fbt )
forte_add_sourcefile_hcpp( TOD2TOD_fbt LTOD2LTOD_fbt )
forte_add_sourcefile_hcpp( REAL2REAL LREAL2LREAL )
forte_add_sourcefile_hcpp( GEN_STRUCT_MUX GEN_STRUCT_DEMUX )

