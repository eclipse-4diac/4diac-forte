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
}

#include <string>

#include "../../../core/src/lua/luaengine.h"

#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions/func_DELETE.h"
#include "forte/iec61131_functions/func_INSERT.h"
#include "forte/iec61131_functions/func_LEFT.h"
#include "forte/iec61131_functions/func_MID.h"
#include "forte/iec61131_functions/func_REPLACE.h"
#include "forte/iec61131_functions/func_RIGHT.h"

namespace forte::lua::test {

  namespace {
    //! Runs a chunk that checks STfunc with check(actual, expected), the error names the failing line
    void runChecks(const char *paChecks) {
      CLuaEngine engine;
      std::string error;
      const std::string source = std::string("local STfunc = require 'STfunc'\n"
                                             "local function check(actual, expected)\n"
                                             "  if actual ~= expected then\n"
                                             "    error(tostring(actual) .. ' ~= ' .. tostring(expected), 2)\n"
                                             "  end\n"
                                             "end\n") +
                                 paChecks + "\nreturn {}";
      BOOST_TEST(engine.loadModule(source, "Checks", error), error);
      lua_settop(engine.getState(), 0);
    }

    //! Calls STfunc.paName with the string and integer arguments
    std::string callStringFunction(CLuaEngine &paEngine,
                                   const char *paName,
                                   std::initializer_list<const char *> paStrings,
                                   std::initializer_list<int> paIntegers) {
      lua_State *const state = paEngine.getState();
      paEngine.pushSTfunc();
      lua_getfield(state, -1, paName);
      lua_remove(state, -2);
      for (const char *string : paStrings) {
        lua_pushstring(state, string);
      }
      for (const int integer : paIntegers) {
        lua_pushinteger(state, integer);
      }
      std::string error;
      BOOST_REQUIRE_MESSAGE(paEngine.call(static_cast<int>(paStrings.size() + paIntegers.size()), 1, error), error);
      std::string result(lua_tostring(state, -1));
      lua_pop(state, 1);
      return result;
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaSTfuncTests)

  BOOST_AUTO_TEST_CASE(stringFunctionsTakeIecArguments) {
    runChecks(R"lua(
check(STfunc.LEFT('abcdef', 2), 'ab')
check(STfunc.LEFT('abc', 0), '')
check(STfunc.RIGHT('abcdef', 2), 'ef')
check(STfunc.RIGHT('abcdef', 0), '')
check(STfunc.MID('abcdef', 2, 3), 'cd')
check(STfunc.MID('abcdef', 10, 5), 'ef')
check(STfunc.INSERT('abcdef', 'XY', 2), 'abXYcdef')
check(STfunc.DELETE('abcdef', 2, 3), 'abef')
check(STfunc.REPLACE('abcdef', 'XY', 3, 2), 'aXYef')
check(STfunc.FIND('abcabc', 'ca'), 3)
check(STfunc.FIND('abc', 'x'), 0)
check(STfunc.CONCAT('a', 'b', 'c'), 'abc')
check(STfunc.LEN('abc'), 3)
check(STfunc.WMID('aäbc', 2, 2), 'ä')
check(STfunc.WLEN('ä'), 2)
)lua");
  }

