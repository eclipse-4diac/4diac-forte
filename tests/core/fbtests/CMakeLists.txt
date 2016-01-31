#*******************************************************************************
# * Copyright (c) 2011 - 2015 ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/
SET(SOURCE_GROUP ${SOURCE_GROUP}\\fbtests)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})


forte_test_add_sourcefile_cpp(fbtester.cpp)
forte_test_add_sourcefile_cpp(fbtestsmanager.cpp)
