/*******************************************************************************
 * Copyright (c) 2015, 2026 fortiss GmbH, Johannes Kepler University Linz
 *                          Martin Erich Jobst, Aimirim STI
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - upgraded to new FB memory layout
 *   Martin Jobst - add dynamic internal variable setup from CBasicFB
 *   Pedro Ricardo C. Souza - ported to the current core, type entry merged in
 *******************************************************************************/

#include "luabfb.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <algorithm>
#include <cmath>

#include "forte/adapter.h"
#include "forte/resource.h"
#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"
#include "luaengine.h"
#include "luatypehash.h"
#include "../resource_internal.h"

namespace forte::lua {
  namespace {
    constexpr char cHandleMetatable[] = "forte.LuaBFB";
    constexpr TForteUInt32 cIndexMask = 0xFFFF;
    constexpr TForteUInt32 cAdapterIDMask = 0xFF;
    constexpr TForteUInt32 cAdapterEventMask = 0xFF;
    constexpr TForteUInt32 cFlagsMask = 0xFF000000;
  } // namespace

  CLuaBFBTypeEntry::CLuaBFBTypeEntry(StringId paTypeNameId, std::string paSource) :
      CRuntimeFBTypeEntry(paTypeNameId, getInternedTypeHash(paSource)),
      mSource(std::move(paSource)) {
  }

  EMGMResponse CLuaBFBTypeEntry::create(CLuaEngine &paEngine,
                                        StringId paTypeNameId,
                                        std::string paSource,
                                        std::shared_ptr<CLuaBFBTypeEntry> &paEntry,
                                        std::string &paError) {
    if (!paEngine.isValid()) {
      paError = "could not set up a Lua state";
      return EMGMResponse::Overflow;
    }
    if (!paEngine.loadModule(paSource, paTypeNameId.data(), paError)) {
      return EMGMResponse::InvalidObject;
    }
    lua_State *const state = paEngine.getState();
    const EMGMResponse response = create(state, lua_gettop(state), paTypeNameId, std::move(paSource), paEntry, paError);
    lua_pop(state, 1);
    return response;
  }

  EMGMResponse CLuaBFBTypeEntry::create(lua_State *paState,
                                        int paModule,
                                        StringId paTypeNameId,
                                        std::string paSource,
                                        std::shared_ptr<CLuaBFBTypeEntry> &paEntry,
                                        std::string &paError) {
    std::shared_ptr<CLuaBFBTypeEntry> entry(new CLuaBFBTypeEntry(paTypeNameId, std::move(paSource)));

    lua_pushliteral(paState, "ECC");
    lua_rawget(paState, paModule);
    const bool hasECC = lua_isfunction(paState, -1);
    lua_pop(paState, 1);
    if (!hasECC) {
      paError = "the module has no ECC function";
      return EMGMResponse::BadParams;
    }

    lua_pushliteral(paState, "interfaceSpec");
    lua_rawget(paState, paModule);
    EMGMResponse response = entry->mInterface.parse(paState, -1, true, paError);
    lua_pop(paState, 1);

    if (response == EMGMResponse::Ready) {
      lua_pushliteral(paState, "internalVarsInformation");
      lua_rawget(paState, paModule);
      if (!lua_isnil(paState, -1)) {
        response = entry->mInterface.parseInternals(paState, -1, paError);
      }
      lua_pop(paState, 1);
    }
    lua_settop(paState, paModule);

    if (response == EMGMResponse::Ready) {
      paEntry = std::move(entry);
    }
    return response;
  }

  CFunctionBlock *CLuaBFBTypeEntry::createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const {
    CResource *const resource = paContainer.getResource();
    if (resource == nullptr) {
      return nullptr;
    }
    const std::shared_ptr<CLuaEngine> &engine = resource->getInternal().getLuaEngine();
    auto instance = std::make_unique<CLuaBFB>(shared_from_this(), engine, paInstanceNameId, paContainer);
    if (std::string error; !instance->setUp(error)) {
      DEVLOG_ERROR("[CLuaBFB] Could not create %s of type %s: %s\n", paInstanceNameId.data(), getTypeNameId().data(),
                   error.c_str());
      return nullptr;
    }
    return instance.release();
  }

