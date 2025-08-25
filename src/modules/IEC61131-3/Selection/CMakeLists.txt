# *******************************************************************************
# Copyright (c) 2011 - 2013 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# Selection
# ############################################################################

target_sources(forte-modules-IEC61131-3 PRIVATE
            F_LIMIT_fbt.h
            F_LIMIT_fbt.cpp
            F_MIN_fbt.h
            F_MIN_fbt.cpp
            F_MAX_fbt.h
            F_MAX_fbt.cpp
            F_SEL_fbt.h
            F_SEL_fbt.cpp
            F_MUX_2_fbt.h
            F_MUX_2_fbt.cpp
)
