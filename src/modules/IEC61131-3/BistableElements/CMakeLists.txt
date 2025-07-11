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
# BistableElements
# ############################################################################

add_library(forte-modules-IEC61131-3-BistableElements OBJECT
            FB_RS_fbt.h FB_RS_fbt.cpp FB_SR_fbt.h FB_SR_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-BistableElements)
