# *******************************************************************************
# Copyright (c) 2017, 2026 fortiss GmbH, Aimirim STI
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Monika Wenger - initial API and implementation and/or initial documentation
#   Pedro Ricardo C. Souza - Lua 5.1 through pkg-config or a given target, embedded STfunc source
# *******************************************************************************/
# ############################################################################
# core/lua
# ############################################################################

if (NOT FORTE_DYNAMIC_TYPE_LOAD)
    return()
endif ()

# Lua 5.1 must be built as C: Lua errors unwind with longjmp, which the Lua sources are written for.
set(FORTE_LUA_TARGET "" CACHE STRING "CMake target providing Lua 5.1 (leave empty to find it through pkg-config)")
mark_as_advanced(FORTE_LUA_TARGET)
add_library(forte-lua-library INTERFACE IMPORTED GLOBAL)
if (FORTE_LUA_TARGET)
    target_link_libraries(forte-lua-library INTERFACE ${FORTE_LUA_TARGET})
else ()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(LUA51 REQUIRED IMPORTED_TARGET GLOBAL lua5.1 lua-5.1 lua51)
    target_link_libraries(forte-lua-library INTERFACE PkgConfig::LUA51)
endif ()

set(FORTE_LUA_INSTRUCTION_LIMIT
        "0"
        CACHE STRING "Lua VM instructions allowed per call into a Lua type, 0 for no limit"
)
mark_as_advanced(FORTE_LUA_INSTRUCTION_LIMIT)

option(FORTE_LUA_SHARED_STATE
       "One Lua state for the type definitions and the instances of all resources, which then take turns running Lua code (saves memory on small targets)"
       OFF
)
mark_as_advanced(FORTE_LUA_SHARED_STATE)

set(FORTE_STFUNC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lua_src/STfunc.lua)
set(FORTE_STFUNC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/generated/stfunc_source.h)
add_custom_command(
        OUTPUT ${FORTE_STFUNC_HEADER}
        COMMAND ${CMAKE_COMMAND} -DINPUT=${FORTE_STFUNC_SOURCE} -DOUTPUT=${FORTE_STFUNC_HEADER}
                -DVARIABLE=cgSTfuncSource -P ${CMAKE_CURRENT_SOURCE_DIR}/embed_lua_source.cmake
        DEPENDS ${FORTE_STFUNC_SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/embed_lua_source.cmake
        COMMENT "Embedding STfunc.lua"
        VERBATIM
)
# forte-core is defined in another directory, which cannot see the custom command's output
add_custom_target(forte-lua-stfunc DEPENDS ${FORTE_STFUNC_HEADER})
add_dependencies(forte-core forte-lua-stfunc)

set(FORTE_LUA_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/luaadapter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luabfb.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luabit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luacfb.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luaengine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luainterface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luatypes.cpp
)

target_sources(forte-core PRIVATE
        luaadapter.h
        luabfb.h
        luabit.h
        luacfb.h
        luaengine.h
        luainterface.h
        luatypehash.h
        luatypes.h
        ${FORTE_LUA_SOURCES}
)

# The Lua sources include a generated header, so they are kept out of the C++ module scan.
set_source_files_properties(${FORTE_LUA_SOURCES} TARGET_DIRECTORY forte-core PROPERTIES
        CXX_SCAN_FOR_MODULES OFF
)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/luaengine.cpp TARGET_DIRECTORY forte-core PROPERTIES
        COMPILE_DEFINITIONS "FORTE_LUA_INSTRUCTION_LIMIT=${FORTE_LUA_INSTRUCTION_LIMIT}"
)

target_include_directories(forte-core PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_link_libraries(forte-core PRIVATE forte-lua-library)
target_compile_definitions(forte-core PUBLIC FORTE_DYNAMIC_TYPE_LOAD)
# public: it changes the layout of ResourceInternal, which the tests include
if (FORTE_LUA_SHARED_STATE)
    target_compile_definitions(forte-core PUBLIC FORTE_LUA_SHARED_STATE)
endif ()
