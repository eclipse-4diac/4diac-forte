# *******************************************************************************
# Copyright (c) 2011 - 2014 ACIN, Profactor GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger, Matthias Plasch *   - initial API and
# implementation and/or initial documentation
# *******************************************************************************/

# ############################################################################
# Arithmetic
# ############################################################################

target_sources(forte-modules-IEC61131-3 PRIVATE
            F_ADD_fbt.h
            F_ADD_fbt.cpp
            F_SUB_fbt.h
            F_SUB_fbt.cpp
            F_MUL_fbt.h
            F_MUL_fbt.cpp
            F_DIV_fbt.h
            F_DIV_fbt.cpp
            F_MOD_fbt.h
            F_MOD_fbt.cpp
            F_EXPT_fbt.h
            F_EXPT_fbt.cpp
            F_MOVE_fbt.h
            F_MOVE_fbt.cpp
            GEN_FORTE_F_MOVE_fbt.h
            GEN_FORTE_F_MOVE_fbt.cpp
            F_MULTIME_fbt.h
            F_MULTIME_fbt.cpp
            F_DIVTIME_fbt.h
            F_DIVTIME_fbt.cpp
            GEN_ADD_fbt.h
            GEN_ADD_fbt.cpp
            F_TRUNC_fbt.h
            F_TRUNC_fbt.cpp
            F_ADD_DT_TIME_fbt.h
            F_ADD_DT_TIME_fbt.cpp
            F_ADD_TOD_TIME_fbt.h
            F_ADD_TOD_TIME_fbt.cpp
            F_SUB_DT_TIME_fbt.h
            F_SUB_DT_TIME_fbt.cpp
            F_SUB_TOD_TIME_fbt.h
            F_SUB_TOD_TIME_fbt.cpp
            F_SUB_DT_DT_fbt.h
            F_SUB_DT_DT_fbt.cpp
            F_SUB_DATE_DATE_fbt.h
            F_SUB_DATE_DATE_fbt.cpp
            F_SUB_TOD_TOD_fbt.h
            F_SUB_TOD_TOD_fbt.cpp
)
