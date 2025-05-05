#*******************************************************************************
# Copyright (c) 2017, 2025 fortiss GmbH, Johannes Kepler University Linz
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Johannes Messmer
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# IO
#############################################################################

set(FORTE_IO OFF CACHE BOOL "Enable IO Modules")

if(NOT FORTE_IO)
  return()
endif(NOT FORTE_IO)

SET(SOURCE_GROUP ${SOURCE_GROUP}\\io)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(configFB)
add_subdirectory(device)
add_subdirectory(eIO)
add_subdirectory(mapper)

forte_add_sourcefile_hcpp(processinterfacefb)
forte_add_sourcefile_h(inputfb.h outputfb.h)
forte_add_sourcefile_hcpp(IB_fbt ID_fbt IE_fbt IL_fbt IW_fbt IX_fbt QB_fbt QD_fbt QL_fbt QW_fbt QX_fbt)
forte_set_process_interface("Modular IO" )
