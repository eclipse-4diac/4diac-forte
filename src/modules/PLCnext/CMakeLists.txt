#*******************************************************************************
# * Copyright (c) 2022 Peirlberger Juergen
# * This program and the accompanying materials are made available under the
# * terms of the Eclipse Public License 2.0 which is available at
# * http://www.eclipse.org/legal/epl-2.0.
# *
# * SPDX-License-Identifier: EPL-2.0
# *
# * Contributors:
# *   Peirlberger Juergen - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_io(PLCNEXT "Support for PLCnext system")

if(FORTE_IO_PLCNEXT)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# PLCnext
#############################################################################
list(INSERT CMAKE_MODULE_PATH 0 "${ARP_TOOLCHAIN_CMAKE_MODULE_PATH}")
forte_add_include_directories(${ArpProgramming_INCLUDE_DIR})

forte_add_sourcefile_hcpp(types/PLCnextBusAdapter)
forte_add_sourcefile_hcpp(types/PLCnextAXLSEDI16)
forte_add_sourcefile_hcpp(types/PLCnextAXLSEDO16)
forte_add_sourcefile_hcpp(types/PLCnextAXLSESC)
forte_add_sourcefile_hcpp(types/PLCnextMaster)

forte_add_sourcefile_hcpp(deviceController slaveHandler slaveHandle/slaveHandle slaveHandle/bitSlaveHandle)
forte_add_sourcefile_hcpp(plcNextDeviceInterface plcNextDeviceStatus)

find_package(ArpDevice REQUIRED)
find_package(ArpProgramming REQUIRED)

forte_add_link_library_beginning(ArpDevice)
forte_add_link_library_beginning(ArpProgramming)
forte_add_link_library_beginning(Arp.System.ModuleLib)
forte_add_link_library_beginning(Arp.System.Module)
forte_add_link_library_beginning(Arp.Plc.AnsiC)

forte_add_link_flags("-Wno-undef")

else("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  MESSAGE(FATAL_ERROR "PLCnext IO system is only supported for FORTE on Linux (Posix) platforms!")
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

endif(FORTE_IO_PLCNEXT)
