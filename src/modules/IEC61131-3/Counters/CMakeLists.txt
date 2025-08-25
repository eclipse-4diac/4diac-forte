# *******************************************************************************
# Copyright (c) 2011 - 2012 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# Counters
# ############################################################################

target_sources(forte-modules-IEC61131-3 PRIVATE
            FB_CTD_DINT_fbt.h
            FB_CTD_DINT_fbt.cpp
            FB_CTD_UDINT_fbt.h
            FB_CTD_UDINT_fbt.cpp
            FB_CTD_fbt.h
            FB_CTD_fbt.cpp
            FB_CTUD_DINT_fbt.h
            FB_CTUD_DINT_fbt.cpp
            FB_CTU_UDINT_fbt.h
            FB_CTU_UDINT_fbt.cpp
            FB_CTU_fbt.h
            FB_CTU_fbt.cpp
            FB_CTU_DINT_fbt.h
            FB_CTU_DINT_fbt.cpp
            FB_CTUD_fbt.h
            FB_CTUD_fbt.cpp
            FB_CTD_LINT_fbt.h
            FB_CTD_LINT_fbt.cpp
            FB_CTD_ULINT_fbt.h
            FB_CTD_ULINT_fbt.cpp
            FB_CTU_LINT_fbt.h
            FB_CTU_LINT_fbt.cpp
            FB_CTU_ULINT_fbt.h
            FB_CTU_ULINT_fbt.cpp
            FB_CTUD_LINT_fbt.h
            FB_CTUD_LINT_fbt.cpp
            FB_CTUD_ULINT_fbt.h
            FB_CTUD_ULINT_fbt.cpp
)
