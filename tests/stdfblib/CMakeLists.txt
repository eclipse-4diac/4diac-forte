#*******************************************************************************
# Copyright (c) 2014 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/
SET(SOURCE_GROUP ${SOURCE_GROUP}\\fblib)

add_subdirectory(events)

forte_test_add_sourcefile_cpp(CFB_TEST.cpp)
forte_test_add_sourcefile_cpp(CFB_TEST_tester.cpp)