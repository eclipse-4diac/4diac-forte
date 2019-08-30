################################################################################
# Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Alois Zoitl, Michael Hofmann
#         - initial implementation and rework communication infrastructure
#     Martin Melik-Merkumians, Monika Wenger
#      - add raw communication layer for ONE STRING input and ONE STRING output
################################################################################
  
#############################################################################
# Core
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_h(comtypes.h comlayersmanager.h)
forte_add_sourcefile_hcpp(basecommfb commfb comlayer)

# Will be created in top-level-cmake-file
forte_add_sourcefile_with_path_cpp(${CMAKE_BINARY_DIR}/core/cominfra/comlayersmanager.cpp) # created file

forte_add_network_layer(ETH ON "ip" CIPComLayer ipcomlayer "Enable Forte Com Ethernet") #adding this first we make sure that sockhand.h is included first
forte_add_network_layer(FBDK ON "fbdk" CFBDKASN1ComLayer fbdkasn1layer "Enable Forte Com FBDK")
forte_add_network_layer(LOCAL ON "loc" CLocalComLayer localcomlayer "Enable Forte local communication")
forte_add_network_layer(RAW ON "raw" CRawDataComLayer rawdatacomlayer "Enable Forte raw communication")
