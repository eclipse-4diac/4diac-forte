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
        FB_CTD_DINT_fbt.h
        FB_CTD_fbt.h
        FB_CTD_LINT_fbt.h
        FB_CTD_UDINT_fbt.h
        FB_CTD_ULINT_fbt.h
        FB_CTU_DINT_fbt.h
        FB_CTU_fbt.h
        FB_CTU_LINT_fbt.h
        FB_CTU_UDINT_fbt.h
        FB_CTU_ULINT_fbt.h
        FB_CTUD_DINT_fbt.h
        FB_CTUD_fbt.h
        FB_CTUD_LINT_fbt.h
        FB_CTUD_ULINT_fbt.h
)
