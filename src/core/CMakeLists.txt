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
#############################################################################
# Core
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_subdirectory(generated)

forte_add_subdirectory(cominfra)
forte_add_subdirectory(datatypes)
forte_add_subdirectory(util)
forte_add_subdirectory(io)
forte_add_subdirectory(fmi)

forte_add_sourcefile_h(conn.h)
forte_add_sourcefile_hcpp(dataconn inoutdataconn eventconn)

forte_add_sourcefile_h(esfb.h event.h fortenode.h fortelist.h genfb.h)
forte_add_sourcefile_hcpp(interfacespec basefb basicfb cfb simplefb device devexec forteinstance mgmcmd)
forte_add_sourcefile_hcpp(extevhan funcbloc fbcontainer)
forte_add_sourcefile_hcpp(resource typelib ecet)
forte_add_sourcefile_h(typelib_internal.h)
forte_add_sourcefile_hcpp(adapterconn adapter anyadapter iec61131_functions)
forte_add_sourcefile_h(forte_st_iterator.h)
forte_add_sourcefile_h(forte_st_util.h)
forte_add_sourcefile_h(delegdataconn.h)
forte_add_sourcefile_hcpp(negdataconn)
forte_add_sourcefile_hcpp(membdataconn)
forte_add_sourcefile_hcpp(gatherdataconn)

forte_add_ecet(CEventChainExecutionThread ecet "standard")

set(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY "30" CACHE STRING "Max supported hierarchy that can be provided in a management commands")
mark_as_advanced(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY)
forte_generate_config_value_include(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY)

option(FORTE_USE_FAKE_ECET "Compile the fake ecet used for testing" FALSE)
mark_as_advanced(FORTE_USE_FAKE_ECET)
if(FORTE_USE_FAKE_ECET)
  forte_add_sourcefile_hcpp(ecetFake)
  forte_add_ecet(CFakeEventExecutionThread core/ecetFake "fake")
endif(FORTE_USE_FAKE_ECET)

if(FORTE_DYNAMIC_TYPE_LOAD)
  forte_add_subdirectory(lua)
endif(FORTE_DYNAMIC_TYPE_LOAD)

forte_add_sourcefile_hcpp(monitoring)

forte_add_subdirectory(trace)

option(FORTE_STRINGDICT_FIXEDMEMORY "Use a fixed-size version of the string dictionary" OFF)
forte_generate_config_flag_include(FORTE_STRINGDICT_FIXEDMEMORY)
if (FORTE_STRINGDICT_FIXEDMEMORY)
    set(FORTE_STRINGDICT_MAXNROFSTRINGS 1024 CACHE STRING "Maximum number of strings in the fixed-size version of the string dictionary")
    set(FORTE_STRINGDICT_BUFFERSIZE 8192 CACHE STRING "Maximum total size of the fixed-size version of the string dictionary")
    forte_generate_config_value_include(FORTE_STRINGDICT_MAXNROFSTRINGS)
    forte_generate_config_value_include(FORTE_STRINGDICT_BUFFERSIZE)
endif ()

