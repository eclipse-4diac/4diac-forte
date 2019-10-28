#*******************************************************************************
# Copyright (c) 2011 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_test_add_sourcefile_cpp(fbdkasn1layerser_test.cpp)
  forte_test_add_sourcefile_cpp(fbdkasn1layerdeser_test.cpp)
  forte_test_add_sourcefile_cpp(extractLayerAndParamsTest.cpp)
  
