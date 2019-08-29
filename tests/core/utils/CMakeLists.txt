#/*******************************************************************************
# Copyright (c) 2013 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/


forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_test_add_sourcefile_cpp(testsingleton.cpp singeltontest.cpp singletontest2ndunit.cpp parameterParserTest.cpp string_utils_test.cpp)