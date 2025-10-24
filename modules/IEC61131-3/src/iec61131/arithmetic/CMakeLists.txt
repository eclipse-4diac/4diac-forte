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

target_sources(forte-iec61131-3 PRIVATE
        F_ADD_DT_TIME_fbt.cpp
        F_ADD_fbt.cpp
        F_ADD_TOD_TIME_fbt.cpp
        F_DIV_fbt.cpp
        F_DIVTIME_fbt.cpp
        F_EXPT_fbt.cpp
        F_MOD_fbt.cpp
        F_MUL_fbt.cpp
        F_MULTIME_fbt.cpp
        F_SUB_DATE_DATE_fbt.cpp
        F_SUB_DT_DT_fbt.cpp
        F_SUB_DT_TIME_fbt.cpp
        F_SUB_fbt.cpp
        F_SUB_TOD_TIME_fbt.cpp
        F_SUB_TOD_TOD_fbt.cpp
        F_TRUNC_fbt.cpp
        GEN_ADD_fbt.cpp
)
