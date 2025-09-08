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
        F_ABS_fbt.h
        F_ACOS_fbt.h
        F_ASIN_fbt.h
        F_ATAN_fbt.h
        F_COS_fbt.h
        F_EXP_fbt.h
        F_LN_fbt.h
        F_LOG_fbt.h
        F_SIN_fbt.h
        F_SQRT_fbt.h
        F_TAN_fbt.h
)
