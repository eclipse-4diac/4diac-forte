/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luaadapter.h"
#include "resource.h"

extern "C" {
#include <lualib.h>
}

int CLuaAdapter_index(lua_State *luaState) {
  CLuaAdapter* luaAdapter = CLuaEngine::luaGetObject<CLuaAdapter>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  CIEC_ANY* var = luaAdapter->getVariable(id);
  CLuaEngine::luaPushAny(luaState, var);
  return 1;
}

int CLuaAdapter_newindex(lua_State *luaState) {
  CLuaAdapter* luaAdapter = CLuaEngine::luaGetObject<CLuaAdapter>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  CIEC_ANY* var = luaAdapter->getVariable(id);
  CLuaEngine::luaGetAny(luaState, var, 3);
  return 0;
}

int CLuaAdapter_call(lua_State *luaState) {
  CLuaAdapter* luaAdapter = CLuaEngine::luaGetObject<CLuaAdapter>(luaState, 1);
  TForteUInt32 id = static_cast<TForteUInt32>(luaL_checkinteger(luaState, 2));
  luaAdapter->sendOutputEvent(id);
  return 0;
}

const char CLuaAdapter::LUA_NAME[] = "FORTE_CLuaAdapter";
const luaL_Reg CLuaAdapter::LUA_FUNCS[] = { { "__index", CLuaAdapter_index }, { "__newindex", CLuaAdapter_newindex }, { "__call", CLuaAdapter_call }, { NULL, NULL } };
CLuaAdapter::CLuaAdapter(CStringDictionary::TStringId instanceNameId, const CLuaAdapterTypeEntry* typeEntry, bool pa_bIsPlug, TForteByte *connData, TForteByte *varsData, CResource *resource)
    : CAdapter(resource, typeEntry->getSocketInterfaceSpec(), instanceNameId, typeEntry->getPlugInterfaceSpec(), pa_bIsPlug, connData, varsData){
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->registerType<CLuaAdapter>();
  luaEngine->pushObject<CLuaAdapter>(this);
  luaEngine->store(this);
}

CLuaAdapter::~CLuaAdapter() {
}

void CLuaAdapter::executeEvent(int pa_nEIID) {
  CLuaEngine *luaEngine = getResource().getLuaEngine();
  luaEngine->load(typeEntry);
  luaEngine->load(this);
  luaEngine->pushInteger(pa_nEIID);
  if (!luaEngine->call(2, 0)) {
    DEVLOG_ERROR("Error calling function executeEvent for instance %s\n", getInstanceName());
    return;
  }
}

CIEC_ANY* CLuaAdapter::getVariable(TForteUInt32 id) {
  CIEC_ANY* value;
  if ((id & CLuaAdapter::LUA_Adapter_DI_FLAG) != 0) {
    value = getDI(id & CLuaAdapter::LUA_Adapter_VAR_MAX);
  } else if ((id & CLuaAdapter::LUA_Adapter_DO_FLAG) != 0) {
    value = getDO(id & CLuaAdapter::LUA_Adapter_VAR_MAX);
  } else {
    value = NULL;
  }
  return value;
}

