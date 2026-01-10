#############################################################################
# Copyright (c) 2026 Martin Erich Jobst
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

target_sources(forte-core PRIVATE
        endianess.cpp
        func_ADD_DT_TIME.cpp
        func_ADD_LDT_LTIME.cpp
        func_ADD_LTIME.cpp
        func_ADD_LTOD_LTIME.cpp
        func_ADD_TIME.cpp
        func_ADD_TOD_TIME.cpp
        func_CONCAT_DATE.cpp
        func_CONCAT_DATE_TOD.cpp
        func_CONCAT_DT.cpp
        func_CONCAT_LDATE.cpp
        func_CONCAT_LDATE_LTOD.cpp
        func_CONCAT_LDT.cpp
        func_CONCAT_LTOD.cpp
        func_CONCAT_TOD.cpp
        func_DT_TO_DATE.cpp
        func_DT_TO_TOD.cpp
        func_FROM_BIG_ENDIAN.cpp
        func_FROM_LITTLE_ENDIAN.cpp
        func_LDT_TO_LDATE.cpp
        func_LDT_TO_LTOD.cpp
        func_LREAL_TRUNC_DINT.cpp
        func_LREAL_TRUNC_INT.cpp
        func_LREAL_TRUNC_LINT.cpp
        func_LREAL_TRUNC_SINT.cpp
        func_LREAL_TRUNC_UDINT.cpp
        func_LREAL_TRUNC_UINT.cpp
        func_LREAL_TRUNC_ULINT.cpp
        func_LREAL_TRUNC_USINT.cpp
        func_NOW.cpp
        func_NOW_MONOTONIC.cpp
        func_REAL_TRUNC_DINT.cpp
        func_REAL_TRUNC_INT.cpp
        func_REAL_TRUNC_LINT.cpp
        func_REAL_TRUNC_SINT.cpp
        func_REAL_TRUNC_UDINT.cpp
        func_REAL_TRUNC_UINT.cpp
        func_REAL_TRUNC_ULINT.cpp
        func_REAL_TRUNC_USINT.cpp
        func_SPLIT_DATE.cpp
        func_SPLIT_DT.cpp
        func_SPLIT_LDATE.cpp
        func_SPLIT_LDT.cpp
        func_SPLIT_LTOD.cpp
        func_SPLIT_TOD.cpp
        func_SUB_DATE_DATE.cpp
        func_SUB_DT_DT.cpp
        func_SUB_DT_TIME.cpp
        func_SUB_LDATE_LDATE.cpp
        func_SUB_LDT_LDT.cpp
        func_SUB_LDT_LTIME.cpp
        func_SUB_LTIME.cpp
        func_SUB_LTOD_LTIME.cpp
        func_SUB_LTOD_LTOD.cpp
        func_SUB_TIME.cpp
        func_SUB_TOD_TIME.cpp
        func_SUB_TOD_TOD.cpp
        func_TO_BIG_ENDIAN.cpp
        func_TO_LITTLE_ENDIAN.cpp
        func_TRUNC_DINT.cpp
        func_TRUNC_INT.cpp
        func_TRUNC_LINT.cpp
        func_TRUNC_SINT.cpp
        func_TRUNC_UDINT.cpp
        func_TRUNC_UINT.cpp
        func_TRUNC_ULINT.cpp
        func_TRUNC_USINT.cpp
)
