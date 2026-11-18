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

#include "luabit.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <bit>
#include <cstdint>

namespace forte::lua {
  namespace {
    // Only trivially destructible locals here, Lua errors leave these functions with longjmp

    //! The argument modulo 2^32: adding 2^52 + 2^51 moves the rounded integer into the low bits of the mantissa
    std::uint32_t toBits(lua_State *paState, int paIndex) {
      constexpr lua_Number cMagic = 6755399441055744.0;
      return static_cast<std::uint32_t>(std::bit_cast<std::uint64_t>(luaL_checknumber(paState, paIndex) + cMagic));
    }

    int pushBits(lua_State *paState, std::uint32_t paBits) {
      lua_pushnumber(paState, static_cast<lua_Number>(static_cast<std::int32_t>(paBits)));
      return 1;
    }

    unsigned int toShift(lua_State *paState, int paIndex) {
      return toBits(paState, paIndex) & 31U;
    }

    int toBit(lua_State *paState) {
      return pushBits(paState, toBits(paState, 1));
    }

    int bitNot(lua_State *paState) {
      return pushBits(paState, ~toBits(paState, 1));
    }

    int bitAnd(lua_State *paState) {
      std::uint32_t bits = toBits(paState, 1);
      for (int index = lua_gettop(paState); index > 1; --index) {
        bits &= toBits(paState, index);
      }
      return pushBits(paState, bits);
    }

    int bitOr(lua_State *paState) {
      std::uint32_t bits = toBits(paState, 1);
      for (int index = lua_gettop(paState); index > 1; --index) {
        bits |= toBits(paState, index);
      }
      return pushBits(paState, bits);
    }

    int bitXor(lua_State *paState) {
      std::uint32_t bits = toBits(paState, 1);
      for (int index = lua_gettop(paState); index > 1; --index) {
        bits ^= toBits(paState, index);
      }
      return pushBits(paState, bits);
    }

    int shiftLeft(lua_State *paState) {
      const std::uint32_t bits = toBits(paState, 1);
      return pushBits(paState, bits << toShift(paState, 2));
    }

    int shiftRight(lua_State *paState) {
      const std::uint32_t bits = toBits(paState, 1);
      return pushBits(paState, bits >> toShift(paState, 2));
    }

    int shiftRightArithmetic(lua_State *paState) {
      const auto bits = static_cast<std::int32_t>(toBits(paState, 1));
      return pushBits(paState, static_cast<std::uint32_t>(bits >> toShift(paState, 2)));
    }

    int rotateLeft(lua_State *paState) {
      const std::uint32_t bits = toBits(paState, 1);
      return pushBits(paState, std::rotl(bits, static_cast<int>(toShift(paState, 2))));
    }

    int rotateRight(lua_State *paState) {
      const std::uint32_t bits = toBits(paState, 1);
      return pushBits(paState, std::rotr(bits, static_cast<int>(toShift(paState, 2))));
    }

    int swapBytes(lua_State *paState) {
      const std::uint32_t bits = toBits(paState, 1);
      return pushBits(paState, (bits >> 24) | ((bits >> 8) & 0xFF00U) | ((bits & 0xFF00U) << 8) | (bits << 24));
    }

    //! tohex(x [, n]): the low |n| hexadecimal digits of x (default 8, at most 8), upper case for a negative n
    int toHex(lua_State *paState) {
      std::uint32_t bits = toBits(paState, 1);
      auto count = lua_isnoneornil(paState, 2) ? 8 : static_cast<std::int32_t>(toBits(paState, 2));
      const char *digits = "0123456789abcdef";
      if (count < 0) {
        count = -count;
        digits = "0123456789ABCDEF";
      }
      if (count > 8) {
        count = 8;
      }
      char text[8];
      for (std::int32_t index = count - 1; index >= 0; --index) {
        text[index] = digits[bits & 0xFU];
        bits >>= 4;
      }
      lua_pushlstring(paState, text, static_cast<size_t>(count));
      return 1;
    }

    constexpr luaL_Reg cFunctions[] = {
        {"tobit", toBit},    {"bnot", bitNot},      {"band", bitAnd},       {"bor", bitOr},
        {"bxor", bitXor},    {"lshift", shiftLeft}, {"rshift", shiftRight}, {"arshift", shiftRightArithmetic},
        {"rol", rotateLeft}, {"ror", rotateRight},  {"bswap", swapBytes},   {"tohex", toHex},
        {nullptr, nullptr}};
  } // namespace

  int openBitLibrary(lua_State *paState) {
    luaL_register(paState, cBitLibraryName, cFunctions);
    return 1;
  }

} // namespace forte::lua
