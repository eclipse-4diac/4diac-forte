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

target_sources(forte-reconfiguration PUBLIC
        FILE_SET HEADERS
        FILES
        EC_KILL_ELEM_fbt.h
        EC_SET_EVT_fbt.h
        EC_START_ELEM_fbt.h
        EC_STOP_ELEM_fbt.h
        ST_CREATE_CONN_fbt.h
        ST_CREATE_FB_fbt.h
        ST_DEL_CONN_fbt.h
        ST_DEL_FB_fbt.h
        ST_REC_CONN_fbt.h
        ST_SET_PARM_fbt.h
)
