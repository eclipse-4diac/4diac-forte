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
forte_add_sourcefile_hcpp( BOOL2BOOL     BYTE2BYTE   DINT2DINT         DWORD2DWORD )
forte_add_sourcefile_hcpp( INT2INT       SINT2SINT )
forte_add_sourcefile_hcpp( STRING2STRING  TIME2TIME   UINT2UINT         UDINT2UDINT )
forte_add_sourcefile_hcpp( USINT2USINT     WORD2WORD     WSTRING2WSTRING )
forte_add_sourcefile_hcpp( GEN_STRUCT_MUX GEN_STRUCT_DEMUX )

if(FORTE_USE_REAL_DATATYPE)
  forte_add_sourcefile_hcpp(REAL2REAL)
endif(FORTE_USE_REAL_DATATYPE)

if(FORTE_USE_LREAL_DATATYPE)
  forte_add_sourcefile_hcpp(LREAL2LREAL)
endif(FORTE_USE_LREAL_DATATYPE)

