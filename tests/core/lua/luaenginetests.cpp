/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <cmath>
#include <string>

#include "../../../core/src/lua/luaengine.h"

#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_wchar.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/typelib.h"

using namespace forte::literals;

namespace forte::lua::test {
  class CIEC_LuaEngineTestStruct : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(LuaEngineTestStruct);

    public:
      CIEC_INT Count;
      CIEC_STRING Text;
      CIEC_ARRAY_FIXED<CIEC_BOOL, 1, 2> Flags;

      CIEC_LuaEngineTestStruct() = default;

      size_t getStructSize() const override {
        return 3;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override {
        return "lua::test::LuaEngineTestStruct"_STRID;
      }

      CIEC_ANY *getMember(size_t paMemberIndex) override {
        switch (paMemberIndex) {
          case 0: return &Count;
          case 1: return &Text;
          case 2: return &Flags;
        }
        return nullptr;
      }

      const CIEC_ANY *getMember(size_t paMemberIndex) const override {
        switch (paMemberIndex) {
          case 0: return &Count;
          case 1: return &Text;
          case 2: return &Flags;
        }
        return nullptr;
      }

    private:
      static const StringId scmElementNames[];
  };

  const StringId CIEC_LuaEngineTestStruct::scmElementNames[] = {"Count"_STRID, "Text"_STRID, "Flags"_STRID};

  DEFINE_FIRMWARE_DATATYPE(LuaEngineTestStruct, "lua::test::LuaEngineTestStruct"_STRID)

  namespace {
    // Loads a module and calls its field f with the values pushed by paPushArgs
    bool callModuleFunction(CLuaEngine &paEngine, const char *paSource, int paNumArgs, int paNumResults) {
      std::string error;
      lua_State *state = paEngine.getState();
      if (!paEngine.loadModule(paSource, "test", error)) {
        BOOST_TEST_MESSAGE(error);
        return false;
      }
      lua_getfield(state, -1, "f");
      lua_remove(state, -2);
      lua_insert(state, -1 - paNumArgs);
      if (!paEngine.call(paNumArgs, paNumResults, error)) {
        BOOST_TEST_MESSAGE(error);
        return false;
      }
      return true;
    }

    int writeChunk(lua_State *, const void *paData, size_t paSize, void *paTarget) {
      static_cast<std::string *>(paTarget)->append(static_cast<const char *>(paData), paSize);
      return 0;
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaEngineTests)

  BOOST_AUTO_TEST_CASE(moduleReturnsTable) {
    CLuaEngine engine;
    std::string error;
    BOOST_REQUIRE(engine.isValid());
    BOOST_TEST(engine.loadModule("return {answer = 42}", "Answer", error));
    lua_getfield(engine.getState(), -1, "answer");
    BOOST_TEST(lua_tonumber(engine.getState(), -1) == 42);
    lua_settop(engine.getState(), 0);
  }

  BOOST_AUTO_TEST_CASE(moduleMustReturnTable) {
    CLuaEngine engine;
    std::string error;
    BOOST_TEST(!engine.loadModule("return 1", "One", error));
    BOOST_TEST(error == "the module does not return a table");
    BOOST_TEST(lua_gettop(engine.getState()) == 0);
  }

  BOOST_AUTO_TEST_CASE(syntaxErrorNamesChunk) {
    CLuaEngine engine;
    std::string error;
    BOOST_TEST(!engine.loadModule("return {", "Broken", error));
    BOOST_TEST(error.rfind("Broken:1:", 0) == 0);
    BOOST_TEST(lua_gettop(engine.getState()) == 0);
  }

  BOOST_AUTO_TEST_CASE(runtimeErrorHasTraceback) {
    CLuaEngine engine;
    std::string error;
    BOOST_TEST(!engine.loadModule("local function g() error('boom') end\ng()\nreturn {}", "Fails", error));
    BOOST_TEST(error.find("Fails:1: boom") != std::string::npos);
    BOOST_TEST(error.find("stack traceback:") != std::string::npos);
    BOOST_TEST(lua_gettop(engine.getState()) == 0);
  }

