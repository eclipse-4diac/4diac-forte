#*******************************************************************************
# Copyright (c) 2018 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Martin Melik-Merkumians - inital contribution
# *    Martin Melik-Merkumians - adds IB, QB, IL, QL
# *******************************************************************************/ 
 
forte_add_module(ADS OFF "Interacting with ADS servers")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(FORTE_MODULE_ADS_PI_ADS_ROOT "" CACHE PATH "Path to ADS library source root directory")

forte_add_include_directories( ${FORTE_MODULE_ADS_PI_ADS_ROOT}/AdsLib )

find_library( ADS_LIBRARY libads.a )
forte_add_link_library( ${ADS_LIBRARY} )

#######################################################
# ADS interface
#######################################################

forte_add_sourcefile_hcpp(CAdsProcessInterface CAdsConnection CAdsConnectionManager)
forte_add_sourcefile_hcpp(ADS_SERVER_CONFIG SET_LOCAL_ADS_ADDRESS)
forte_add_sourcefile_h(processinterface.h)
forte_set_process_interface("ADS" IX QX IW QW ID QD IL QL)
