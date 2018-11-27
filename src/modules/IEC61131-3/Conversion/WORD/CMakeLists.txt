#*******************************************************************************
# * Copyright (c) 2012 - 2013 ACIN, Profactor GmbH, 2018 TU Vienna/ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Monika Wenger, Matthias Plasch
# *   - initial API and implementation and/or initial documentation
# *   Martin Melik-Merkumians - removing illegal casts
# *******************************************************************************/
#############################################################################
# WORD_TO_xxx Conversion
#############################################################################
forte_add_sourcefile_hcpp(F_WORD_TO_BYTE)
forte_add_sourcefile_hcpp(F_WORD_TO_DINT)
forte_add_sourcefile_hcpp(F_WORD_TO_DWORD)
forte_add_sourcefile_hcpp(F_WORD_TO_INT)
forte_add_sourcefile_hcpp(F_WORD_TO_SINT)
forte_add_sourcefile_hcpp(F_WORD_TO_UDINT)
forte_add_sourcefile_hcpp(F_WORD_TO_UINT)
forte_add_sourcefile_hcpp(F_WORD_TO_USINT)
forte_add_sourcefile_hcpp(F_WORD_TO_STRING)

if(FORTE_USE_64BIT_DATATYPES)       
  forte_add_sourcefile_hcpp(F_WORD_TO_LINT)
  forte_add_sourcefile_hcpp(F_WORD_TO_LWORD)
  forte_add_sourcefile_hcpp(F_WORD_TO_ULINT)
endif(FORTE_USE_64BIT_DATATYPES)

if(FORTE_USE_WSTRING_DATATYPE) 
  forte_add_sourcefile_hcpp(F_WORD_TO_WSTRING)
endif(FORTE_USE_WSTRING_DATATYPE) 