#*******************************************************************************
# * Copyright (c) 2013 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Tests for the standard Event FBs
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCE_GROUP ${SOURCE_GROUP}\\events)


if(FORTE_FUNCTIONBLOCK_TESTS)

forte_add_sourcefile_hcpp(F_EQ_TIME_tester F_GT_TIME_tester)

endif(FORTE_FUNCTIONBLOCK_TESTS)
