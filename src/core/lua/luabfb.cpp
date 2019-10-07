/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luabfb.h"
#include "resource.h"
#include "luaengine.h"
#include "../adapter.h"

extern "C" {
#include <lualib.h>
}

// write variable to lua engine
int CLuaFB_index(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaPushAny(paLuaState, var);
  return 1;
}

// get variables from lua engine
int CLuaFB_newindex(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaGetAny(paLuaState, var, 3);
  return 0;
}

int CLuaFB_call(lua_State *paLuaState) {
  CLuaBFB* luaFB = CLuaEngine::luaGetObject<CLuaBFB>(paLuaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(paLuaState, 2));
  if((id & CLuaBFB::LUA_FB_AD_FLAG) != 0) {
    luaFB->sendAdapterEvent((id >> 16) & CLuaBFB::LUA_AD_VAR_MAX, id & CLuaBFB::LUA_FB_VAR_MAX);
  } else {
    luaFB->sendOutputEvent(id);
  }
  return 0;
}

const char CLuaBFB::LUA_NAME[] = "FORTE_CLuaFB";
const luaL_Reg CLuaBFB::LUA_FUNCS[] = { { "__index", CLuaFB_index }, { "__newindex", CLuaFB_newindex }, { "__call", CLuaFB_call }, { NULL, NULL } };

CLuaBFB::CLuaBFB(CStringDictionary::TStringId paInstanceNameId, const CLuaBFBTypeEntry* paTypeEntry, TForteByte *paConnData, TForteByte *paVarsData,
    CResource *paResource) :
    CBasicFB(paResource, paTypeEntry->getInterfaceSpec(), paInstanceNameId, paTypeEntry->getInternalVarsInformation(), paConnData, paVarsData),
        mTypeEntry(paTypeEntry) {
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->registerType<CLuaBFB>();
  luaEngine->pushObject<CLuaBFB>(this);
  luaEngine->store(this);
}

CLuaBFB::~CLuaBFB() {
}

void CLuaBFB::executeEvent(int paEIID) {
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->load(mTypeEntry);
  luaEngine->load(this);
  luaEngine->pushInteger(paEIID > 255 ? recalculateID(paEIID) : paEIID);
  if(!luaEngine->call(2, 0)) {
    DEVLOG_ERROR("Error calling function executeEvent for instance %s\n", getInstanceName());
  }
}

CIEC_ANY* CLuaBFB::getVariable(TForteUInt32 paId) {
  if(CLuaBFB::LUA_FB_STATE == paId) {
    return &m_nECCState;
  }
  if((paId & CLuaBFB::LUA_FB_IN_FLAG) != 0) {
    return getVarInternal(paId & CLuaBFB::LUA_FB_VAR_MAX);
  }
  if((paId & CLuaBFB::LUA_FB_AD_FLAG) != 0) {
    if((paId & CLuaBFB::LUA_FB_DO_FLAG) != 0) {
      return m_apoAdapters[(paId >> 16) & CLuaBFB::LUA_AD_VAR_MAX]->getDO(paId & CLuaBFB::LUA_FB_VAR_MAX);
    }
    if((paId & CLuaBFB::LUA_FB_DI_FLAG) != 0) {
      return m_apoAdapters[(paId >> 16) & CLuaBFB::LUA_AD_VAR_MAX]->getDI(paId & CLuaBFB::LUA_FB_VAR_MAX);
    }
    return 0;
  }
  if((paId & CLuaBFB::LUA_FB_DI_FLAG) != 0) {
    return getDI(paId & CLuaBFB::LUA_FB_VAR_MAX);
  }
  if((paId & CLuaBFB::LUA_FB_DO_FLAG) != 0) {
    return getDO(paId & CLuaBFB::LUA_FB_VAR_MAX);
  }
  return 0;
}

