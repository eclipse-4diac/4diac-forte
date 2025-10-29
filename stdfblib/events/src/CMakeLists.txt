#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-events PRIVATE
        ARTimeOut_adp.cpp
        ATimeOut_adp.cpp
        E_CTD_fbt.cpp
        E_CTU_fbt.cpp
        E_CTUD_fbt.cpp
        E_CTUD_UDINT_fbt.cpp
        E_CYCLE_fbt.cpp
        E_D_FF_fbt.cpp
        E_DELAY_fbt.cpp
        E_DEMUX_fbt.cpp
        E_F_TRIG_fbt.cpp
        E_MERGE_fbt.cpp
        E_N_TABLE_fbt.cpp
        E_PERMIT_fbt.cpp
        E_R_TRIG_fbt.cpp
        E_RDELAY_fbt.cpp
        E_REND_fbt.cpp
        E_RESTART_fbt.cpp
        E_RS_fbt.cpp
        E_RTimeOut_fbt.cpp
        E_SELECT_fbt.cpp
        E_SPLIT_fbt.cpp
        E_SR_fbt.cpp
        E_SWITCH_fbt.cpp
        E_T_FF_fbt.cpp
        E_T_FF_SR_fbt.cpp
        E_TABLE_CTRL_fbt.cpp
        E_TABLE_fbt.cpp
        E_TimeOut_fbt.cpp
        E_TRAIN_fbt.cpp
        E_TRIG_fbt.cpp
        GEN_E_DEMUX_fbt.cpp
        GEN_E_MUX_fbt.cpp
        GEN_E_MERGE_fbt.cpp
        GEN_E_SPLIT_fbt.cpp
        timedfb.cpp
)

add_subdirectory(timers)
