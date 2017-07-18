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

#ifndef SRC_CORE_LUAADAPTER_H_
#define SRC_CORE_LUAADAPTER_H_

#include "adapter.h"
#include "luaengine.h"
#include "forte_any.h"
#include "forte_array.h"

#include <string>
#include "luaadaptertypeentry.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>

  int CLuaFB_index(lua_State *luaState);
  int CLuaFB_newindex(lua_State *luaState);
  int CLuaFB_call(lua_State *luaState);
}

class CLuaAdapter: public CAdapter {
private:
  static const TForteUInt32 LUA_Adapter_DI_FLAG = 16384;
  static const TForteUInt32 LUA_Adapter_DO_FLAG = 32768;
  static const TForteUInt32 LUA_Adapter_VAR_MAX = 4095;

  const CLuaAdapterTypeEntry* typeEntry;

  CIEC_ANY* getVariable(TForteUInt32 id);
public:
  static const char LUA_NAME[];
  static const luaL_Reg LUA_FUNCS[];

  CLuaAdapter(CStringDictionary::TStringId instanceNameId, const CLuaAdapterTypeEntry* typeEntry, bool pa_bIsPlug, TForteByte *connData, TForteByte *varsData, CResource *resource);
  virtual ~CLuaAdapter();

  virtual void executeEvent(int pa_nEIID);

  virtual CStringDictionary::TStringId getFBTypeId(void) const {
    return typeEntry->getTypeNameId();
  }

  friend int CLuaAdapter_index(lua_State *luaState);
  friend int CLuaAdapter_newindex(lua_State *luaState);
  friend int CLuaAdapter_call(lua_State *luaState);
};

#endif /* SRC_CORE_LUAADAPTER_H_ */
