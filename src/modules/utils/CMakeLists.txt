#*******************************************************************************
# Copyright (c) 2011 - 2018 AIT, ACIN, Profactor GmbH, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Thomas Strasser, Alois Zoitl, Matthias Plasch, Ben Schneider
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_module(UTILS OFF "FORTE UTILITY FBs")

#############################################################################
# FORTE UTILITY FBs
#############################################################################

forte_add_sourcefile_hcpp(E_STOPWATCH GetInstancePath_fbt_fbt GetInstancePathAndName_fbt)
forte_add_sourcefile_hcpp(OUT_ANY_CONSOLE_fbt GEN_F_MUX_fbt GEN_CSV_WRITER_fbt)
forte_add_sourcefile_hcpp(GEN_ARRAY2VALUES_fbt GEN_VALUES2ARRAY_fbt GEN_ARRAY2ARRAY_fbt GET_AT_INDEX_fbt SET_AT_INDEX_fbt)
forte_add_sourcefile_hcpp(FB_RANDOM GET_STRUCT_VALUE_fbt SET_STRUCT_VALUE_fbt)

forte_add_sourcefile_hcpp(STEST_END_fbt)
forte_add_sourcefile_hcpp(TEST_CONDITION_fbt)
forte_add_subdirectory(Selection)