  BOOST_AUTO_TEST_CASE(stringFunctionsMatchCompiledFunctions) {
    CLuaEngine engine;
    const char *const text = "abcdef";
    const CIEC_STRING iecText(text, 6);
    for (int l = -1; l <= 7; ++l) {
      BOOST_TEST_CONTEXT("L=" << l) {
        BOOST_TEST(callStringFunction(engine, "LEFT", {text}, {l}) ==
                   func_LEFT(iecText, CIEC_INT(static_cast<TForteInt16>(l))).getStorage());
        BOOST_TEST(callStringFunction(engine, "RIGHT", {text}, {l}) ==
                   func_RIGHT(iecText, CIEC_INT(static_cast<TForteInt16>(l))).getStorage());
      }
      for (int p = -1; p <= 7; ++p) {
        BOOST_TEST_CONTEXT("L=" << l << " P=" << p) {
          const CIEC_INT iecL(static_cast<TForteInt16>(l));
          const CIEC_INT iecP(static_cast<TForteInt16>(p));
          BOOST_TEST(callStringFunction(engine, "MID", {text}, {l, p}) == func_MID(iecText, iecL, iecP).getStorage());
          BOOST_TEST(callStringFunction(engine, "DELETE", {text}, {l, p}) ==
                     func_DELETE(iecText, iecL, iecP).getStorage());
          BOOST_TEST(callStringFunction(engine, "REPLACE", {text, "XY"}, {l, p}) ==
                     func_REPLACE(iecText, CIEC_STRING("XY", 2), iecL, iecP).getStorage());
        }
      }
    }
    for (int p = -1; p <= 7; ++p) {
      BOOST_TEST_CONTEXT("P=" << p) {
        BOOST_TEST(callStringFunction(engine, "INSERT", {text, "XY"}, {p}) ==
                   func_INSERT(iecText, CIEC_STRING("XY", 2), CIEC_INT(static_cast<TForteInt16>(p))).getStorage());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(bitFunctionsAreUnsigned) {
    runChecks(R"lua(
check(STfunc.NOT32(0), 4294967295)
check(STfunc.SHL32(1, 31), 2147483648)
check(STfunc.SHL32(1, 32), 0)
check(STfunc.SHR32(4294967295, 28), 15)
check(STfunc.ROL32(2147483648, 1), 1)
check(STfunc.ROR32(1, 1), 2147483648)
check(STfunc.AND(4294967295, 4294967040), 4294967040)
check(STfunc.OR(2147483648, 1), 2147483649)
check(STfunc.XOR(4294967295, 1), 4294967294)
check(STfunc.SHL8(255, 8), 0)
check(STfunc.SHL8(129, 1), 2)
check(STfunc.SHR16(65535, 16), 0)
check(STfunc.ROL8(129, 1), 3)
check(STfunc.SHL64(1, 40), 2 ^ 40)
check(STfunc.SHL64(1, 64), 0)
check(STfunc.SHR64(2 ^ 40, 40), 1)
check(STfunc.NOT8(1), 254)
)lua");
  }

  BOOST_AUTO_TEST_CASE(bitFunctionsKeepHighWords) {
    runChecks(R"lua(
check(STfunc.SHL64(2 ^ 32 + 1, 4), 2 ^ 36 + 16)
check(STfunc.SHL64(3, 31), 2 ^ 32 + 2 ^ 31)
check(STfunc.SHR64(2 ^ 40 + 256, 8), 2 ^ 32 + 1)
check(STfunc.SHR64(2 ^ 52, 33), 2 ^ 19)
check(STfunc.ROL64(2 ^ 32, 4), 2 ^ 36)
check(STfunc.ROR64(2 ^ 36, 4), 2 ^ 32)
check(STfunc.AND64(2 ^ 40 + 255, 2 ^ 40 + 15), 2 ^ 40 + 15)
check(STfunc.OR64(2 ^ 33, 1), 2 ^ 33 + 1)
check(STfunc.XOR64(2 ^ 33 + 1, 1), 2 ^ 33)
)lua");
  }

  BOOST_AUTO_TEST_CASE(numericFunctions) {
    runChecks(R"lua(
check(STfunc.MOD(7, 3), 1)
check(STfunc.MOD(-7, 3), -1)
check(STfunc.MOD(7, -3), 1)
check(STfunc.MOD(7, 0), 0)
check(STfunc.TRUNC(2.7), 2)
check(STfunc.TRUNC(-2.7), -2)
check(STfunc.LIMIT(0, 5, 3), 3)
check(STfunc.SEL(true, 1, 2), 2)
check(STfunc.MUX(1, 10, 20, 30), 20)
check(STfunc.ADD(1, 2, 3), 6)
check(STfunc.GT(3, 2, 1), true)
check(STfunc.VAL_TO_BYTE(0x1234), 0x34)
check(STfunc.VAL_TO_WORD(0x12345678), 0x5678)
check(STfunc.VAL_TO_DWORD(-1), 4294967295)
local original = STfunc.array({{1, 2}}, 0, {5, 6})
local copy = STfunc.MOVE(original)
copy[1] = 9
check(original[1], 5)
check(copy.lo, 1)
check(copy[2], 6)
)lua");
  }

  BOOST_AUTO_TEST_CASE(arraysAndCharacters) {
    runChecks(R"lua(
local matrix = STfunc.array({{0, 1}, {2, 4}}, 7, {{2, {1, 2, 3}}})
check(STfunc.LOWER_BOUND(matrix, 2), 2)
check(STfunc.UPPER_BOUND(matrix, 2), 4)
check(matrix[0][2], 1)
check(matrix[1][4], 3)
check(STfunc.WCHAR_TO_BYTE('A'), 65)
check(STfunc.WCHAR_TO_BYTE('€'), 0xAC)
check(STfunc.CHAR_TO_WCHAR('A'), 'A')
check(STfunc.CHAR_TO_WCHAR(string.char(0xE4)), 'ä')
check(STfunc.STRING_TO_WSTRING('abc'), 'abc')
check(STfunc.WSTRING_TO_STRING('abc'), 'abc')
)lua");
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
