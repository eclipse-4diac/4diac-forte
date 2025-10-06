# ############################################################################
# Copyright (c) 2010, 2025 Profactor GmbH, ACIN, fortiss GmbH
#                          Johannes Kepler University
#                          Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
#      - initial implementation and rework communication infrastructure *
#    Martin Jobst
#      - adapt for LUA integration
#    Alois Zoitl
#      - introduced new CGenFB class for better handling generic FBs
#    Martin Jobst
#      - add CTF tracing support
#      - include reorganization
# ############################################################################

option(FORTE_USE_FAKE_ECET "Compile the fake ecet used for testing" FALSE)
mark_as_advanced(FORTE_USE_FAKE_ECET)

target_sources(forte-core PRIVATE
        adapter.cpp
        adapterconn.cpp
        anyadapter.cpp
        anyadapter.h
        basefb.cpp
        basicfb.cpp
        cfb.cpp
        dataconn.cpp
        delegdataconn.h
        devexec.cpp
        device.cpp
        devicefactory.cpp
        ecet.cpp
        ecetfactory.cpp
        eventconn.cpp
        extevhan.cpp
        fbcontainer.cpp
        forteinstance.cpp
        forteiterator.h
        fortelist.h
        fortenode.h
        funcbloc.cpp
        gatherdataconn.cpp
        gatherdataconn.h
        iec61131_functions.cpp
        inoutdataconn.cpp
        interfacespec.cpp
        membdataconn.cpp
        membdataconn.h
        mgmcmd.cpp
        monitoring.cpp
        monitoring.h
        negdataconn.cpp
        negdataconn.h
        resource.cpp
        resource_internal.h
        simplefb.cpp
        startuphook.cpp
        stringid.cpp
        timerha.cpp
        timerhandlerfactory.cpp
        typelib.cpp
        typelib_internal.h
        $<$<BOOL:${FORTE_USE_FAKE_ECET}>:${CMAKE_CURRENT_SOURCE_DIR}/ecetFake.h>
        $<$<BOOL:${FORTE_USE_FAKE_ECET}>:${CMAKE_CURRENT_SOURCE_DIR}/ecetFake.cpp>
)

add_subdirectory(com)
add_subdirectory(cominfra)
add_subdirectory(datatypes)
add_subdirectory(fmi)
add_subdirectory(io)
add_subdirectory(lua)
add_subdirectory(trace)
add_subdirectory(util)
