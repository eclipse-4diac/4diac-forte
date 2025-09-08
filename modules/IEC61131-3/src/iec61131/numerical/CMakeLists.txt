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
        F_ABS_fbt.cpp
        F_ACOS_fbt.cpp
        F_ASIN_fbt.cpp
        F_ATAN_fbt.cpp
        F_COS_fbt.cpp
        F_EXP_fbt.cpp
        F_LN_fbt.cpp
        F_LOG_fbt.cpp
        F_SIN_fbt.cpp
        F_SQRT_fbt.cpp
        F_TAN_fbt.cpp
)
