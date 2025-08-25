# *******************************************************************************
# Copyright (c) 2016 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Johannes Messmer *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_io(EMBRICK OFF "Support for the modular emBRICK system")

# ############################################################################
# emBrick FBs
# ############################################################################

add_library(forte-modules-embrick OBJECT
            handler/spi.h
            handler/spi.cpp
            handler/pin.h
            handler/pin.cpp
            handler/bus.h
            handler/bus.cpp
            slave/slave.h
            slave/slave.cpp
            slave/handle.h
            slave/handle.cpp
            slave/handles/bit.h
            slave/handles/bit.cpp
            slave/handles/analog.h
            slave/handles/analog.cpp
            slave/handles/analog10.h
            slave/handles/analog10.cpp
            types/EBBusAdapter.h
            types/EBBusAdapter.cpp
            types/EBMaster.h
            types/EBMaster.cpp
            types/Slave.h
            types/Slave.cpp
            types/EBSlave2181.h
            types/EBSlave2181.cpp
            types/EBSlave2301.h
            types/EBSlave2301.cpp
)
target_link_libraries(forte-modules-embrick PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-modules-embrick)
