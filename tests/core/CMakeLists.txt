#*******************************************************************************
# * Copyright (c) 2011 - 2015 ACIN, nxtControl, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  

  
forte_test_add_sourcefile_cpp(stringdicttests.cpp)
forte_test_add_sourcefile_cpp(typelibdatatypetests.cpp)
forte_test_add_sourcefile_cpp(nameidentifiertest.cpp)
forte_test_add_sourcefile_cpp(mgmstatemachinetest.cpp)
forte_test_add_sourcefile_cpp(iec61131_functionstests.cpp)
forte_test_add_sourcefile_cpp(convert_functionstests.cpp)
forte_test_add_sourcefile_cpp(interalvartests.cpp)

forte_test_add_subdirectory(datatypes)
forte_test_add_subdirectory(cominfra)
forte_test_add_subdirectory(fbtests)
forte_test_add_subdirectory(utils)