  CLuaBFB::CLuaBFB(std::shared_ptr<const CLuaBFBTypeEntry> paTypeEntry,
                   std::shared_ptr<CLuaEngine> paEngine,
                   StringId paInstanceNameId,
                   CFBContainer &paContainer) :
      CLuaTypeHolder(std::move(paTypeEntry)),
      CBasicFB(paContainer,
               mTypeEntry->getInterface().getSpec(),
               paInstanceNameId,
               mTypeEntry->getInterface().getInternalNames()),
      mEngine(std::move(paEngine)),
      mDIs(CLuaInterface::createValues(mTypeEntry->getInterface().getDITypes())),
      mDOs(CLuaInterface::createValues(mTypeEntry->getInterface().getDOTypes())),
      mDIOs(CLuaInterface::createValues(mTypeEntry->getInterface().getDIOTypes())),
      mInternals(CLuaInterface::createValues(mTypeEntry->getInterface().getInternalTypes())),
      mDIConns(mDIs.size(), nullptr),
      mDIOInConns(mDIOs.size(), nullptr),
      mECCRef(LUA_NOREF),
      mHandleRef(LUA_NOREF) {
    const SFBInterfaceSpec &spec = getFBInterfaceSpec();
    mEOConns.reserve(spec.getNumEOs());
    for (TPortId i = 0; i < spec.getNumEOs(); ++i) {
      mEOConns.emplace_back(*this, i);
    }
    mDOConns.reserve(mDOs.size());
    for (TPortId i = 0; i < mDOs.size(); ++i) {
      mDOConns.emplace_back(*this, i, *mDOs[i]);
    }
    mDIOOutConns.reserve(mDIOs.size());
    for (TPortId i = 0; i < mDIOs.size(); ++i) {
      mDIOOutConns.push_back(std::make_unique<CInOutDataConnection>(*this, i, mDIOs[i].get()));
    }
    // the list ID of an adapter is its Lua adapter ID, so incoming adapter events map directly to the Lua IDs
    const auto adapters = mTypeEntry->getInterface().getAdapters();
    for (std::size_t id = 0; id < adapters.size(); ++id) {
      const SLuaAdapterDefinition &adapter = adapters[id];
      const auto listID = static_cast<TForteUInt8>(id);
      if (adapter.mIsPlug) {
        mPlugs.push_back(std::make_unique<CTypedPlugPin>(adapter.mName, *this, adapter.mTypeName, listID));
      } else {
        mSockets.push_back(std::make_unique<CTypedSocketPin>(adapter.mName, *this, adapter.mTypeName, listID));
      }
    }
  }

  CLuaBFB::~CLuaBFB() {
    util::CCriticalRegion region(mEngine->getLock());
    lua_State *const state = mEngine->getState();
    if (mHandleRef != LUA_NOREF) {
      // Lua may still hold the handle, it must not reach this instance anymore
      mEngine->pushRef(mHandleRef);
      *static_cast<CLuaBFB **>(lua_touserdata(state, -1)) = nullptr;
      lua_pop(state, 1);
      mEngine->unref(mHandleRef);
    }
    if (mECCRef != LUA_NOREF) {
      mEngine->unref(mECCRef);
    }
  }

