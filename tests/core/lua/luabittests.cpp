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

namespace forte::lua::test {

  namespace {
    //! Runs a module that checks the bit library with check(actual, expected), the error names the failing line
    void runChecks(const char *paChecks) {
      CLuaEngine engine;
      std::string error;
      const std::string source = std::string("local function check(actual, expected)\n"
                                             "  if actual ~= expected then\n"
                                             "    error(tostring(actual) .. ' ~= ' .. tostring(expected), 2)\n"
                                             "  end\n"
                                             "end\n") +
                                 paChecks + "\nreturn {}";
      BOOST_TEST(engine.loadModule(source, "Checks", error), error);
      lua_settop(engine.getState(), 0);
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaBitTests)

  BOOST_AUTO_TEST_CASE(argumentsAreTakenModulo32Bits) {
    runChecks(R"lua(
check(bit.tobit(4294967295), -1)
check(bit.tobit(4294967296), 0)
check(bit.tobit(2 ^ 40 + 1234), 1234)
check(bit.tobit(-1), -1)
check(bit.tobit(2147483648), -2147483648)
check(bit.tobit(1.4), 1)
check(bit.tobit(1.6), 2)
check(bit.tobit(-1.4), -1)
check(bit.tobit('12'), 12)
)lua");
  }

  BOOST_AUTO_TEST_CASE(logicalOperations) {
    runChecks(R"lua(
check(bit.bnot(0), -1)
check(bit.bnot(-1), 0)
check(bit.band(305419896, 255), 120)
check(bit.band(-1, 7, 3), 3)
check(bit.bor(1, 2, 4, 8), 15)
check(bit.bor(2147483648, 1), -2147483647)
check(bit.bxor(-1, 1), -2)
check(bit.bxor(1, 3, 2), 0)
)lua");
  }

  BOOST_AUTO_TEST_CASE(shiftsUseFiveBits) {
    runChecks(R"lua(
check(bit.lshift(1, 0), 1)
check(bit.lshift(1, 8), 256)
check(bit.lshift(1, 31), -2147483648)
check(bit.lshift(1, 40), 256)
check(bit.lshift(1, 32), 1)
check(bit.lshift(1, -1), -2147483648)
check(bit.rshift(256, 8), 1)
check(bit.rshift(-256, 8), 16777215)
check(bit.arshift(-256, 8), -1)
check(bit.arshift(256, 8), 1)
check(bit.arshift(-2147483648, 31), -1)
)lua");
  }

  BOOST_AUTO_TEST_CASE(rotationsAndByteSwap) {
    runChecks(R"lua(
check(bit.rol(305419896, 12), 1164411171)
check(bit.ror(305419896, 12), 1736516421)
check(bit.rol(305419896, 0), 305419896)
check(bit.rol(305419896, 32), 305419896)
check(bit.ror(1, 1), -2147483648)
check(bit.bswap(305419896), 2018915346)
check(bit.bswap(2596069104), -253838182)
)lua");
  }

  BOOST_AUTO_TEST_CASE(hexadecimalText) {
    runChecks(R"lua(
check(bit.tohex(65535), '0000ffff')
check(bit.tohex(-1), 'ffffffff')
check(bit.tohex(57005, -4), 'DEAD')
check(bit.tohex(305419896, 2), '78')
check(bit.tohex(1, 12), '00000001')
check(bit.tohex(1, 0), '')
)lua");
  }

  BOOST_AUTO_TEST_CASE(nonNumbersAreRejected) {
    runChecks(R"lua(
local ok, message = pcall(bit.band, 1, nil)
check(ok, false)
check(message:find('number expected', 1, true) ~= nil, true)
check(pcall(bit.tobit, {}), false)
check(pcall(bit.tohex), false)
)lua");
  }

  BOOST_AUTO_TEST_CASE(requireReturnsTheModuleCopy) {
    runChecks(R"lua(
check(require('bit'), bit)
bit.band = nil
check(require('bit').band, nil)
check(type(require('STfunc').AND), 'function')
)lua");
  }

  BOOST_AUTO_TEST_CASE(stfuncKeepsItsOwnBitLibrary) {
    runChecks(R"lua(
bit.bor = nil
check(require('STfunc').OR(2147483648, 1), 2147483649)
)lua");
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
