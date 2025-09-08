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
        F_EQ_fbt.cpp
        F_GE_fbt.cpp
        F_GT_fbt.cpp
        F_LE_fbt.cpp
        F_LT_fbt.cpp
        F_NE_fbt.cpp
)
