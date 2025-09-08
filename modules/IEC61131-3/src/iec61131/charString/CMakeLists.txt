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
        F_CONCAT_DATE_TOD_fbt.cpp
        F_CONCAT_fbt.cpp
        F_DELETE_fbt.cpp
        F_FIND_fbt.cpp
        F_INSERT_fbt.cpp
        F_LEFT_fbt.cpp
        F_LEN_fbt.cpp
        F_MID_fbt.cpp
        F_REPLACE_fbt.cpp
        F_RIGHT_fbt.cpp
)
