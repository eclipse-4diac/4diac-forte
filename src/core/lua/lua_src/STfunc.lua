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

local bit = require("bit")
local STfunc = {}

-- Arithmetic Functions
function STfunc.ADD(a, ...)
  local sum = a
  local args = {...}
  for i, val in ipairs(args) do
    sum = sum + val
  end
  return sum
end

function STfunc.MUL(a, ...)
  local prod = a
  local args = {...}
  for i, val in ipairs(args) do
    prod = prod * val
  end
  return prod
end

function STfunc.SUB(a, b)
  return a - b
end

function STfunc.DIV(a, b)
  return a / b
end

function STfunc.MOD(a, b)
  return a % b
end

function STfunc.EXPT(a, b)
  return a ^ b
end

-- Bit-Shift Functions
function STfunc.SHL1(a, b)
  if b > 0 then
    return 0
  else
    return a
  end
end

function STfunc.SHR1(a, b)
  if b > 0 then
    return 0
  else
    return a
  end
end

function STfunc.ROL1(a, b)
  return a
end

function STfunc.ROR1(a, b)
  return a
end

function STfunc.SHL8(a, b)
  return bit.band(bit.lshift(a, b), 0x000000ff)
end

function STfunc.SHR8(a, b)
  return bit.rshift(a, b)
end

function STfunc.ROL8(a, b)
  local r = b % 8
  return bit.band(bit.bor(bit.lshift(a, r), bit.rshift(a, 8-r)), 0x000000ff)
end

function STfunc.ROR8(a, b)
  local r = b % 8
  return bit.band(bit.bor(bit.rshift(a, r), bit.lshift(a, 8-r)), 0x000000ff)
end

function STfunc.SHL16(a, b)
  return bit.band(bit.lshift(a, b), 0x0000ffff)
end

function STfunc.SHR16(a, b)
  return bit.rshift(a, b)
end

function STfunc.ROL16(a, b)
  local r = b % 16
  return bit.band(bit.bor(bit.lshift(a, r), bit.rshift(a, 16-r)), 0x0000ffff)
end

function STfunc.ROR16(a, b)
  local r = b % 16
  return bit.band(bit.bor(bit.rshift(a, r), bit.lshift(a, 16-r)), 0x0000ffff)
end

function STfunc.SHL32(a, b)
  return bit.lshift(a, b)
end

function STfunc.SHR32(a, b)
  return bit.rshift(a, b)
end

function STfunc.ROL32(a, b)
  return bit.rol(a, b)
end

function STfunc.ROR32(a, b)
  return bit.ror(a, b)
end

function STfunc.SHL64(a, b)
  local al = a % 0x0000000100000000
  local ah = (a - al) / 0x0000000100000000
  local oh = 0
  local ol = 0
  if b == 0 then
    return a
  end
  if b < 32 then
    ol = bit.lshift(al, b)
    oh = bit.bor(bit.lshift(ah, b), bit.rshift(al, 32-b))
  else
    ol = 0
    oh = bit.lshift(al, b-32);
  end
  ol = ol % 0x0000000100000000
  return oh * 0x0000000100000000 + ol
end

function STfunc.SHR64(a,b)
  local al = a % 0x0000000100000000
  local ah = (a - al) / 0x0000000100000000
  local oh = 0
  local ol = 0
  if b == 0 then
    return a
  end
  if b < 32 then
    ol = bit.bor(bit.rshift(al, b), bit.lshift(ah, 32-b))
    oh = bit.rshift(ah, b)
  else
    ol = bit.rshift(ah, b-32)
    oh = 0;
  end
  ol = ol % 0x0000000100000000
  return oh * 0x0000000100000000 + ol
end

function STfunc.ROL64(a,b)
  local r = b % 64
  if r == 0 then
    return a
  end
  return STfunc.SHL64(a, r) + STfunc.SHR64(a, 64-r)
end

function STfunc.ROR64(a,b)
  local r = b % 64
  if r == 0 then
    return a
  end
  return STfunc.SHR64(a, r) + STfunc.SHL64(a, 64-r)
end

-- Bitwise Boolean Functions
function STfunc.AND(a, ...)
  local o = a
  local args = {...}
  for i, val in ipairs(args) do
    o = bit.band(o, val)
  end
  return o
end

function STfunc.OR(a, ...)
  local o = a
  local args = {...}
  for i, val in ipairs(args) do
    o = bit.bor(o, val)
  end
  return o
end

