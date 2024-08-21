#*******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, Profactor GmbH
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
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# LWORD_TO_xxx Conversion
#############################################################################      
forte_add_sourcefile_hcpp(F_LWORD_TO_BYTE_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_DINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_DWORD_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_INT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_LINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_SINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_UDINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_UINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_ULINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_USINT_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_WORD_fbt)
forte_add_sourcefile_hcpp(F_LWORD_AS_STRING_fbt)
forte_add_sourcefile_hcpp(F_LWORD_TO_LREAL_fbt)

if(FORTE_USE_WSTRING_DATATYPE)
  forte_add_sourcefile_hcpp(F_LWORD_AS_WSTRING_fbt)
endif(FORTE_USE_WSTRING_DATATYPE)
