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
        F_AND_fbt.cpp
        F_NOT_fbt.cpp
        F_OR_fbt.cpp
        F_ROL_fbt.cpp
        F_ROR_fbt.cpp
        F_SHL_fbt.cpp
        F_SHR_fbt.cpp
        F_XOR_fbt.cpp
        GEN_AND_fbt.cpp
        GEN_OR_fbt.cpp
        GEN_XOR_fbt.cpp
        genbitbase_fbt.cpp
)
