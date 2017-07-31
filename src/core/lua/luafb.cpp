/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Jobst
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luafb.h"
#include "resource.h"

extern "C" {
#include <lualib.h>
}

// write variable to lua engine
int CLuaFB_index(lua_State *luaState) {
  CLuaFB* luaFB = CLuaEngine::luaGetObject<CLuaFB>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaPushAny(luaState, var);
  return 1;
}

// get variables from lua engine
int CLuaFB_newindex(lua_State *luaState) {
  CLuaFB* luaFB = CLuaEngine::luaGetObject<CLuaFB>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  CIEC_ANY* var = luaFB->getVariable(id);
  CLuaEngine::luaGetAny(luaState, var, 3);
  return 0;
}

int CLuaFB_call(lua_State *luaState) {
  CLuaFB* luaFB = CLuaEngine::luaGetObject<CLuaFB>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  if((id & CLuaFB::LUA_FB_AD_FLAG) != 0){
    luaFB->sendAdapterEvent((id >> 16) & CLuaFB::LUA_AD_VAR_MAX, id & CLuaFB::LUA_FB_VAR_MAX);
  }else{
    luaFB->sendOutputEvent(id);
  }
  return 0;
}

const char CLuaFB::LUA_NAME[] = "FORTE_CLuaFB";
const luaL_Reg CLuaFB::LUA_FUNCS[] = { { "__index", CLuaFB_index }, { "__newindex", CLuaFB_newindex }, { "__call", CLuaFB_call }, { NULL, NULL } };

CLuaFB::CLuaFB(CStringDictionary::TStringId instanceNameId, const CLuaFBTypeEntry* typeEntry, TForteByte *connData, TForteByte *varsData, CResource *resource)
    : CBasicFB(resource, typeEntry->getInterfaceSpec(), instanceNameId, typeEntry->getInternalVarsInformation(), connData, varsData), typeEntry(typeEntry) {
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->registerType<CLuaFB>();
  luaEngine->pushObject<CLuaFB>(this);
  luaEngine->store(this);
}

CLuaFB::~CLuaFB() {
}

void CLuaFB::executeEvent(int pa_nEIID) {
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->load(typeEntry);
  luaEngine->load(this);
  luaEngine->pushInteger(pa_nEIID > 255 ? recalculateID(pa_nEIID) : pa_nEIID);
  if (!luaEngine->call(2, 0)) {
    DEVLOG_ERROR("Error calling function executeEvent for instance %s\n", getInstanceName());
    return;
  }
}

CIEC_ANY* CLuaFB::getVariable(TForteUInt32 id) {
  if (CLuaFB::LUA_FB_STATE == id) {
    return &m_nECCState;
  }
  if((id & CLuaFB::LUA_FB_IN_FLAG) != 0) {
    return getVarInternal(id & CLuaFB::LUA_FB_VAR_MAX);
  }
  if ((id & CLuaFB::LUA_FB_AD_FLAG) != 0) {
    if ((id & CLuaFB::LUA_FB_DI_FLAG) != 0) {
      if(m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->isPlug()){
        return m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->getDO(id & CLuaFB::LUA_FB_VAR_MAX);
      }else{
        return m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->getDO(id & CLuaFB::LUA_FB_VAR_MAX);
      }
    }
    if ((id & CLuaFB::LUA_FB_DO_FLAG) != 0) {
      if(m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->isPlug()){
        return m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->getDI(id & CLuaFB::LUA_FB_VAR_MAX);
      }else{
        return m_apoAdapters[(id >> 16) & CLuaFB::LUA_AD_VAR_MAX]->getDI(id & CLuaFB::LUA_FB_VAR_MAX);
      }
    }
    return 0;
  }
  if ((id & CLuaFB::LUA_FB_DI_FLAG) != 0) {
    return getDI(id & CLuaFB::LUA_FB_VAR_MAX);
  }
  if ((id & CLuaFB::LUA_FB_DO_FLAG) != 0) {
    return getDO(id & CLuaFB::LUA_FB_VAR_MAX);
  }
  return 0;
}

