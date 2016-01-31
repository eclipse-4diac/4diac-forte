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

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src/core/cominfra)
  
  
  forte_test_add_sourcefile_cpp(fbdkasn1layerser_test.cpp)
  forte_test_add_sourcefile_cpp(fbdkasn1layerdeser_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/cominfra/comlayer.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/cominfra/fbdkasn1layer.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/cominfra/commfb.cpp)
  forte_test_add_sourcefile_cpp(devlogtestmock.cpp)
  forte_test_add_sourcefile_cpp(comlayermantestmock.cpp)
  
  #forte_test_add_sourcefile_cpp(../../../bin/posix/core/cominfra/comlayersmanager.cpp)
