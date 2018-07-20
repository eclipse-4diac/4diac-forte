#/*******************************************************************************
# * Copyright (c) 2013 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/


forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_test_add_sourcefile_cpp(testsingleton.cpp singeltontest.cpp singletontest2ndunit.cpp parameterParserTest.cpp string_utils_test.cpp)