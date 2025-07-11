# *******************************************************************************
# Copyright (c) 2011 - 2013 ACIN, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger, Alois Zoitl *   - initial API and
# implementation and/or initial documentation
# *******************************************************************************/

# ############################################################################
# Comparison
# ############################################################################
# add_subdirectory(tests)

add_library(forte-modules-IEC61131-3-Comparison OBJECT
            F_GT_fbt.h
            F_GT_fbt.cpp
            F_EQ_fbt.h
            F_EQ_fbt.cpp
            F_GE_fbt.h
            F_GE_fbt.cpp
            F_LE_fbt.h
            F_LE_fbt.cpp
            F_LT_fbt.h
            F_LT_fbt.cpp
            F_NE_fbt.h
            F_NE_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Comparison)
