# *******************************************************************************
# * Copyright (c) 2022, 2024 Peirlberger Juergen, Jose Cabral
# * This program and the accompanying materials are made available under the
# * terms of the Eclipse Public License 2.0 which is available at
# * http://www.eclipse.org/legal/epl-2.0. *
# * SPDX-License-Identifier: EPL-2.0 *
# * Contributors:
# * Peirlberger Juergen - initial API and implementation and/or initial
#   documentation
# *******************************************************************************/

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_PLCNEXT "Support for PLCnext system" OFF)

if (NOT FORTE_MODULE_PLCNEXT)
    return()
endif ()

# ############################################################################
# PLCnext
# ############################################################################

# FIXME: This is undocumented, where does this settings come from? list(INSERT
# CMAKE_MODULE_PATH 0 "${ARP_TOOLCHAIN_CMAKE_MODULE_PATH}")

find_package(ArpDevice REQUIRED)
find_package(ArpProgramming REQUIRED)

target_link_libraries(forte PUBLIC ArpDevice)
target_link_libraries(forte PUBLIC ArpProgramming)
target_link_libraries(forte PUBLIC Arp.System.ModuleLib)
target_link_libraries(forte PUBLIC Arp.System.Module)
target_link_libraries(forte PUBLIC Arp.Plc.AnsiC)

add_library(forte-PLCnext
            types/PLCnextBusAdapter.h
            types/PLCnextBusAdapter.cpp
            types/PLCnextAXLSEDI16.h
            types/PLCnextAXLSEDI16.cpp
            types/PLCnextAXLSEDO16.h
            types/PLCnextAXLSEDO16.cpp
            types/PLCnextAXLSESC.h
            types/PLCnextAXLSESC.cpp
            types/PLCnextMaster.h
            types/PLCnextMaster.cpp
            deviceController.h
            deviceController.cpp
            slaveHandler.h
            slaveHandler.cpp
            slaveHandle/slaveHandle.h
            slaveHandle/slaveHandle.cpp
            slaveHandle/bitSlaveHandle.h
            slaveHandle/bitSlaveHandle.cpp
            plcNextDeviceInterface.h
            plcNextDeviceInterface.cpp
            plcNextDeviceStatus.h
            plcNextDeviceStatus.cpp
)
target_link_libraries(forte-PLCnext PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-PLCnext,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-PLCnext>>)
target_include_directories(forte-PLCnext
                           PUBLIC ${ArpProgramming_INCLUDE_DIR}
)

install(TARGETS forte-PLCnext EXPORT forte-export FILE_SET HEADERS)