  BOOST_AUTO_TEST_CASE(deepTracebackIsShortened) {
    CLuaEngine engine;
    std::string error;
    BOOST_TEST(!engine.loadModule("local function f(n)\n"
                                  "  if n == 0 then error('deep') end\n"
                                  "  return 1 + f(n - 1)\n"
                                  "end\n"
                                  "f(40)\n"
                                  "return {}",
                                  "Deep", error));
    BOOST_TEST(error.rfind("Deep:2: deep\nstack traceback:\n\t[C]: in function 'error'\n", 0) == 0, error);
    BOOST_TEST(error.find("\n\t...\n") != std::string::npos, error);
    std::size_t lines = 0;
    for (std::size_t pos = error.find("\n\t"); pos != std::string::npos; pos = error.find("\n\t", pos + 1)) {
      ++lines;
    }
    // the first 11 levels, the ellipsis and the last 10 levels
    BOOST_TEST(lines == 22U, error);
    BOOST_TEST(lua_gettop(engine.getState()) == 0);
  }

  BOOST_AUTO_TEST_CASE(nonStringErrorHasTraceback) {
    CLuaEngine engine;
    std::string error;
    BOOST_TEST(!engine.loadModule("error({})\nreturn {}", "Table", error));
    BOOST_TEST(error.rfind("error object is not a string\nstack traceback:", 0) == 0, error);
  }

