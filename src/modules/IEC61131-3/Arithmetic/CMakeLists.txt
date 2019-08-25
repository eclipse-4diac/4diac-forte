#*******************************************************************************
# Copyright (c) 2011 - 2014 ACIN, Profactor GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger, Matthias Plasch
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Arithmetic
#############################################################################
forte_add_sourcefile_hcpp(F_ADD)
forte_add_sourcefile_hcpp(F_SUB)
forte_add_sourcefile_hcpp(F_MUL)
forte_add_sourcefile_hcpp(F_DIV)
forte_add_sourcefile_hcpp(F_MOD)
forte_add_sourcefile_hcpp(F_EXPT)
forte_add_sourcefile_hcpp(F_MOVE)
forte_add_sourcefile_hcpp(F_MULTIME)
forte_add_sourcefile_hcpp(F_DIVTIME)
forte_add_sourcefile_hcpp(GEN_ADD)
  
if(FORTE_USE_REAL_DATATYPE)
  forte_add_sourcefile_hcpp(F_TRUNC)
endif(FORTE_USE_REAL_DATATYPE)

if(FORTE_USE_64BIT_DATATYPES) 
  forte_add_sourcefile_hcpp(F_ADD_DT_TIME)
  forte_add_sourcefile_hcpp(F_ADD_TOD_TIME)
  forte_add_sourcefile_hcpp(F_SUB_DT_TIME)
  forte_add_sourcefile_hcpp(F_SUB_TOD_TIME) 
  forte_add_sourcefile_hcpp(F_SUB_DT_DT)
  forte_add_sourcefile_hcpp(F_SUB_DATE_DATE)
  forte_add_sourcefile_hcpp(F_SUB_TOD_TOD)
endif(FORTE_USE_64BIT_DATATYPES) 
