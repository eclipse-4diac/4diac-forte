--[[
 * Copyright (c) 2022 Patrick Aigner
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Patrick Aigner
 *   - initial API and implementation and/or initial documentation
--]]


--[[
  Use following command to generate Lua byte code for loading (copyright header needs to be inserted manually)
    luajit -b STfunc.lua luamodule.h

  For quicker development (by avoiding unnecessary builds):
    > comment out this line in luaengine.cpp constructor:
        lua_setfield(luaState, -2, "STfunc");
    > copy STfunc.lua into same directory as forte executable
        this file will be automatically loaded by Lua
--]]

local STfunc = {}

function STfunc.ADD(a,b)
  return a+b
end

function STfunc.SUB(a,b)
  return a-b
end

function STfunc.MUL(a,b)
  return a*b
end

function STfunc.DIV(a,b)
  return a/b
end

return STfunc