function STfunc.XOR(a, ...)
  local o = a
  local args = {...}
  for i, val in ipairs(args) do
    o = bit.bxor(o, val)
  end
  return o
end

function STfunc.AND64(a, ...)
  local ol = a % 0x0000000100000000
  local oh = (a - ol) / 0x0000000100000000
  local args = {...}
  for i, val in ipairs(args) do
    local vl = val % 0x0000000100000000
    local vh = (val - vl) / 0x0000000100000000
    ol = bit.band(ol, vl)
    oh = bit.band(oh, vh)
  end
  ol = ol % 0x0000000100000000
  return oh * 0x0000000100000000 + ol
end

function STfunc.OR64(a, ...)
  local ol = a % 0x0000000100000000
  local oh = (a - ol) / 0x0000000100000000
  local args = {...}
  for i, val in ipairs(args) do
    local vl = val % 0x0000000100000000
    local vh = (val - vl) / 0x0000000100000000
    ol = bit.bor(ol, vl)
    oh = bit.bor(oh, vh)
  end
  ol = ol % 0x0000000100000000
  return oh * 0x0000000100000000 + ol
end

function STfunc.XOR64(a, ...)
  local ol = a % 0x0000000100000000
  local oh = (a - ol) / 0x0000000100000000
  local args = {...}
  for i, val in ipairs(args) do
    local vl = val % 0x0000000100000000
    local vh = (val - vl) / 0x0000000100000000
    ol = bit.bxor(ol, vl)
    oh = bit.bxor(oh, vh)
  end
  ol = ol % 0x0000000100000000
  return oh * 0x0000000100000000 + ol
end

function STfunc.NOT1(a)
  return bit.band(bit.bnot(a), 0x00000001)
end

function STfunc.NOT8(a)
  return bit.band(bit.bnot(a), 0x000000ff)
end

function STfunc.NOT16(a)
  return bit.band(bit.bnot(a), 0x0000ffff)
end

function STfunc.NOT32(a)
  return bit.bnot(a)
end

function STfunc.NOT64(a)
  local al = a % 0x0000000100000000
  local ah = (a - al) / 0x0000000100000000
  al = bit.bnot(al)
  ah = bit.bnot(ah)
  al = al % 0x0000000100000000
  return ah * 0x0000000100000000 + al
end

--Selection functions
function STfunc.LIMIT(mn, input, mx)
  return math.min(math.max(input, mn), mx)
end

function STfunc.SEL(g, in0, in1)
  if g then
    return in1
  else
    return in0
  end
end

function STfunc.MUX(k, ...)
  arr = {...}
  n = #(arr)
  if k < 0 or k > n-1 then
    return nil
  end
  return arr[k+1]
end

--Comparison functions
function STfunc.GT(...)
  arr = {...}
  n = #(arr)
  last = arr[1]
  for i = 2, n do
    if last <= arr[i] then -- not last > arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.GE(...)
  arr = {...}
  n = #(arr)
  last = arr[1]
  for i = 2, n do
    if last < arr[i] then -- not last >= arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.LT(...)
  arr = {...}
  n = #(arr)
  last = arr[1]
  for i = 2, n do
    if last >= arr[i] then -- not last < arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.LE(...)
  arr = {...}
  n = #(arr)
  last = arr[1]
  for i = 2, n do
    if last > arr[i] then -- not last <= arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.EQ(...)
  arr = {...}
  n = #(arr)
  last = arr[1]
  for i = 2, n do
    if last ~= arr[i] then -- not last == arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.NE(a, b)
  return a ~= b
end

--String functions
function STfunc.LEN(s)
  return #(s)
end

function STfunc.LEFT(s, p)
  return string.sub(s, 1, p)
end

function STfunc.RIGHT(s, p)
  return string.sub(s, -p)
end

function STfunc.MID(s, p, n)
  return string.sub(s, p, p + n - 1)
end

function STfunc.CONCAT(...)
  return table.concat({...})
end

function STfunc.INSERT(s1, s2, p)
  return string.sub(s1, 1, p-1) .. s2 .. string.sub(s1, p)
end

function STfunc.DELETE(s1, p, n)
  return string.sub(s1, 1, p-1) .. string.sub(s1, p + n)
end

function STfunc.REPLACE(s1, s2, p, n)
  return string.sub(s1, 1, p-1) .. s2 .. string.sub(s1, p + n)
end

function STfunc.FIND(s1, s2)
  ind, _ = string.find(s1, s2, 1, true)
  if ind == nil then
    return 0
  end
  return ind
end

return STfunc