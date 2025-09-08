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
        F_CONCAT_DATE_TOD_fbt.h
        F_CONCAT_fbt.h
        F_DELETE_fbt.h
        F_FIND_fbt.h
        F_INSERT_fbt.h
        F_LEFT_fbt.h
        F_LEN_fbt.h
        F_MID_fbt.h
        F_REPLACE_fbt.h
        F_RIGHT_fbt.h
)
