# *******************************************************************************
# Copyright (c) 2012, 2024 AIT, ACIN, fortiss GmbH, Hit robot group, Samator
# Indo Gas This program and the accompanying materials are made available
# under the terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Filip Andren, Alois Zoitl - initial API and implementation
# and/or initial documentation Tibalt Zhao - Ease the workload to compile OPC
# DA Ketut Kumajaya - switch to OPCClientToolKit with 64bit support
# *******************************************************************************/
# ############################################################################
# OPC Com Layer
# ############################################################################

if (NOT WIN32)
    return()
endif ()

option(FORTE_COM_OPC "Enable OPC communication" OFF)
if (NOT FORTE_COM_OPC)
    return()
endif ()

set(FORTE_COM_OPC_LIB_ROOT
        ""
        CACHE PATH "Path to OPC library root directory"
)
set(FORTE_COM_OPC_BOOST_ROOT
        ""
        CACHE PATH "Path to Boost library root directory"
)

add_library(forte-com-opc
        $<$<BOOL:${FORTE_COM_OPC}>:opccomlayer.cpp>
        $<$<BOOL:${FORTE_COM_OPC}>:opccomlayer.h>
        opcconnection.h
        opcconnection.cpp
        opcconnectionhandler.h
        opcconnectionhandler.cpp
        opceventhandler.h
        opceventhandler.cpp
        opcconnectionimpl.h
        opcconnectionimpl.cpp
        opcprocessvar.h
        opcprocessvar.cpp
        Cmd_AddConnection.h
        Cmd_AddConnection.cpp
        Cmd_RemoveConnection.h
        Cmd_RemoveConnection.cpp
        Cmd_SetProcessVarValue.h
        Cmd_SetProcessVarValue.cpp
)
target_link_libraries(forte-com-opc PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-opc,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-opc>>)
target_include_directories(forte-com-opc PRIVATE ${FORTE_COM_OPC_BOOST_ROOT})
target_include_directories(forte-com-opc PRIVATE ${FORTE_COM_OPC_LIB_ROOT}/include/OPCClientToolKit)
target_link_directories(forte-com-opc PUBLIC ${FORTE_COM_OPC_LIB_ROOT}/lib)
target_link_libraries(forte-com-opc PRIVATE OPCClientToolKit)

install(TARGETS forte-com-opc EXPORT forte-export FILE_SET HEADERS)
