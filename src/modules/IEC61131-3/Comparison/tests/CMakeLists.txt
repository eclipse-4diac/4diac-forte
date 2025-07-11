# *******************************************************************************
# Copyright (c) 2013 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# Tests for the standard Event FBs
# ############################################################################

if (NOT FORTE_FUNCTIONBLOCK_TESTS)
    return()
endif ()
add_library(forte-modules-IEC61131-3-Comparison-tests OBJECT
            F_EQ_TIME_tester.h F_EQ_TIME_tester.cpp F_GT_TIME_tester.h
            F_GT_TIME_tester.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Comparison-tests)
