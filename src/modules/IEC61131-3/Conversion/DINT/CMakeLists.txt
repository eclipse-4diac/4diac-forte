#*******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, Profactor GmbH, 2018 TU Vienna/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger, Matthias Plasch
# *   - initial API and implementation and/or initial documentation
# *   Martin Melik-Merkumians - removing illegal casts
# *******************************************************************************/
#############################################################################
# DINT_TO_xxx Conversion
#############################################################################
forte_add_sourcefile_hcpp(F_DINT_TO_BYTE)
forte_add_sourcefile_hcpp(F_DINT_TO_DWORD)
forte_add_sourcefile_hcpp(F_DINT_TO_INT)
forte_add_sourcefile_hcpp(F_DINT_TO_SINT)
forte_add_sourcefile_hcpp(F_DINT_TO_UINT)
forte_add_sourcefile_hcpp(F_DINT_TO_UDINT)
forte_add_sourcefile_hcpp(F_DINT_TO_USINT)
forte_add_sourcefile_hcpp(F_DINT_TO_WORD)
forte_add_sourcefile_hcpp(F_DINT_TO_STRING)

if(FORTE_USE_64BIT_DATATYPES)   
  forte_add_sourcefile_hcpp(F_DINT_TO_LINT)
  forte_add_sourcefile_hcpp(F_DINT_TO_LWORD)
  forte_add_sourcefile_hcpp(F_DINT_TO_ULINT)
endif(FORTE_USE_64BIT_DATATYPES)

if(FORTE_USE_REAL_DATATYPE)
  forte_add_sourcefile_hcpp(F_DINT_TO_REAL)
endif(FORTE_USE_REAL_DATATYPE)

if(FORTE_USE_LREAL_DATATYPE)
  forte_add_sourcefile_hcpp(F_DINT_TO_LREAL)
endif(FORTE_USE_LREAL_DATATYPE)

if(FORTE_USE_WSTRING_DATATYPE)
  forte_add_sourcefile_hcpp(F_DINT_TO_WSTRING)
endif(FORTE_USE_WSTRING_DATATYPE)