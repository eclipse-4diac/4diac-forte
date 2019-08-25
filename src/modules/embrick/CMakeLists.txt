#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH
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

forte_add_io(EMBRICK "Support for the modular emBRICK system")

if(FORTE_IO_EMBRICK)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# emBrick FBs
#############################################################################

forte_add_sourcefile_hcpp(handler/spi handler/pin handler/bus)

forte_add_sourcefile_hcpp(slave/slave slave/handle)
forte_add_sourcefile_hcpp(slave/handles/bit slave/handles/analog slave/handles/analog10)

forte_add_sourcefile_hcpp(types/BusAdapter)
forte_add_sourcefile_hcpp(types/Master)
forte_add_sourcefile_hcpp(types/Slave types/Slave2181 types/Slave2301)

else("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  MESSAGE(FATAL_ERROR "The emBRICK IO system is only supported for FORTE on Linux platforms!")
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

endif(FORTE_IO_EMBRICK)
