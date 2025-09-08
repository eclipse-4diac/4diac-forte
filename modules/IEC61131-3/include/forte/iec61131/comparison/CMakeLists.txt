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
        F_EQ_fbt.h
        F_GE_fbt.h
        F_GT_fbt.h
        F_LE_fbt.h
        F_LT_fbt.h
        F_NE_fbt.h
)