  bool CLuaBFB::setUp(std::string &paError) {
    for (std::size_t id = 0; id < mTypeEntry->getInterface().getAdapters().size(); ++id) {
      if (getAdapterBlock(id) == nullptr) {
        paError = "could not create adapter ";
        paError += mTypeEntry->getInterface().getAdapters()[id].mName.data();
        return false;
      }
    }
    if (std::find(mDIs.begin(), mDIs.end(), nullptr) != mDIs.end() ||
        std::find(mDOs.begin(), mDOs.end(), nullptr) != mDOs.end() ||
        std::find(mInternals.begin(), mInternals.end(), nullptr) != mInternals.end()) {
      paError = "could not create the variables";
      return false;
    }

    util::CCriticalRegion region(mEngine->getLock());
    if (!mEngine->pushTypeModule(mTypeEntry, mTypeEntry->getDefinition(), getFBTypeId().data(), paError)) {
      return false;
    }
    lua_State *const state = mEngine->getState();
    lua_pushliteral(state, "ECC");
    lua_rawget(state, -2);
    lua_remove(state, -2);
    if (!lua_isfunction(state, -1)) {
      lua_pop(state, 1);
      paError = "the module has no ECC function";
      return false;
    }
    // the handle allocates, an allocation error must not reach the panic handler
    lua_pushcfunction(state, createHandle);
    lua_pushlightuserdata(state, this);
    if (!mEngine->call(1, 1, paError)) {
      lua_pop(state, 1);
      return false;
    }
    mHandleRef = mEngine->ref();
    mECCRef = mEngine->ref();
    return true;
  }

  CIEC_ANY *CLuaBFB::getDI(TPortId paDINum) {
    return (paDINum < mDIs.size()) ? mDIs[paDINum].get() : nullptr;
  }

  CIEC_ANY *CLuaBFB::getDO(TPortId paDONum) {
    return (paDONum < mDOs.size()) ? mDOs[paDONum].get() : nullptr;
  }

  CIEC_ANY *CLuaBFB::getDIO(TPortId paDIONum) {
    return (paDIONum < mDIOs.size()) ? mDIOs[paDIONum].get() : nullptr;
  }

  void CLuaBFB::setInitialValues() {
    CBasicFB::setInitialValues();
    const CLuaInterface &interface = mTypeEntry->getInterface();
    CLuaInterface::setInitialValues(interface.getDITypes(), mDIs);
    CLuaInterface::setInitialValues(interface.getDOTypes(), mDOs);
    CLuaInterface::setInitialValues(interface.getDIOTypes(), mDIOs);
    CLuaInterface::setInitialValues(interface.getInternalTypes(), mInternals);
  }

  CIEC_ANY *CLuaBFB::getVarInternal(size_t paVarIntNum) {
    return mInternals[paVarIntNum].get();
  }

  CEventConnection *CLuaBFB::getEOConUnchecked(TPortId paEONum) {
    return &mEOConns[paEONum];
  }

  CDataConnection *CLuaBFB::getDOConUnchecked(TPortId paDONum) {
    return &mDOConns[paDONum];
  }

  CDataConnection **CLuaBFB::getDIConUnchecked(TPortId paDINum) {
    return &mDIConns[paDINum];
  }

  CInOutDataConnection **CLuaBFB::getDIOInConUnchecked(TPortId paDIONum) {
    return &mDIOInConns[paDIONum];
  }

  CInOutDataConnection *CLuaBFB::getDIOOutConUnchecked(TPortId paDIONum) {
    return mDIOOutConns[paDIONum].get();
  }

  IPlugPin *CLuaBFB::getPlugPinUnchecked(TPortId paPlugNum) {
    return mPlugs[paPlugNum].get();
  }

  ISocketPin *CLuaBFB::getSocketPinUnchecked(TPortId paSocketNum) {
    return mSockets[paSocketNum].get();
  }

  CAdapter *CLuaBFB::getAdapterBlock(std::size_t paAdapterID) {
    if (paAdapterID < mPlugs.size()) {
      return mPlugs[paAdapterID]->getAdapterBlock();
    }
    paAdapterID -= mPlugs.size();
    return (paAdapterID < mSockets.size()) ? mSockets[paAdapterID]->getAdapterBlock() : nullptr;
  }

