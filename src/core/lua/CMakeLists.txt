# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
# ############################################################################
# core/lua
# ############################################################################

if (NOT FORTE_DYNAMIC_TYPE_LOAD)
    return()
endif ()

find_package(PkgConfig)

if (FORTE_USE_LUATYPES STREQUAL "LuaJIT")
    set(LUAJIT_LIBRARY
        ""
        CACHE FILEPATH "LuaJit Library"
    )
    set(LUAJIT_INCLUDE_DIR
        ""
        CACHE PATH "LuaJit Include Directory"
    )
    target_link_libraries(forte PUBLIC ${LUAJIT_LIBRARY})
    if (UNIX AND NOT APPLE)
        set(LUAJIT_MATH_LIBRARY
            ""
            CACHE FILEPATH "LuaJit Math Library"
        )
        target_link_libraries(forte PUBLIC ${LUAJIT_MATH_LIBRARY})
    endif ()
elseif (FORTE_USE_LUATYPES STREQUAL "Lua")

    pkg_check_modules(LUA lua)

    if (NOT LUA_FOUND)
        find_package(Lua REQUIRED)
    endif ()

    target_link_libraries(forte PUBLIC ${LUA_LIBRARIES})

endif ()

target_sources(forte-core PRIVATE
            luaengine.h
            luaengine.cpp
            luatype.h
            luatype.cpp
            luabfb.h
            luabfb.cpp
            luabfbtypeentry.h
            luabfbtypeentry.cpp
            luaadapter.h
            luaadapter.cpp
            luaadaptertypeentry.h
            luaadaptertypeentry.cpp
            luacfb.h
            luacfb.cpp
            luacfbtypeentry.h
            luacfbtypeentry.cpp
)
target_include_directories(forte-core PRIVATE ${LUAJIT_INCLUDE_DIR})
target_include_directories(forte-core PRIVATE ${LUA_INCLUDE_DIRS})
