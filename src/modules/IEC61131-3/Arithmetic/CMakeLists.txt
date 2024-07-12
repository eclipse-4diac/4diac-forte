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
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# Arithmetic
#############################################################################
forte_add_sourcefile_hcpp(F_ADD_fbt)
forte_add_sourcefile_hcpp(F_SUB_fbt)
forte_add_sourcefile_hcpp(F_MUL_fbt)
forte_add_sourcefile_hcpp(F_DIV_fbt)
forte_add_sourcefile_hcpp(F_MOD_fbt)
forte_add_sourcefile_hcpp(F_EXPT_fbt)
forte_add_sourcefile_hcpp(F_MOVE_fbt)
forte_add_sourcefile_hcpp(GEN_F_MOVE_fct)
forte_add_sourcefile_hcpp(F_MULTIME_fbt)
forte_add_sourcefile_hcpp(F_DIVTIME_fbt)
forte_add_sourcefile_hcpp(GEN_ADD_fbt)
forte_add_sourcefile_hcpp(F_TRUNC_fbt)
forte_add_sourcefile_hcpp(F_ADD_DT_TIME_fbt)
forte_add_sourcefile_hcpp(F_ADD_TOD_TIME_fbt)
forte_add_sourcefile_hcpp(F_SUB_DT_TIME_fbt)
forte_add_sourcefile_hcpp(F_SUB_TOD_TIME_fbt) 
forte_add_sourcefile_hcpp(F_SUB_DT_DT_fbt)
forte_add_sourcefile_hcpp(F_SUB_DATE_DATE_fbt)
forte_add_sourcefile_hcpp(F_SUB_TOD_TOD_fbt)
