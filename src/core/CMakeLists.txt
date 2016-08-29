#*******************************************************************************
# * Copyright (c) 2010 - 2016 Profactor GmbH, ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
# *      - initial implementation and rework communication infrastructure
# *    Martin Jobst - adapt for LUA integration
# *******************************************************************************/
#############################################################################
# Core
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(FORTE_USE_LUATYPES "None" CACHE STRING "Enable Lua FB types")
list(APPEND luas "LuaJIT" "Lua")
set_property(CACHE FORTE_USE_LUATYPES PROPERTY STRINGS None ${luas})
if("${FORTE_USE_LUATYPES}" STREQUAL "LuaJIT")
  set(LUAJIT_LIBRARY "" CACHE STRING "LuaJit Library")
  set(LUAJIT_INCLUDE_DIR "" CACHE STRING "LuaJit Include Directory")
  forte_add_include_directories(${LUAJIT_INCLUDE_DIR})
  forte_add_link_library(${LUAJIT_LIBRARY})
  if(UNIX AND NOT APPLE)
    set(LUAJIT_MATH_LIBRARY "" CACHE STRING "LuaJit Math Library")
    forte_add_link_library(${LUAJIT_MATH_LIBRARY})
  endif(UNIX AND NOT APPLE)
elseif("${FORTE_USE_LUATYPES}" STREQUAL "Lua")
  find_package(Lua REQUIRED)
  forte_add_include_directories(${LUA_INCLUDE_DIR})
  forte_add_link_library(${LUA_LIBRARIES})
endif("${FORTE_USE_LUATYPES}" STREQUAL "LuaJIT")

forte_add_subdirectory(cominfra)
forte_add_subdirectory(datatypes)
forte_add_subdirectory(utils)

forte_add_sourcefile_hcpp(conn  dataconn  eventconn)     

forte_add_sourcefile_h   (esfb.h      event.h    mgmcmd.h      fortenode.h fortelist.h convert_functions.h)
forte_add_sourcefile_hcpp(basicfb     cfb        device      devexec )
forte_add_sourcefile_hcpp(extevhan   funcbloc      fbcontainer  if2indco )
forte_add_sourcefile_hcpp(resource    stringdict typelib       ecet)
forte_add_sourcefile_hcpp(adapterconn adapter    iec61131_functions)
if(NOT FORTE_USE_LUATYPES STREQUAL "None")
  forte_add_sourcefile_hcpp(luaengine luafb luatype)
endif(NOT FORTE_USE_LUATYPES STREQUAL "None")

if(FORTE_SUPPORT_MONITORING)
  forte_add_sourcefile_hcpp(monitoring)
endif(FORTE_SUPPORT_MONITORING)


