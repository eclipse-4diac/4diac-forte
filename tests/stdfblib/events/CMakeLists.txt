#*******************************************************************************
# * Copyright (c) 2011, 2014 ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl  - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Tests for the standard Event FBs
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCE_GROUP ${SOURCE_GROUP}\\events)

forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_DELAY.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_F_TRIG.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_PERMIT.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_R_TRIG.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_SR.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_SWITCH.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_SELECT.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/GEN_E_DEMUX.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/GEN_E_MUX.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_CTD.cpp)
forte_test_add_sourcefile_cpp(../../../src/stdfblib/events/E_CTU.cpp)
forte_test_add_sourcefile_cpp(../../../src/core/basicfb.cpp)

#Removed as it is to unstable on build server 
#forte_test_add_sourcefile_cpp(E_DELAY_tester.cpp)

forte_test_add_sourcefile_cpp(E_F_TRIG_tester.cpp)
forte_test_add_sourcefile_cpp(E_PERMIT_tester.cpp)
forte_test_add_sourcefile_cpp(E_R_TRIG_tester.cpp)
forte_test_add_sourcefile_cpp(E_SR_tester.cpp)
forte_test_add_sourcefile_cpp(E_SELECT_tester.cpp)
forte_test_add_sourcefile_cpp(E_SWITCH_tester.cpp)
forte_test_add_sourcefile_cpp(E_CTD_tester.cpp)
forte_test_add_sourcefile_cpp(E_CTU_tester.cpp)
