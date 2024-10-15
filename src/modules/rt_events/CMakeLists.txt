#*******************************************************************************
# Copyright (c) 2010, 2011 ACIN, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_module(RT_Events OFF "Real-Time Event Funktion blocks")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# RT_Events FB
#############################################################################
forte_add_sourcefile_hcpp(RT_E_CYCLE_fbt  RT_E_DEMUX_fbt       RT_E_F_TRIG_fbt  RT_E_PERMIT_fbt  RT_E_R_TRIG_fbt  RT_E_SPLIT_fbt)
forte_add_sourcefile_hcpp(RT_E_DELAY_fbt  RT_E_EC_COUPLER_fbt  RT_E_MERGE_fbt   RT_E_REND_fbt    RT_E_SELECT_fbt  RT_E_SWITCH_fbt RT_E_TRAIN_fbt)
forte_add_sourcefile_hcpp(RT_Bridge_fbt)
