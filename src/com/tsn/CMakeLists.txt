################################################################################
# Copyright (c) 2018 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Ben Schneider
#     - Initial implementation
################################################################################

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

forte_add_network_layer(TSN OFF "tsn" CTSNLayer tsn_layer "Enable Forte Com Ethernet Time-Sensitive Networking for Pub-Sub")


if(FORTE_COM_TSN)
   forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   forte_add_sourcefile_hcpp(tsn_layer)
endif(FORTE_COM_TSN)

endif()