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

#include "luatypes.h"

extern "C" {
#include <lua.h>
}

#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"
#include "luaadapter.h"
#include "luabfb.h"
#include "luacfb.h"
#include "luaengine.h"

namespace forte::lua {
  namespace {
    enum class EFBKind { Basic, Composite, Unknown };

    /*!\brief Holds paEngine for one type definition
     *
     * The engine may be shared with running instances (FORTE_LUA_SHARED_STATE), so the definition takes the lock
     * and its own instruction budget, and gives the instances theirs back afterwards.
     */
    class CDefinitionScope {
      public:
        explicit CDefinitionScope(CLuaEngine &paEngine) :
            mRegion(paEngine.getLock()),
            mEngine(paEngine),
            mInstanceLimit(paEngine.getInstructionLimit()) {
          paEngine.setInstructionLimit(CLuaEngine::scmDefinitionInstructionLimit);
        }

        ~CDefinitionScope() {
          mEngine.setInstructionLimit(mInstanceLimit);
        }

        CDefinitionScope(const CDefinitionScope &) = delete;
        CDefinitionScope &operator=(const CDefinitionScope &) = delete;

      private:
        util::CCriticalRegion mRegion;
        CLuaEngine &mEngine;
        const unsigned int mInstanceLimit;
    };

    //! The kind of FB type the module table at paModule defines
    EFBKind getFBKind(lua_State *paState, int paModule) {
      lua_pushliteral(paState, "ECC");
      lua_rawget(paState, paModule);
      const bool hasECC = !lua_isnil(paState, -1);
      lua_pushliteral(paState, "fbnSpec");
      lua_rawget(paState, paModule);
      const bool hasNetwork = !lua_isnil(paState, -1);
      lua_pop(paState, 2);
      return hasECC ? EFBKind::Basic : (hasNetwork ? EFBKind::Composite : EFBKind::Unknown);
    }

    //! paCreate is TEntry::create without its entry and error parameters
    template<typename TEntry, typename TCreate, typename TRegister>
    EMGMResponse createAndRegister(TCreate paCreate, TRegister paRegister, std::string &paError) {
      std::shared_ptr<TEntry> entry;
      EMGMResponse response = paCreate(entry, paError);
      if (response == EMGMResponse::Ready) {
        response = paRegister(std::move(entry));
        if (response == EMGMResponse::InvalidOperation) {
          paError = "a compiled type has this name";
        } else if (response == EMGMResponse::InvalidState) {
          paError = "the type exists with another definition and has instances";
        }
      }
      return response;
    }

    //! Logs the result; the module's tables are garbage now and go before the next type or instance needs memory
    EMGMResponse
    report(CLuaEngine &paEngine, StringId paTypeNameId, EMGMResponse paResponse, const std::string &paError) {
      paEngine.collectGarbage();
      if (paResponse == EMGMResponse::Ready) {
        DEVLOG_INFO("[Lua] Created type %s\n", paTypeNameId.data());
      } else {
        DEVLOG_ERROR("[Lua] Could not create type %s: %s\n", paTypeNameId.data(), paError.c_str());
      }
      return paResponse;
    }
  } // namespace

  EMGMResponse createFBType(CLuaEngine &paEngine, StringId paTypeNameId, std::string paSource) {
    const CDefinitionScope scope(paEngine);
    std::string error;
    if (!isValidTypeName(paTypeNameId.data())) {
      return report(paEngine, paTypeNameId, EMGMResponse::BadParams, "invalid type name");
    }
    if (!paEngine.isValid()) {
      return report(paEngine, paTypeNameId, EMGMResponse::Overflow, "could not set up a Lua state");
    }
    // loaded once for both the kind and the entry, a module's code is the largest part of a definition
    if (!paEngine.loadModule(paSource, paTypeNameId.data(), error)) {
      return report(paEngine, paTypeNameId, EMGMResponse::InvalidObject, error);
    }
    lua_State *const state = paEngine.getState();
    const int module = lua_gettop(state);
    const auto registerFB = [](std::shared_ptr<const CRuntimeFBTypeEntry> paEntry) {
      return registerRuntimeFBType(std::move(paEntry));
    };
    EMGMResponse response = EMGMResponse::BadParams;
    switch (getFBKind(state, module)) {
      case EFBKind::Basic:
        response = createAndRegister<CLuaBFBTypeEntry>(
            [&](std::shared_ptr<CLuaBFBTypeEntry> &paEntry, std::string &paError) {
              return CLuaBFBTypeEntry::create(state, module, paTypeNameId, std::move(paSource), paEntry, paError);
            },
            registerFB, error);
        break;
      case EFBKind::Composite:
        response = createAndRegister<CLuaCFBTypeEntry>(
            [&](std::shared_ptr<CLuaCFBTypeEntry> &paEntry, std::string &paError) {
              return CLuaCFBTypeEntry::create(state, module, paTypeNameId, std::move(paSource), paEntry, paError);
            },
            registerFB, error);
        break;
      case EFBKind::Unknown: error = "the module has neither an ECC function nor an fbnSpec table"; break;
    }
    lua_settop(state, module - 1);
    return report(paEngine, paTypeNameId, response, error);
  }

  EMGMResponse createAdapterType(CLuaEngine &paEngine, StringId paTypeNameId, std::string paSource) {
    const CDefinitionScope scope(paEngine);
    std::string error;
    if (!isValidTypeName(paTypeNameId.data())) {
      return report(paEngine, paTypeNameId, EMGMResponse::BadParams, "invalid type name");
    }
    const auto registerAdapter = [](std::shared_ptr<const CRuntimeAdapterTypeEntry> paEntry) {
      return registerRuntimeAdapterType(std::move(paEntry));
    };
    const EMGMResponse response = createAndRegister<CLuaAdapterTypeEntry>(
        [&](std::shared_ptr<CLuaAdapterTypeEntry> &paEntry, std::string &paError) {
          return CLuaAdapterTypeEntry::create(paEngine, paTypeNameId, std::move(paSource), paEntry, paError);
        },
        registerAdapter, error);
    return report(paEngine, paTypeNameId, response, error);
  }

} // namespace forte::lua
