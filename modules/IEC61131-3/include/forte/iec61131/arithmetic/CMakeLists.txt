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

target_sources(forte-iec61131-3 PUBLIC
        FILE_SET HEADERS
        FILES
        F_ADD_DT_TIME_fbt.h
        F_ADD_fbt.h
        F_ADD_TOD_TIME_fbt.h
        F_DIV_fbt.h
        F_DIVTIME_fbt.h
        F_EXPT_fbt.h
        F_MOD_fbt.h
        F_MUL_fbt.h
        F_MULTIME_fbt.h
        F_SUB_DATE_DATE_fbt.h
        F_SUB_DT_DT_fbt.h
        F_SUB_DT_TIME_fbt.h
        F_SUB_fbt.h
        F_SUB_TOD_TIME_fbt.h
        F_SUB_TOD_TOD_fbt.h
        F_TRUNC_fbt.h
        GEN_ADD_fbt.h
)
