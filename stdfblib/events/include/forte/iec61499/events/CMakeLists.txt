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

target_sources(forte-events PUBLIC
        FILE_SET HEADERS
        FILES
        ARTimeOut_adp.h
        ATimeOut_adp.h
        E_CTD_fbt.h
        E_CTU_fbt.h
        E_CTUD_fbt.h
        E_CTUD_UDINT_fbt.h
        E_CYCLE_fbt.h
        E_D_FF_fbt.h
        E_DELAY_fbt.h
        E_DEMUX_fbt.h
        E_F_TRIG_fbt.h
        E_MERGE_fbt.h
        E_N_TABLE_fbt.h
        E_PERMIT_fbt.h
        E_R_TRIG_fbt.h
        E_RDELAY_fbt.h
        E_REND_fbt.h
        E_RESTART_fbt.h
        E_RS_fbt.h
        E_RTimeOut_fbt.h
        E_SELECT_fbt.h
        E_SPLIT_fbt.h
        E_SR_fbt.h
        E_SWITCH_fbt.h
        E_T_FF_fbt.h
        E_T_FF_SR_fbt.h
        E_TABLE_CTRL_fbt.h
        E_TABLE_fbt.h
        E_TimeOut_fbt.h
        E_TRAIN_fbt.h
        E_TRIG_fbt.h
        GEN_E_DEMUX_fbt.h
        GEN_E_MUX_fbt.h
        GEN_E_MERGE_fbt.h
        GEN_E_SPLIT_fbt.h
        timedfb.h
)
add_subdirectory(timers)
