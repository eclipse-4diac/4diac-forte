# *******************************************************************************
# Copyright (c) 2010 - 2016 Profactor GmbH, ACIN, fortiss GmbH * 2018 Johannes
# Kepler University 2022 Martin Erich Jobst This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
# *      - initial implementation and rework communication infrastructure *
# Martin Jobst - adapt for LUA integration *    Alois Zoitl - introduced new
# CGenFB class for better handling generic FBs Martin Jobst - add CTF tracing
# support
# *******************************************************************************/
# ############################################################################
# Core
# ############################################################################
add_subdirectory(generated)

option(FORTE_USE_FAKE_ECET "Compile the fake ecet used for testing" FALSE)
mark_as_advanced(FORTE_USE_FAKE_ECET)

if (FORTE_DYNAMIC_TYPE_LOAD)
    add_subdirectory(lua)
endif (FORTE_DYNAMIC_TYPE_LOAD)
add_subdirectory(trace)

add_library(forte-core OBJECT
            conn.h
            dataconn.h
            dataconn.cpp
            inoutdataconn.h
            inoutdataconn.cpp
            eventconn.h
            eventconn.cpp
            esfb.h
            event.h
            fortenode.h
            fortelist.h
            genfb.h
            interfacespec.h
            interfacespec.cpp
            basefb.h
            basefb.cpp
            basicfb.h
            basicfb.cpp
            cfb.h
            cfb.cpp
            simplefb.h
            simplefb.cpp
            device.h
            device.cpp
            devicefactory.h
            devexec.h
            devexec.cpp
            forteinstance.h
            forteinstance.cpp
            mgmcmd.h
            mgmcmd.cpp
            extevhan.h
            extevhan.cpp
            funcbloc.h
            funcbloc.cpp
            fbcontainer.h
            fbcontainer.cpp
            resource.h
            resource.cpp
            stringid.h
            stringid.cpp
        timerha.h
        timerha.cpp
            timerhandlerfactory.h
            typelib.h
            typelib.cpp
            ecet.h
            ecet.cpp
            typelib_internal.h
            adapterconn.h
            adapterconn.cpp
            adapter.h
            adapter.cpp
            anyadapter.h
            anyadapter.cpp
            iec61131_functions.h
            iec61131_functions.cpp
            forte_st_iterator.h
            forte_st_util.h
            delegdataconn.h
            negdataconn.h
            negdataconn.cpp
            membdataconn.h
            membdataconn.cpp
            gatherdataconn.h
            gatherdataconn.cpp
            $<$<BOOL:${FORTE_USE_FAKE_ECET}>:${CMAKE_CURRENT_SOURCE_DIR}/ecetFake.h>
            $<$<BOOL:${FORTE_USE_FAKE_ECET}>:${CMAKE_CURRENT_SOURCE_DIR}/ecetFake.cpp>
            monitoring.h
            monitoring.cpp
)
target_link_libraries(forte PRIVATE forte-core)

add_subdirectory(com)
add_subdirectory(cominfra)
add_subdirectory(datatypes)
add_subdirectory(util)
add_subdirectory(io)
add_subdirectory(fmi)
