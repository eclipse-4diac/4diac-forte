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

add_library(forte-stdfblib-events OBJECT
	E_CTU_fbt.cpp
	E_CTD_fbt.cpp
	E_CTUD_fbt.cpp
	E_CTUD_UDINT_fbt.cpp
	E_DELAY_fbt.cpp
	E_D_FF_fbt.cpp
	E_MERGE_fbt.cpp
	E_REND_fbt.cpp
	E_RS_fbt.cpp
	E_SELECT_fbt.cpp
	E_SR_fbt.cpp
	E_CYCLE_fbt.cpp
	E_DEMUX_fbt.cpp
	E_F_TRIG_fbt.cpp
	E_PERMIT_fbt.cpp
	E_RDELAY_fbt.cpp
	E_RESTART_fbt.cpp
	E_R_TRIG_fbt.cpp
	E_SPLIT_fbt.cpp
	E_SWITCH_fbt.cpp
	GEN_E_DEMUX_fbt.cpp
	GEN_E_MUX_fbt.cpp
	ATimeOut_adp.cpp
	E_TimeOut_fbt.cpp
	ARTimeOut_adp.cpp
	E_RTimeOut_fbt.cpp
	E_T_FF_fbt.cpp
	E_TRAIN_fbt.cpp
	E_PULSE_fbt.cpp
	E_TP_fbt.cpp
	E_TON_fbt.cpp
	E_TOF_fbt.cpp
	E_TONOF_fbt.cpp
)

target_link_libraries(forte PRIVATE forte-stdfblib-events)
