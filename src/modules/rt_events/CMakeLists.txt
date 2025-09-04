# *******************************************************************************
# Copyright (c) 2010, 2011 ACIN, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
forte_register_module(RT_Events OFF "Real-Time Event Funktion blocks")

# ############################################################################
# RT_Events FB
# ############################################################################

add_library(forte-modules-rt_events OBJECT
            RT_E_CYCLE_fbt.h
            RT_E_CYCLE_fbt.cpp
            RT_E_DEMUX_fbt.h
            RT_E_DEMUX_fbt.cpp
            RT_E_F_TRIG_fbt.h
            RT_E_F_TRIG_fbt.cpp
            RT_E_PERMIT_fbt.h
            RT_E_PERMIT_fbt.cpp
            RT_E_R_TRIG_fbt.h
            RT_E_R_TRIG_fbt.cpp
            RT_E_SPLIT_fbt.h
            RT_E_SPLIT_fbt.cpp
            RT_E_DELAY_fbt.h
            RT_E_DELAY_fbt.cpp
            RT_E_EC_COUPLER_fbt.h
            RT_E_EC_COUPLER_fbt.cpp
            RT_E_MERGE_fbt.h
            RT_E_MERGE_fbt.cpp
            RT_E_REND_fbt.h
            RT_E_REND_fbt.cpp
            RT_E_SELECT_fbt.h
            RT_E_SELECT_fbt.cpp
            RT_E_SWITCH_fbt.h
            RT_E_SWITCH_fbt.cpp
            RT_E_TRAIN_fbt.h
            RT_E_TRAIN_fbt.cpp
            RT_Bridge_fbt.h
            RT_Bridge_fbt.cpp
)
target_link_libraries(forte-modules-rt_events PUBLIC forte-core forte-events)
target_link_libraries(forte PUBLIC forte-modules-rt_events)
