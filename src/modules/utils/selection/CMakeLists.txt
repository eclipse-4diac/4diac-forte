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
add_library(forte-modules-utils-selection OBJECT
            F_SEL_E_2_fbt.h F_SEL_E_2_fbt.cpp F_SEL_E_3_fbt.h
            F_SEL_E_3_fbt.cpp F_SEL_E_4_fbt.h F_SEL_E_4_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils-selection)
