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
        F_AND_fbt.h
        F_NOT_fbt.h
        F_OR_fbt.h
        F_ROL_fbt.h
        F_ROR_fbt.h
        F_SHL_fbt.h
        F_SHR_fbt.h
        F_XOR_fbt.h
        GEN_AND_fbt.h
        GEN_OR_fbt.h
        GEN_XOR_fbt.h
        genbitbase_fbt.h
)
