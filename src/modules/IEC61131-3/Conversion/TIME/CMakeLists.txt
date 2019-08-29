#*******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, 2018 TU Vienna/ACIN, 2019 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger
# *   - initial API and implementation and/or initial documentation
# *   Martin Melik-Merkumians - removing illegal casts
# *   Martin Melik-Merkumians - adding TIME IN RESOLUTION TO FBs
# *******************************************************************************/
#############################################################################
# TIME_TO_xxx Conversion
#############################################################################
forte_add_sourcefile_hcpp(F_TIME_TO_STRING)

if(FORTE_USE_64BIT_DATATYPES)
  forte_add_sourcefile_hcpp(F_TIME_IN_S_TO_LINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_MS_TO_LINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_US_TO_LINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_NS_TO_LINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_S_TO_ULINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_MS_TO_ULINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_US_TO_ULINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_NS_TO_ULINT)
  forte_add_sourcefile_hcpp(F_TIME_IN_S_TO_LREAL)
  forte_add_sourcefile_hcpp(F_TIME_IN_MS_TO_LREAL)
  forte_add_sourcefile_hcpp(F_TIME_IN_US_TO_LREAL)
  forte_add_sourcefile_hcpp(F_TIME_IN_NS_TO_LREAL)
endif()

if(FORTE_USE_WSTRING_DATATYPE)
  forte_add_sourcefile_hcpp(F_TIME_TO_WSTRING)
endif(FORTE_USE_WSTRING_DATATYPE)