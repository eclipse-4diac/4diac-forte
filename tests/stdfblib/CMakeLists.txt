#*******************************************************************************
# * Copyright (c) 2014 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/
SET(SOURCE_GROUP ${SOURCE_GROUP}\\fblib)

add_subdirectory(events)

forte_test_add_sourcefile_cpp(CFB_TEST.cpp)
forte_test_add_sourcefile_cpp(CFB_TEST_tester.cpp)