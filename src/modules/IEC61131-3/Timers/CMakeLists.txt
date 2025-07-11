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
# Timers
# ############################################################################

add_library(forte-modules-IEC61131-3-Timers OBJECT
            FB_TOF_fbt.h FB_TOF_fbt.cpp FB_TON_fbt.h FB_TON_fbt.cpp
            FB_TP_fbt.h FB_TP_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Timers)
