/*******************************************************************************
 * Copyright (c) 2015, 2023 fortiss GmbH, Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - upgraded to new FB memory layout
 *******************************************************************************/

#include "luabfb.h"
#include "resource.h"
#include "luaengine.h"
#include "../adapter.h"
#include "criticalregion.h"

extern "C" {
#include <lualib.h>
}

// write variable to lua engine
int CLuaFB_index(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaPushAny(paLuaState, *var);
  return 1;
}

// get variables from lua engine
int CLuaFB_newindex(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaGetAny(paLuaState, *var, 3);
  return 0;
}

int CLuaFB_call(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  if((id & CLuaBFB::scmLuaFBAdpFlag) != 0) {
    luaFB->sendAdapterEvent((id >> 16) & CLuaBFB::scmLuaAdpVarMax, id & CLuaBFB::scmLuaFBVarMax, luaFB->mInvokingExecEnv);
  } else {
    luaFB->sendOutputEvent(id, luaFB->mInvokingExecEnv);
  }
  return 0;
}

const char CLuaBFB::LUA_NAME[] = "FORTE_CLuaFB";
const luaL_Reg CLuaBFB::LUA_FUNCS[] = { { "__index", CLuaFB_index }, { "__newindex", CLuaFB_newindex }, { "__call", CLuaFB_call }, { nullptr, nullptr } };

CLuaBFB::CLuaBFB(CStringDictionary::TStringId paInstanceNameId, const CLuaBFBTypeEntry* paTypeEntry, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, paTypeEntry->getInterfaceSpec(), paInstanceNameId, paTypeEntry->getInternalVarsInformation()),
        mTypeEntry(paTypeEntry) {
  CLuaEngine *luaEngine = getResource()->getLuaEngine();
  luaEngine->registerType<CLuaBFB>();
  luaEngine->pushObject<CLuaBFB>(this);
  luaEngine->store(this);
}

CLuaBFB::~CLuaBFB() = default;

bool CLuaBFB::initialize() {
  //before calling super we need to configure the interface of the FB
  setupFBInterface(getFBInterfaceSpec());
  return CBasicFB::initialize();
}

void CLuaBFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) {
  mInvokingExecEnv = paECET;
  CLuaEngine *luaEngine = getResource()->getLuaEngine();
  luaEngine->load(mTypeEntry);
  luaEngine->load(this);
  luaEngine->pushInteger(paEIID > 255 ? recalculateID(static_cast<int>(paEIID)) : paEIID);
  if(!luaEngine->call(2, 0)) {
    DEVLOG_ERROR("Error calling function executeEvent for instance %s\n", getInstanceName());
  }
}

CIEC_ANY* CLuaBFB::getVariable(TForteUInt32 paId) {
  if(CLuaBFB::scmLuaFBState == paId) {
    return &mECCState;
  }
  if((paId & CLuaBFB::scmLuaFBInFlag) != 0) {
    return getVarInternal(paId & CLuaBFB::scmLuaFBVarMax);
  }
  if((paId & CLuaBFB::scmLuaFBAdpFlag) != 0) {
    if((paId & CLuaBFB::scmLuaFBDoFlag) != 0) {
      return mAdapters[(paId >> 16) & CLuaBFB::scmLuaAdpVarMax]->getDO(paId & CLuaBFB::scmLuaFBVarMax);
    }
    if((paId & CLuaBFB::scmLuaFBDiFlag) != 0) {
      return mAdapters[(paId >> 16) & CLuaBFB::scmLuaAdpVarMax]->getDI(paId & CLuaBFB::scmLuaFBVarMax);
    }
    return nullptr;
  }
  if((paId & CLuaBFB::scmLuaFBDiFlag) != 0) {
    return getDI(paId & CLuaBFB::scmLuaFBVarMax);
  }
  if((paId & CLuaBFB::scmLuaFBDoFlag) != 0) {
    return getDO(paId & CLuaBFB::scmLuaFBVarMax);
  }
  return nullptr;
}

void CLuaBFB::readInputData(TEventID paEIID) {
  if(nullptr != mInterfaceSpec->mEIWithIndexes && scmNoDataAssociated != mInterfaceSpec->mEIWithIndexes[paEIID]) {
    const TDataIOID *eiWithStart = &(mInterfaceSpec->mEIWith[mInterfaceSpec->mEIWithIndexes[paEIID]]);
    for(size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
      TDataIOID diNum = eiWithStart[i];
      readData(diNum, *getDI(diNum), *getDIConUnchecked(diNum));
    }
  }
}

void CLuaBFB::writeOutputData(TEventID paEO) {
  if (nullptr != mInterfaceSpec->mEOWithIndexes && -1 != mInterfaceSpec->mEOWithIndexes[paEO]) {
    const TDataIOID *eiWithStart = &(mInterfaceSpec->mEOWith[mInterfaceSpec->mEOWithIndexes[paEO]]);
    for (size_t i = 0; eiWithStart[i] != scmWithListDelimiter; ++i) {
      TDataIOID doNum = eiWithStart[i];
      writeData(doNum, *getDO(doNum), *getDOConUnchecked(doNum));
    }
  }
}
