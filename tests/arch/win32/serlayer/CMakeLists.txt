#*******************************************************************************
# * Copyright (c) 2011 ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)
IF(FORTE_COM_SER)
  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src/arch/win32/serlayer)
  
  forte_test_add_sourcefile_cpp(../../../../src/arch/win32/serlayer/cwin32sercomparameterparser.cpp)
  
  forte_test_add_sourcefile_cpp(win32sercomparameterparsertests.cpp)
ENDIF(FORTE_COM_SER)
