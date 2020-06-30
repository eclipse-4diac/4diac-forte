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

#ifndef SRC_CORE_LUABFB_H_
#define SRC_CORE_LUABFB_H_

#include "basicfb.h"
#include "luabfbtypeentry.h"

class CIEC_ANY;

extern "C" {
#include <lua.h>
#include <lauxlib.h>

  int CLuaFB_index(lua_State *luaState);
  int CLuaFB_newindex(lua_State *luaState);
  int CLuaFB_call(lua_State *luaState);
}

class CLuaBFB : public CBasicFB {
  private:
    static const TForteUInt32 LUA_FB_VAR_MAX = 65535;
    static const TForteUInt32 LUA_AD_VAR_MAX = 255;

    static const TForteUInt32 LUA_FB_STATE = 0;
    static const TForteUInt32 LUA_FB_DI_FLAG = 1 << 25;
    static const TForteUInt32 LUA_FB_DO_FLAG = 1 << 26;
    static const TForteUInt32 LUA_FB_AD_FLAG = 1 << 27;
    static const TForteUInt32 LUA_FB_IN_FLAG = 1 << 28;

    const CLuaBFBTypeEntry* mTypeEntry;

    CIEC_ANY* getVariable(TForteUInt32 paId);

    int recalculateID(int pa_nEIID) {
      return CLuaBFB::LUA_FB_AD_FLAG | ((((pa_nEIID >> 8) - 1) << 16) & 0xFF0000) | (pa_nEIID & 0x00FF);
    }

  public:
    static const char LUA_NAME[];
    static const luaL_Reg LUA_FUNCS[];

    CLuaBFB(CStringDictionary::TStringId paInstanceNameId, const CLuaBFBTypeEntry* paTypeEntry, TForteByte *paConnData, TForteByte *paVarsData,
        CResource *paResource);
    virtual ~CLuaBFB();

    virtual void executeEvent(int paEIID);

    virtual CStringDictionary::TStringId getFBTypeId(void) const {
      return mTypeEntry->getTypeNameId();
    }

    friend int CLuaFB_index(lua_State *paLuaState);
    friend int CLuaFB_newindex(lua_State *paLuaState);
    friend int CLuaFB_call(lua_State *paLuaState);
};

#endif /* SRC_CORE_LUABFB_H_ */
