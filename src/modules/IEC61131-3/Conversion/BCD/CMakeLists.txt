#*******************************************************************************
# Copyright (c) 2013 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Conversion when bit-string variable contains data encoded in BCD
# x_BCD_TO_xx 
# xx_TO_BCD_x
#############################################################################
forte_add_sourcefile_hcpp(F_BYTE_BCD_TO_USINT)
forte_add_sourcefile_hcpp(F_WORD_BCD_TO_UINT)
forte_add_sourcefile_hcpp(F_DWORD_BCD_TO_UDINT)

forte_add_sourcefile_hcpp(F_USINT_TO_BCD_BYTE)
forte_add_sourcefile_hcpp(F_UINT_TO_BCD_WORD)
forte_add_sourcefile_hcpp(F_UDINT_TO_BCD_DWORD)

if(FORTE_USE_64BIT_DATATYPES) 
  forte_add_sourcefile_hcpp(F_LWORD_BCD_TO_ULINT)
  forte_add_sourcefile_hcpp(F_ULINT_TO_BCD_LWORD)
endif(FORTE_USE_64BIT_DATATYPES)