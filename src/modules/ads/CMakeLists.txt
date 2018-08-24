#*******************************************************************************
# * Copyright (c) 2018 TU Wien/ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Martin Melik-Merkumians
# *******************************************************************************/ 
 
forte_add_module(ADS "Interacting with ADS servers")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(FORTE_MODULE_ADS_PI_ADS_ROOT "" CACHE PATH "Path to ADS library source root directory")

forte_add_include_directories( ${FORTE_MODULE_ADS_PI_ADS_ROOT}/AdsLib )

find_library( ADS_LIBRARY AdsLib-Linux.a )
forte_add_link_library( ${ADS_LIBRARY} )

#######################################################
# ADS interface
#######################################################

forte_add_definition(-std=c++11)

forte_add_sourcefile_hcpp(CAdsProcessInterface CAdsConnection CAdsConnectionManager)
forte_add_sourcefile_hcpp(ADS_SERVER_CONFIG SET_LOCAL_ADS_ADDRESS)

forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp(../../stdfblib/io/IW)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QW)
forte_add_sourcefile_hcpp(../../stdfblib/io/ID)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QD)