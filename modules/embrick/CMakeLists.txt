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

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_EMBRICK "Support for the modular emBRICK system" OFF)

if (NOT FORTE_MODULE_EMBRICK)
    return()
endif ()

# ############################################################################
# emBrick FBs
# ############################################################################

add_library(forte-embrick
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
target_link_libraries(forte-embrick PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-embrick,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-embrick>>)
install(TARGETS forte-embrick EXPORT forte-export FILE_SET HEADERS)