  void CLuaBFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) {
    auto id = static_cast<TForteUInt32>(paEIID);
    if (id > cAdapterEventMask) {
      // an adapter event: (list ID + 1) << 8 | event
      id = scmAdapterFlag | ((((id >> 8) - 1) & cAdapterIDMask) << 16) | (id & cAdapterEventMask);
    }

    std::string error;
    bool success;
    {
      util::CCriticalRegion region(mEngine->getLock());
      lua_State *const state = mEngine->getState();
      mInvokingECET = paECET;
      mEngine->pushRef(mECCRef);
      mEngine->pushRef(mHandleRef);
      lua_pushnumber(state, static_cast<lua_Number>(id));
      success = mEngine->call(2, 0, error);
      mInvokingECET = nullptr;
    }
    if (!success) {
      DEVLOG_ERROR("[CLuaBFB] %s: %s\n", getFullQualifiedApplicationInstanceName('.').c_str(), error.c_str());
    }
  }

  void CLuaBFB::readInputData(TEventID paEIID) {
    const std::size_t numDIs = mDIs.size();
    for (const TPortId di : mTypeEntry->getInterface().getEIWith(paEIID)) {
      if (di < numDIs) {
        readData(di, *mDIs[di], mDIConns[di]);
      } else {
        // an in-out variable takes the value its input connection refers to
        const std::size_t dio = di - numDIs;
        readData(numDIs + mDOs.size() + dio, *mDIOs[dio], mDIOOutConns[dio].get());
      }
    }
  }

  void CLuaBFB::writeOutputData(TEventID paEO) {
    const std::size_t numDIs = mDIs.size();
    const std::size_t numDOs = mDOs.size();
    for (const TPortId dataOutput : mTypeEntry->getInterface().getEOWith(paEO)) {
      if (dataOutput < numDOs) {
        writeData(numDIs + dataOutput, *mDOs[dataOutput], mDOConns[dataOutput]);
      } else {
        // an in-out variable writes back to the value its input connection refers to
        const std::size_t dio = dataOutput - numDOs;
        writeData(numDIs + numDOs + dio, *mDIOs[dio], *mDIOOutConns[dio]);
      }
    }
  }

  CIEC_ANY *CLuaBFB::resolve(TForteUInt32 paKey) {
    const std::size_t index = paKey & cIndexMask;
    const std::size_t adapterID = (paKey >> 16) & cAdapterIDMask;
    switch (paKey & cFlagsMask) {
      case scmDIFlag: return (adapterID == 0) ? getDI(index) : nullptr;
      case scmDOFlag: return (adapterID == 0) ? getDO(index) : nullptr;
      case scmInternalFlag: return (adapterID == 0 && index < mInternals.size()) ? mInternals[index].get() : nullptr;
      case scmDIOFlag: return (adapterID == 0) ? getDIO(index) : nullptr;
      case scmAdapterFlag | scmDIFlag:
        if (CAdapter *const adapter = getAdapterBlock(adapterID); adapter != nullptr) {
          return adapter->getDI(index);
        }
        return nullptr;
      case scmAdapterFlag | scmDOFlag:
        if (CAdapter *const adapter = getAdapterBlock(adapterID); adapter != nullptr) {
          return adapter->getDO(index);
        }
        return nullptr;
      default: return nullptr;
    }
  }

  // The functions below are called from Lua and raise Lua errors: no objects with destructors in them.

  CLuaBFB *CLuaBFB::checkInstance(lua_State *paState) {
    CLuaBFB *const instance = *static_cast<CLuaBFB **>(luaL_checkudata(paState, 1, cHandleMetatable));
    if (instance == nullptr) {
      luaL_error(paState, "the function block was deleted");
    }
    return instance;
  }

  TForteUInt32 CLuaBFB::checkKey(lua_State *paState, int paIndex) {
    const lua_Number key = luaL_checknumber(paState, paIndex);
    if (!(key >= 0 && key <= 4294967295.0) || key != std::floor(key)) {
      luaL_error(paState, "invalid access constant %f", key);
    }
    return static_cast<TForteUInt32>(key);
  }

  int CLuaBFB::index(lua_State *paState) {
    CLuaBFB *const instance = checkInstance(paState);
    const TForteUInt32 key = checkKey(paState, 2);
    if (key == scmStateKey) {
      lua_pushnumber(paState, static_cast<TForteUInt16>(instance->mECCState));
      return 1;
    }
    const CIEC_ANY *const variable = instance->resolve(key);
    if (variable == nullptr) {
      return luaL_error(paState, "no variable for access constant %f", static_cast<lua_Number>(key));
    }
    CLuaEngine::pushValue(paState, *variable);
    return 1;
  }

  int CLuaBFB::newIndex(lua_State *paState) {
    CLuaBFB *const instance = checkInstance(paState);
    const TForteUInt32 key = checkKey(paState, 2);
    if (key == scmStateKey) {
      const lua_Number state = luaL_checknumber(paState, 3);
      if (!(state >= 0 && state <= 65535) || state != std::floor(state)) {
        return luaL_error(paState, "invalid ECC state %f", state);
      }
      instance->mECCState = CIEC_STATE(static_cast<TForteUInt16>(state));
      return 0;
    }
    CIEC_ANY *const variable = instance->resolve(key);
    if (variable == nullptr) {
      return luaL_error(paState, "no variable for access constant %f", static_cast<lua_Number>(key));
    }
    if (const char *const error = CLuaEngine::getValue(paState, 3, *variable); error != nullptr) {
      return luaL_error(paState, "access constant %f: %s", static_cast<lua_Number>(key), error);
    }
    return 0;
  }

  int CLuaBFB::call(lua_State *paState) {
    CLuaBFB *const instance = checkInstance(paState);
    const TForteUInt32 key = checkKey(paState, 2);
    if (instance->mInvokingECET == nullptr) {
      return luaL_error(paState, "events can only be sent from the ECC");
    }
    if ((key & cFlagsMask) == scmAdapterFlag) {
      CAdapter *const adapter = instance->getAdapterBlock((key >> 16) & cAdapterIDMask);
      const TEventID event = key & cIndexMask;
      if (adapter == nullptr || event >= adapter->getFBInterfaceSpec().getNumEIs()) {
        return luaL_error(paState, "no adapter event for access constant %f", static_cast<lua_Number>(key));
      }
      instance->sendAdapterEvent(*adapter, event, instance->mInvokingECET);
      return 0;
    }
    if (key >= instance->getFBInterfaceSpec().getNumEOs()) {
      return luaL_error(paState, "no output event for access constant %f", static_cast<lua_Number>(key));
    }
    instance->sendOutputEvent(key, instance->mInvokingECET);
    return 0;
  }

  int CLuaBFB::createHandle(lua_State *paState) {
    CLuaBFB *const instance = static_cast<CLuaBFB *>(lua_touserdata(paState, 1));
    *static_cast<CLuaBFB **>(lua_newuserdata(paState, sizeof(CLuaBFB *))) = instance;
    pushMetatable(paState);
    lua_setmetatable(paState, -2);
    return 1;
  }

  void CLuaBFB::pushMetatable(lua_State *paState) {
    if (luaL_newmetatable(paState, cHandleMetatable) != 0) {
      lua_pushcfunction(paState, index);
      lua_setfield(paState, -2, "__index");
      lua_pushcfunction(paState, newIndex);
      lua_setfield(paState, -2, "__newindex");
      lua_pushcfunction(paState, call);
      lua_setfield(paState, -2, "__call");
      lua_pushboolean(paState, 0);
      lua_setfield(paState, -2, "__metatable");
    }
  }

} // namespace forte::lua
