#*******************************************************************************
# Copyright (c) 2010 - 2014 ACIN, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl, Monika Wenger
# *     - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Event FB
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCE_GROUP ${SOURCE_GROUP}\\events)


forte_add_sourcefile_hcpp(E_CTU E_CTD E_CTUD  E_DELAY  E_D_FF    E_MERGE)
forte_add_sourcefile_hcpp( E_REND   E_RS     E_SELECT  E_SR)
forte_add_sourcefile_hcpp(E_CYCLE   E_DEMUX  E_F_TRIG  E_PERMIT) 
forte_add_sourcefile_hcpp(E_RDELAY   E_RESTART E_R_TRIG E_SPLIT   E_SWITCH)
forte_add_sourcefile_hcpp(GEN_E_DEMUX GEN_E_MUX)
forte_add_sourcefile_hcpp(ATimeOut E_TimeOut ARTimeOut E_RTimeOut E_T_FF)
forte_add_sourcefile_hcpp(E_TRAIN)
