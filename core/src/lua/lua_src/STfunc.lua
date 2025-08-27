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

-- Function Wrapper
function STfunc.wrapfunc(f, fb, env, list, ...)
  local args = {...}
  local res = {f(fb, unpack(args))} -- table.unpack in Lua version >= 5.2
  for i, v in ipairs(list) do
    env[v] = res[i+1]
  end
  return res[1]
end

-- Array Upper and Lower Bound
function STfunc.UPPER_BOUND(arr, dim)
  if dim < 1 then
    error('dim < 1')
  end
  while dim > 1 do
    arr = arr[arr.lo]
    dim = dim - 1
  end
  return arr.up
end

function STfunc.LOWER_BOUND(arr, dim)
  if dim < 1 then
    error('dim < 1')
  end
  while dim > 1 do
    arr = arr[arr.lo]
    dim = dim - 1
  end
  return arr.lo
end

-- Array generation
local InitRange = {ind = 1, rloop = 1, rind = 1, init = {}, def = nil}

function InitRange:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function InitRange:next()
  local val = nil
  if self.init[self.ind] == nil then
    return self.def
  end
  if type(self.init[self.ind]) == 'table' then
    val = self.init[self.ind][2][self.rind]
    self.rind = self.rind + 1
    if self.rind > #self.init[self.ind][2] then
      self.rind = 1
      self.rloop = self.rloop + 1
    end
    if self.rloop > self.init[self.ind][1] then
      self.rloop = 1
      self.ind = self.ind + 1
    end
  else
    val = self.init[self.ind]
    self.ind = self.ind + 1
  end
  return val
end

local function arraygen(ranges, ini)
  local lo = ranges[1][1]
  local up = ranges[1][2]
  local arr = {lo = lo, up = up}
  if #ranges > 1 then
    local subranges = {table.unpack(ranges, 2)}
    for i = lo, up do
      arr[i] = arraygen(subranges, ini)
    end
  else
    for i = lo, up do
      arr[i] = ini:next()
    end
  end
  return arr
end

function STfunc.array(ranges, def, init)
  local ini = InitRange:new{init = init or {}, def = def}
  return arraygen(ranges, ini)
end

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
  local arr = {...}
  local n = #(arr)
  if k < 0 or k > n-1 then
    return nil
  end
  return arr[k+1]
end

--Comparison functions
function STfunc.GT(...)
  local arr = {...}
  local n = #(arr)
  local last = arr[1]
  for i = 2, n do
    if last <= arr[i] then -- not last > arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.GE(...)
  local arr = {...}
  local n = #(arr)
  local last = arr[1]
  for i = 2, n do
    if last < arr[i] then -- not last >= arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.LT(...)
  local arr = {...}
  local n = #(arr)
  local last = arr[1]
  for i = 2, n do
    if last >= arr[i] then -- not last < arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.LE(...)
  local arr = {...}
  local n = #(arr)
  local last = arr[1]
  for i = 2, n do
    if last > arr[i] then -- not last <= arr[i]
      return false
    end
    last = arr[i]
  end
  return true
end

function STfunc.EQ(...)
  local arr = {...}
  local n = #(arr)
  local last = arr[1]
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
  local ind, _ = string.find(s1, s2, 1, true)
  if ind == nil then
    return 0
  end
  return ind
end

--WString functions
function STfunc.WLEN(s)
  return math.floor(#(s) / 2)
end

function STfunc.WLEFT(s, p)
  return string.sub(s, 1, p*2)
end

function STfunc.WRIGHT(s, p)
  return string.sub(s, -p*2)
end

function STfunc.WMID(s, p, n)
  return string.sub(s, p*2 - 1, p*2 + n*2 - 2)
end

function STfunc.WCONCAT(...)
  return table.concat({...})
end

function STfunc.WINSERT(s1, s2, p)
  return string.sub(s1, 1, p*2-2) .. s2 .. string.sub(s1, p*2-1)
end

function STfunc.WDELETE(s1, p, n)
  return string.sub(s1, 1, p*2-2) .. string.sub(s1, p*2 + n*2-1)
end

function STfunc.WREPLACE(s1, s2, p, n)
  return string.sub(s1, 1, p*2-2) .. s2 .. string.sub(s1, p*2 + n*2 - 1)
end

function STfunc.WFIND(s1, s2)
  local ind, _ = string.find(s1, s2, 1, true)
  while ind ~= nil and ind % 2 == 0 do
    ind, _ = string.find(s1, s2, ind + 1, true)
  end
  if ind == nil then
    return 0
  end
  return math.floor((ind + 1) / 2)
end

--Conversion functions
function STfunc.BOOL_TO_INT(b)
  if b then
    return 1
  else
    return 0
  end
end

function STfunc.BYTE_TO_BOOL(b)
  return bit.band(b, 1) == 1
end

function STfunc.WCHAR_TO_BYTE(c)
  return string.byte(c, 1, 1) * 256 + string.byte(c, 2, 2)
end

function STfunc.CHAR_TO_WCHAR(c)
  return string.char(0) .. c
end

function STfunc.VAL_TO_BYTE(n)
  return bit.bor(n, 0x000000ff)
end

function STfunc.VAL_TO_WORD(n)
  return bit.bor(n, 0x0000ffff)
end

function STfunc.VAL_TO_DWORD(n)
  return bit.bor(n, 0xffffffff)
end

function STfunc.STRING_TO_WSTRING(s)
  return string.gsub(s, '.', function(c)
    return string.char(0) .. c
    end)
end

function STfunc.WSTRING_TO_STRING(s)
  return string.gsub(s, '..', function(c)
    return string.sub(c, 2, 2)
    end)
end

return STfunc