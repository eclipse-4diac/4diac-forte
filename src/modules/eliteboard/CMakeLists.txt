#*******************************************************************************
# Copyright (c) 2021, 2022 Jonathan Lainer
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Jonathan Lainer - Initial implementation.
# *******************************************************************************/

if("${FORTE_ARCHITECTURE}" STREQUAL "FreeRTOSLwIP")
forte_add_io(ELITEBOARD "Support for the eLITe-Board development board")

if(FORTE_IO_ELITEBOARD)
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# eLITe-Borad specific fixes
#############################################################################

forte_add_definition("-ffunction-sections -fdata-sections -fno-threadsafe-statics -fno-rtti -fno-exceptions -Wl,--gc-sections")

#############################################################################
# Function Blocks
#############################################################################

add_subdirectory(handler)
forte_add_sourcefile_hcpp(handle/PinHandle)
forte_add_sourcefile_hcpp(types/EliteBoard)
forte_add_sourcefile_hcpp(types/Port_fbt types/PortAdapter_adp)

endif(FORTE_IO_ELITEBOARD)
endif("${FORTE_ARCHITECTURE}" STREQUAL "FreeRTOSLwIP")
