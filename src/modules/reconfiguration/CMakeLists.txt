#*******************************************************************************
# Copyright (c) 2014 Profactor GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Matthias Plasch - initial API and implementation and/or initial documentation
# *******************************************************************************/


forte_add_module(RECONFIGURATION OFF "FORTE Online Reconfiguration")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# FORTE Reconfiguration FBs
#############################################################################
forte_add_sourcefile_hcpp(EC_KILL_ELEM_fbt EC_SET_EVT_fbt EC_START_ELEM_fbt EC_STOP_ELEM_fbt)
forte_add_sourcefile_hcpp(ST_CREATE_CONN_fbt ST_CREATE_FB_fbt ST_DEL_CONN_fbt ST_DEL_FB_fbt ST_REC_CONN_fbt ST_SET_PARM_fbt)