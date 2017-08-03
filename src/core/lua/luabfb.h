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

#ifndef SRC_CORE_LUABFB_H_
#define SRC_CORE_LUABFB_H_

#include "basicfb.h"
#include "luaengine.h"
#include "forte_any.h"
#include "forte_array.h"
#include "luabfbtypeentry.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>

  int CLuaFB_index(lua_State *luaState);
  int CLuaFB_newindex(lua_State *luaState);
  int CLuaFB_call(lua_State *luaState);
}

class CLuaBFB: public CBasicFB {
private:
  static const TForteUInt32 LUA_FB_VAR_MAX = 65535;
  static const TForteUInt32 LUA_AD_VAR_MAX = 255;

  static const TForteUInt32 LUA_FB_STATE = 0;
  static const TForteUInt32 LUA_FB_DI_FLAG = 1 << 25;
  static const TForteUInt32 LUA_FB_DO_FLAG = 1 << 26;
  static const TForteUInt32 LUA_FB_AD_FLAG = 1 << 27;
  static const TForteUInt32 LUA_FB_IN_FLAG = 1 << 28;

  const CLuaBFBTypeEntry* typeEntry;

  CIEC_ANY* getVariable(TForteUInt32 id);

  int recalculateID(int pa_nEIID) {
    return CLuaBFB::LUA_FB_AD_FLAG | ((pa_nEIID >> 8) - 1) | (pa_nEIID & 0x00ff);
  }

public:
  static const char LUA_NAME[];
  static const luaL_Reg LUA_FUNCS[];

  CLuaBFB(CStringDictionary::TStringId instanceNameId, const CLuaBFBTypeEntry* typeEntry, TForteByte *connData, TForteByte *varsData, CResource *resource);
  virtual ~CLuaBFB();

  virtual void executeEvent(int pa_nEIID);

  virtual CStringDictionary::TStringId getFBTypeId(void) const {
    return typeEntry->getTypeNameId();
  }

  friend int CLuaFB_index(lua_State *luaState);
  friend int CLuaFB_newindex(lua_State *luaState);
  friend int CLuaFB_call(lua_State *luaState);
};

#endif /* SRC_CORE_LUABFB_H_ */
