# *******************************************************************************
# Copyright (c) 2013 ACIN This program and the accompanying materials are made
# available under the terms of the Eclipse Public License 2.0 which is
# available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# DT_TO_xxx Conversion
# ############################################################################

add_library(forte-modules-IEC61131-3-Conversion-DT OBJECT
            F_DT_TO_TOD_fbt.h F_DT_TO_TOD_fbt.cpp F_DT_TO_DATE_fbt.h
            F_DT_TO_DATE_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Conversion-DT)
