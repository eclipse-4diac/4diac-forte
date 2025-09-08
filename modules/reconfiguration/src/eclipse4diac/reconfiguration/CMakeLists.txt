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

target_sources(forte-reconfiguration PRIVATE
        EC_KILL_ELEM_fbt.cpp
        EC_SET_EVT_fbt.cpp
        EC_START_ELEM_fbt.cpp
        EC_STOP_ELEM_fbt.cpp
        ST_CREATE_CONN_fbt.cpp
        ST_CREATE_FB_fbt.cpp
        ST_DEL_CONN_fbt.cpp
        ST_DEL_FB_fbt.cpp
        ST_REC_CONN_fbt.cpp
        ST_SET_PARM_fbt.cpp
)
