#*******************************************************************************
# Copyright (c) 2010 - 2016 Profactor GmbH, ACIN, fortiss GmbH
# *                      2018 Johannes Kepler University
#               2022 Martin Erich Jobst
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
# *      - initial implementation and rework communication infrastructure
# *    Martin Jobst - adapt for LUA integration
# *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
#      Martin Jobst - add CTF tracing support
# *******************************************************************************/

option(FORTE_STRINGDICT_FIXED_MEMORY "Use a fixed-size buffer for string data, avoiding dynamic memory allocation" OFF)
mark_as_advanced(FORTE_STRINGDICT_FIXED_MEMORY)

set(FORTE_STRINGDICT_BUFFERSIZE "8000" CACHE STRING "FORTE string dict string buffer size")
mark_as_advanced(FORTE_STRINGDICT_BUFFERSIZE)

set(FORTE_STRINGDICT_MAXNROFSTRINGS "300" CACHE STRING "FORTE string dict max nr of strings")
mark_as_advanced(FORTE_STRINGDICT_MAXNROFSTRINGS)


add_library(forte-core OBJECT
    adapter.cpp
    adapterconn.cpp
    anyadapter.cpp
    basefb.cpp
    basicfb.cpp
    cfb.cpp
    dataconn.cpp
    devexec.cpp
    device.cpp
    ecet.cpp 
    eventconn.cpp 
    extevhan.cpp
    fbcontainer.cpp
    forteinstance.cpp
    funcbloc.cpp
    iec61131_functions.cpp 
    inoutdataconn.cpp
    interfacespec.cpp
    mgmcmd.cpp
    $<$<BOOL:${FORTE_SUPPORT_MONITORING}>:monitoring.cpp>
    negdataconn.cpp
    resource.cpp
    stringdict.cpp
    typelib.cpp
    ${FORTE_GENERATED_FILES_DIR}/deviceExecutionHandlers.cpp
    ${FORTE_GENERATED_FILES_DIR}/deviceFactory.cpp
    ${FORTE_GENERATED_FILES_DIR}/ecetFactory.cpp
    ${FORTE_GENERATED_FILES_DIR}/forteinit.cpp
)

target_compile_definitions(forte-core PRIVATE
    $<$<BOOL:${FORTE_STRINGDICT_FIXED_MEMORY}>:FORTE_STRINGDICT_FIXED_MEMORY>
    FORTE_STRINGDICT_BUFFERSIZE=${FORTE_STRINGDICT_BUFFERSIZE}
    FORTE_STRINGDICT_MAXNROFSTRINGS=${FORTE_STRINGDICT_MAXNROFSTRINGS}
)

target_link_libraries(forte PRIVATE forte-core)

add_subdirectory(cominfra)
add_subdirectory(datatypes)
add_subdirectory(util)
add_subdirectory(io)
add_subdirectory(fmi)

forte_register_ecet(CEventChainExecutionThread ecet.h)
forte_set_default_ecet(CEventChainExecutionThread)
