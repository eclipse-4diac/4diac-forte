#*******************************************************************************
# * Copyright (c) 2011 - 2015 ACIN, nxtControl, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src/core)
  
    forte_test_add_sourcefile_cpp(../../src/core/typelib.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/stringdict.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/funcbloc.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/cfb.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/eventconn.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/conn.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/ecet.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/resource.cpp)
    
    if(FORTE_SUPPORT_MONITORING)
      forte_test_add_sourcefile_cpp(../../src/core/monitoring.cpp)
    endif(FORTE_SUPPORT_MONITORING)
    
    forte_test_add_sourcefile_cpp(../../src/core/fbcontainer.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/device.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/dataconn.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/adapterconn.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/if2indco.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/adapter.cpp)
        
    forte_test_add_sourcefile_cpp(../../src/core/devexec.cpp)
    forte_test_add_sourcefile_cpp(../../src/core/extevhan.cpp)
    
    forte_test_add_sourcefile_cpp(../../src/core/utils/string_utils.cpp)
    
    forte_test_add_sourcefile_cpp(../../src/core/utils/parameterParser.cpp)
    
    forte_test_add_sourcefile_cpp(../../src/stdfblib/ita/EMB_RES.cpp)
    
  #TODO add here architecture specific adds in order to allow testrunns also on other platforms
     forte_test_add_sourcefile_cpp(../../src/arch/timerha.cpp)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    forte_test_add_sourcefile_cpp(../../src/arch/posix/forte_sync.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/posix/forte_sem.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/posix/forte_thread.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/posix/pctimeha.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/genforte_printer.cpp)
endif()

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    forte_test_add_sourcefile_cpp(../../src/arch/win32/forte_sync.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/win32/forte_sem.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/win32/forte_thread.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/win32/pctimeha.cpp)
    forte_test_add_sourcefile_cpp(../../src/arch/win32/winforte_printer.cpp)
endif()

  
forte_test_add_sourcefile_cpp(../../src/core/stringdict.cpp)
forte_test_add_sourcefile_cpp(stringdicttests.cpp)
forte_test_add_sourcefile_cpp(typelibdatatypetests.cpp)
forte_test_add_sourcefile_cpp(nameidentifiertest.cpp)
forte_test_add_sourcefile_cpp(mgmstatemachinetest.cpp)
forte_test_add_sourcefile_cpp(iec61131_functionstests.cpp)

if("${FORTE_USE_LUATYPES}" STREQUAL "LuaJIT" OR "${FORTE_USE_LUATYPES}" STREQUAL "Lua")
  forte_test_add_sourcefile_cpp(../../src/core/lua/luaengine.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luatype.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luabfb.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luabfbtypeentry.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luaadapter.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luaadaptertypeentry.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luacfb.cpp)
  forte_test_add_sourcefile_cpp(../../src/core/lua/luacfbtypeentry.cpp)
endif("${FORTE_USE_LUATYPES}" STREQUAL "LuaJIT" OR "${FORTE_USE_LUATYPES}" STREQUAL "Lua")

forte_test_add_subdirectory(datatypes)
forte_test_add_subdirectory(cominfra)
forte_test_add_subdirectory(fbtests)
forte_test_add_subdirectory(utils)
