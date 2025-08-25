# *******************************************************************************
# Copyright (c) 2014 Profactor GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Matthias Plasch - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
forte_register_module(RECONFIGURATION OFF "FORTE Online Reconfiguration")

# ############################################################################
# FORTE Reconfiguration FBs
# ############################################################################
add_library(forte-modules-reconfiguration OBJECT
            EC_KILL_ELEM_fbt.h
            EC_KILL_ELEM_fbt.cpp
            EC_SET_EVT_fbt.h
            EC_SET_EVT_fbt.cpp
            EC_START_ELEM_fbt.h
            EC_START_ELEM_fbt.cpp
            EC_STOP_ELEM_fbt.h
            EC_STOP_ELEM_fbt.cpp
            ST_CREATE_CONN_fbt.h
            ST_CREATE_CONN_fbt.cpp
            ST_CREATE_FB_fbt.h
            ST_CREATE_FB_fbt.cpp
            ST_DEL_CONN_fbt.h
            ST_DEL_CONN_fbt.cpp
            ST_DEL_FB_fbt.h
            ST_DEL_FB_fbt.cpp
            ST_REC_CONN_fbt.h
            ST_REC_CONN_fbt.cpp
            ST_SET_PARM_fbt.h
            ST_SET_PARM_fbt.cpp
)
target_link_libraries(forte-modules-reconfiguration PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-modules-reconfiguration)
