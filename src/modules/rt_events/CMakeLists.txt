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

#############################################################################
# RT_Events FB
#############################################################################
forte_add_sourcefile_hcpp(RT_E_CYCLE  RT_E_DEMUX       RT_E_F_TRIG  RT_E_PERMIT  RT_E_R_TRIG  RT_E_SPLIT)
forte_add_sourcefile_hcpp(RT_E_DELAY  RT_E_EC_COUPLER  RT_E_MERGE   RT_E_REND    RT_E_SELECT  RT_E_SWITCH RT_E_TRAIN)
