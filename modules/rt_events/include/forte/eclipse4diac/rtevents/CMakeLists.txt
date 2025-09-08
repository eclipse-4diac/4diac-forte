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

target_sources(forte-rtevents PUBLIC
        FILE_SET HEADERS
        FILES
        RT_Bridge_fbt.h
        RT_E_CYCLE_fbt.h
        RT_E_DELAY_fbt.h
        RT_E_DEMUX_fbt.h
        RT_E_EC_COUPLER_fbt.h
        RT_E_F_TRIG_fbt.h
        RT_E_MERGE_fbt.h
        RT_E_PERMIT_fbt.h
        RT_E_R_TRIG_fbt.h
        RT_E_REND_fbt.h
        RT_E_SELECT_fbt.h
        RT_E_SPLIT_fbt.h
        RT_E_SWITCH_fbt.h
        RT_E_TRAIN_fbt.h
        rtesingle.h
)