  BOOST_AUTO_TEST_CASE(binaryChunkRejected) {
    CLuaEngine engine;
    std::string error;
    std::string bytecode;
    lua_State *state = engine.getState();
    BOOST_REQUIRE(luaL_loadstring(state, "return {}") == 0);
    BOOST_REQUIRE(lua_dump(state, writeChunk, &bytecode) == 0);
    lua_settop(state, 0);
    BOOST_TEST(!engine.loadModule(bytecode, "Binary", error));
    BOOST_TEST(error.find("binary chunk") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(sandboxHidesUnsafeLibraries) {
    CLuaEngine engine;
    std::string error;
    lua_State *state = engine.getState();
    BOOST_REQUIRE(engine.loadModule(
        "return {n = (io and 1 or 0) + (os and 1 or 0) + (debug and 1 or 0) + (package and 1 or 0)"
        " + (jit and 1 or 0) + (ffi and 1 or 0) + (load and 1 or 0) + (loadstring and 1 or 0)"
        " + (dofile and 1 or 0) + (getmetatable and 1 or 0) + (getfenv and 1 or 0) + (setfenv and 1 or 0)"
        " + (string.dump and 1 or 0) + (coroutine and 1 or 0),"
        " ok = pcall(require, 'ffi'), self = (_G.math == math)}",
        "Sandbox", error));
    lua_getfield(state, -1, "n");
    BOOST_TEST(lua_tonumber(state, -1) == 0);
    lua_getfield(state, -2, "ok");
    BOOST_TEST(!lua_toboolean(state, -1));
    lua_getfield(state, -3, "self");
    BOOST_TEST(lua_toboolean(state, -1));
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(modulesDoNotShareGlobals) {
    CLuaEngine engine;
    std::string error;
    lua_State *state = engine.getState();
    BOOST_REQUIRE(engine.loadModule("leaked = 1\nstring.upper = nil\nmath.pi = 3\nreturn {}", "Writer", error));
    BOOST_REQUIRE(
        engine.loadModule("return {leaked = leaked, upper = string.upper('a'), pi = math.pi}", "Reader", error));
    lua_getfield(state, -1, "leaked");
    BOOST_TEST(lua_isnil(state, -1));
    lua_getfield(state, -2, "upper");
    BOOST_TEST(std::string(lua_tostring(state, -1)) == "A");
    lua_getfield(state, -3, "pi");
    BOOST_TEST(lua_tonumber(state, -1) > 3.14);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(stfuncIsAvailableAndReadOnly) {
    CLuaEngine engine;
    std::string error;
    lua_State *state = engine.getState();
    BOOST_REQUIRE(engine.loadModule("local STfunc = require 'STfunc'\n"
                                    "return {limit = STfunc.LIMIT(0, 5, 3), same = (STfunc == require('STfunc')),"
                                    " packed = select('#', table.unpack({1, 2, 3}))}",
                                    "UsesSTfunc", error));
    lua_getfield(state, -1, "limit");
    BOOST_TEST(lua_tonumber(state, -1) == 3);
    lua_getfield(state, -2, "same");
    BOOST_TEST(lua_toboolean(state, -1));
    lua_getfield(state, -3, "packed");
    BOOST_TEST(lua_tonumber(state, -1) == 3);
    lua_settop(state, 0);

    BOOST_TEST(!engine.loadModule("require('STfunc').LIMIT = nil\nreturn {}", "Tamper", error));
    BOOST_TEST(error.find("attempt to modify STfunc") != std::string::npos);
  }

  BOOST_AUTO_TEST_CASE(everySTfuncNameLoadsOnDemand) {
    CLuaEngine engine;
    std::string error;
    std::string source = "local STfunc = require 'STfunc'\n";
    for (const std::string_view name : getSTfuncNames()) {
      source += "assert(type(STfunc." + std::string(name) + ") == 'function', '" + std::string(name) + "')\n";
    }
    source += "return {alias = (STfunc.WLEN == STfunc.LEN), recursive = STfunc.MOVE({{1}})[1][1],"
              " unknown = (STfunc.NOT_A_FUNCTION == nil and STfunc[1] == nil)}";
    BOOST_TEST(getSTfuncNames().size() > 70);
    BOOST_REQUIRE_MESSAGE(engine.loadModule(source, "AllSTfunc", error), error);
    lua_State *state = engine.getState();
    lua_getfield(state, -1, "alias");
    BOOST_TEST(lua_toboolean(state, -1));
    lua_getfield(state, -2, "recursive");
    BOOST_TEST(lua_tonumber(state, -1) == 1);
    lua_getfield(state, -3, "unknown");
    BOOST_TEST(lua_toboolean(state, -1));
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(stfuncErrorsKeepTheFileLines) {
    CLuaEngine engine;
    std::string error;
    // UPPER_BOUND raises 'dim < 1' on line 49 of STfunc.lua
    BOOST_TEST(!engine.loadModule("require('STfunc').UPPER_BOUND({lo = 1, up = 1}, 0)", "BadDim", error));
    BOOST_TEST(error.find("STfunc:49: dim < 1") != std::string::npos, error);
  }

  BOOST_AUTO_TEST_CASE(reusedStateKeepsNoModuleGarbage) {
    CLuaEngine engine;
    std::string error;
    lua_State *state = engine.getState();
    const auto loadModule = [&] {
      BOOST_REQUIRE(
          engine.loadModule("local t = {}\nfor i = 1, 1000 do t[i] = {i} end\nreturn {t = t}", "Garbage", error));
      lua_settop(state, 0);
    };
    // the first module grows the state's internal tables, which stay for the next ones
    loadModule();
    engine.collectGarbage();
    const int before = lua_gc(state, LUA_GCCOUNT, 0);
    for (int i = 0; i < 20; ++i) {
      loadModule();
    }
    engine.collectGarbage();
    BOOST_TEST(lua_gc(state, LUA_GCCOUNT, 0) <= before);
  }

  BOOST_AUTO_TEST_CASE(callReturnsResults) {
    CLuaEngine engine;
    lua_pushnumber(engine.getState(), 21);
    BOOST_REQUIRE(callModuleFunction(engine, "return {f = function(a) return a * 2, 'x' end}", 1, 2));
    BOOST_TEST(lua_tonumber(engine.getState(), -2) == 42);
    BOOST_TEST(std::string(lua_tostring(engine.getState(), -1)) == "x");
    lua_settop(engine.getState(), 0);
  }

  BOOST_AUTO_TEST_CASE(refsKeepValues) {
    CLuaEngine engine;
    lua_pushstring(engine.getState(), "kept");
    const int ref = engine.ref();
    BOOST_TEST(lua_gettop(engine.getState()) == 0);
    engine.pushRef(ref);
    BOOST_TEST(std::string(lua_tostring(engine.getState(), -1)) == "kept");
    engine.unref(ref);
    lua_settop(engine.getState(), 0);
  }

  BOOST_AUTO_TEST_CASE(instructionLimitStopsEndlessLoop) {
    CLuaEngine engine(100000);
    std::string error;
    BOOST_TEST(!engine.loadModule("while true do end", "Endless", error));
    BOOST_TEST(error.find("instruction limit exceeded") != std::string::npos);
    BOOST_TEST(!engine.loadModule("while true do pcall(function() while true do end end) end", "Swallow", error));
    BOOST_TEST(error.find("instruction limit exceeded") != std::string::npos);
    BOOST_TEST(engine.loadModule("local n = 0\nfor i = 1, 1000 do n = n + i end\nreturn {n = n}", "Bounded", error));
    lua_settop(engine.getState(), 0);
  }

  BOOST_AUTO_TEST_CASE(elementaryRoundTrips) {
    CLuaEngine engine;
    lua_State *state = engine.getState();

    auto roundTrip = [state](const CIEC_ANY &paIn, CIEC_ANY &paOut) {
      CLuaEngine::pushValue(state, paIn);
      const char *error = CLuaEngine::getValue(state, -1, paOut);
      lua_settop(state, 0);
      return error == nullptr;
    };

    CIEC_BOOL boolOut;
    BOOST_TEST(roundTrip(CIEC_BOOL(true), boolOut));
    BOOST_TEST(static_cast<bool>(boolOut));
    CIEC_SINT sintOut;
    BOOST_TEST(roundTrip(CIEC_SINT(-7), sintOut));
    BOOST_TEST(static_cast<TForteInt8>(sintOut) == -7);
    CIEC_LWORD lwordOut;
    BOOST_TEST(roundTrip(CIEC_LWORD(0x0010000000000001ULL), lwordOut));
    BOOST_TEST(static_cast<TForteLWord>(lwordOut) == 0x0010000000000001ULL);
    CIEC_REAL realOut;
    BOOST_TEST(roundTrip(CIEC_REAL(1.5f), realOut));
    BOOST_TEST(static_cast<TForteFloat>(realOut) == 1.5f);
    CIEC_LREAL lrealOut;
    BOOST_TEST(roundTrip(CIEC_LREAL(-2.25), lrealOut));
    BOOST_TEST(static_cast<TForteDFloat>(lrealOut) == -2.25);
    CIEC_TIME timeOut;
    BOOST_TEST(roundTrip(CIEC_TIME(1500000000), timeOut));
    BOOST_TEST(static_cast<CIEC_TIME::TValueType>(timeOut) == 1500000000);
    CIEC_CHAR charOut;
    BOOST_TEST(roundTrip(CIEC_CHAR('Z'), charOut));
    BOOST_TEST(static_cast<TForteChar>(charOut) == 'Z');
    CIEC_WCHAR wcharOut;
    BOOST_TEST(roundTrip(CIEC_WCHAR(static_cast<TForteWChar>(0x20AC)), wcharOut));
    BOOST_TEST(static_cast<unsigned int>(static_cast<TForteWChar>(wcharOut)) == 0x20ACU);
    CIEC_STRING stringOut;
    BOOST_TEST(roundTrip(CIEC_STRING("hello", 5), stringOut));
    BOOST_TEST(stringOut.getStorage() == "hello");
    CIEC_WSTRING wstringOut;
    BOOST_TEST(roundTrip(CIEC_WSTRING("\xE2\x82\xAC 5"), wstringOut));
    BOOST_TEST(std::string(wstringOut.getValue()) == "\xE2\x82\xAC 5");
  }

  BOOST_AUTO_TEST_CASE(timeIsNanoseconds) {
    CLuaEngine engine;
    CLuaEngine::pushValue(engine.getState(), CIEC_TIME(1500000000));
    BOOST_TEST(lua_tonumber(engine.getState(), -1) == 1500000000.0);
    lua_settop(engine.getState(), 0);
  }

  BOOST_AUTO_TEST_CASE(integersWrapLikeCasts) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_INT intValue;
    lua_pushnumber(state, 40000);
    BOOST_TEST(CLuaEngine::getValue(state, -1, intValue) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(intValue) == -25536);
    lua_pushnumber(state, -2.9);
    BOOST_TEST(CLuaEngine::getValue(state, -1, intValue) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(intValue) == -2);
    CIEC_UDINT udintValue;
    lua_pushnumber(state, -1);
    BOOST_TEST(CLuaEngine::getValue(state, -1, udintValue) == nullptr);
    BOOST_TEST(static_cast<TForteUInt32>(udintValue) == 4294967295U);
    CIEC_ULINT ulintValue;
    lua_pushnumber(state, 18446744073709549568.0);
    BOOST_TEST(CLuaEngine::getValue(state, -1, ulintValue) == nullptr);
    BOOST_TEST(static_cast<TForteUInt64>(ulintValue) == 18446744073709549568ULL);
    CIEC_DWORD dwordValue;
    lua_pushnumber(state, -2147483648.0); // a signed result of the bit library
    BOOST_TEST(CLuaEngine::getValue(state, -1, dwordValue) == nullptr);
    BOOST_TEST(static_cast<TForteDWord>(dwordValue) == 0x80000000U);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(wrongLuaTypeIsRejected) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_INT intValue(5);
    lua_pushstring(state, "12");
    BOOST_TEST(CLuaEngine::getValue(state, -1, intValue) != nullptr);
    BOOST_TEST(static_cast<TForteInt16>(intValue) == 5);
    lua_pushnumber(state, std::nan(""));
    BOOST_TEST(CLuaEngine::getValue(state, -1, intValue) != nullptr);
    CIEC_STRING stringValue("keep", 4);
    lua_pushnumber(state, 1);
    BOOST_TEST(CLuaEngine::getValue(state, -1, stringValue) != nullptr);
    BOOST_TEST(stringValue.getStorage() == "keep");
    CIEC_BOOL boolValue;
    lua_pushstring(state, "true");
    BOOST_TEST(CLuaEngine::getValue(state, -1, boolValue) != nullptr);
    CIEC_CHAR charValue;
    lua_pushstring(state, "ab");
    BOOST_TEST(CLuaEngine::getValue(state, -1, charValue) != nullptr);
    CIEC_WSTRING wstringValue;
    lua_pushstring(state, "\xF0\x9F\x98\x80");
    BOOST_TEST(CLuaEngine::getValue(state, -1, wstringValue) != nullptr);
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 1> arrayValue;
    lua_pushnumber(state, 1);
    BOOST_TEST(CLuaEngine::getValue(state, -1, arrayValue) != nullptr);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(arrayKeepsIecBounds) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_ARRAY_FIXED<CIEC_INT, 5, 7> array;
    array[5] = CIEC_INT(50);
    array[6] = CIEC_INT(60);
    array[7] = CIEC_INT(70);
    CLuaEngine::pushValue(state, array);
    BOOST_REQUIRE(
        callModuleFunction(engine,
                           "local S = require 'STfunc'\n"
                           "return {f = function(t) t[6] = t[6] + 1\n"
                           "  return t, t.lo, t.up, t[5], t[7], t[4], S.LOWER_BOUND(t, 1), S.UPPER_BOUND(t, 1) end}",
                           1, 8));
    BOOST_TEST(lua_tonumber(state, 2) == 5);
    BOOST_TEST(lua_tonumber(state, 3) == 7);
    BOOST_TEST(lua_tonumber(state, 4) == 50);
    BOOST_TEST(lua_tonumber(state, 5) == 70);
    BOOST_TEST(lua_isnil(state, 6));
    BOOST_TEST(lua_tonumber(state, 7) == 5);
    BOOST_TEST(lua_tonumber(state, 8) == 7);
    BOOST_TEST(CLuaEngine::getValue(state, 1, array) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(array[6]) == 61);
    BOOST_TEST(static_cast<TForteInt16>(array[5]) == 50);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(zeroBasedArrayRoundTrip) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_ARRAY_FIXED<CIEC_DINT, 0, 3> in;
    CIEC_ARRAY_FIXED<CIEC_DINT, 0, 3> out;
    for (intmax_t i = 0; i <= 3; ++i) {
      in[i] = CIEC_DINT(static_cast<TForteInt32>(i * 10 - 5));
    }
    CLuaEngine::pushValue(state, in);
    lua_rawgeti(state, -1, 0);
    BOOST_TEST(lua_tonumber(state, -1) == -5);
    lua_pop(state, 1);
    BOOST_TEST(CLuaEngine::getValue(state, -1, out) == nullptr);
    for (intmax_t i = 0; i <= 3; ++i) {
      BOOST_TEST(static_cast<TForteInt32>(out[i]) == static_cast<TForteInt32>(in[i]));
    }
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(nilArrayElementLeavesValue) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_ARRAY_FIXED<CIEC_INT, 1, 2> array;
    array[1] = CIEC_INT(1);
    array[2] = CIEC_INT(2);
    lua_newtable(state);
    lua_pushnumber(state, 20);
    lua_rawseti(state, -2, 2);
    BOOST_TEST(CLuaEngine::getValue(state, -1, array) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(array[1]) == 1);
    BOOST_TEST(static_cast<TForteInt16>(array[2]) == 20);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_CASE(structByMemberName) {
    CLuaEngine engine;
    lua_State *state = engine.getState();
    CIEC_LuaEngineTestStruct structure;
    structure.Count = CIEC_INT(3);
    structure.Text = CIEC_STRING("abc", 3);
    structure.Flags[2] = CIEC_BOOL(true);
    CLuaEngine::pushValue(state, structure);
    BOOST_REQUIRE(callModuleFunction(engine,
                                     "return {f = function(s)\n"
                                     "  local r = {Count = s.Count + 1, Text = s.Text .. 'd',"
                                     " Flags = {[1] = true, [2] = s.Flags[2]}}\n"
                                     "  return r, s.Flags.lo end}",
                                     1, 2));
    BOOST_TEST(lua_tonumber(state, 2) == 1);
    CIEC_LuaEngineTestStruct out;
    BOOST_TEST(CLuaEngine::getValue(state, 1, out) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(out.Count) == 4);
    BOOST_TEST(out.Text.getStorage() == "abcd");
    BOOST_TEST(static_cast<bool>(out.Flags[1]));
    BOOST_TEST(static_cast<bool>(out.Flags[2]));
    lua_settop(state, 0);

    // a missing member keeps its value, a member of the wrong kind is an error
    lua_newtable(state);
    lua_pushnumber(state, 9);
    lua_setfield(state, -2, "Count");
    BOOST_TEST(CLuaEngine::getValue(state, -1, out) == nullptr);
    BOOST_TEST(static_cast<TForteInt16>(out.Count) == 9);
    BOOST_TEST(out.Text.getStorage() == "abcd");
    lua_pushboolean(state, 1);
    lua_setfield(state, -2, "Text");
    BOOST_TEST(CLuaEngine::getValue(state, -1, out) != nullptr);
    lua_settop(state, 0);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
