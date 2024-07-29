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

#ifndef SRC_CORE_LUABFB_H_
#define SRC_CORE_LUABFB_H_

#include "genfb.h"
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

class CLuaBFB : public CGenFunctionBlock<CBasicFB> {
  private:
    static constexpr TForteUInt32 scmLuaFBVarMax = 65535;
    static constexpr TForteUInt32 scmLuaAdpVarMax = 255;

    static constexpr TForteUInt32 scmLuaFBState = 0;
    static constexpr TForteUInt32 scmLuaFBDiFlag = 1 << 25;
    static constexpr TForteUInt32 scmLuaFBDoFlag = 1 << 26;
    static constexpr TForteUInt32 scmLuaFBAdpFlag = 1 << 27;
    static constexpr TForteUInt32 scmLuaFBInFlag = 1 << 28;

    const CLuaBFBTypeEntry* mTypeEntry;
    CEventChainExecutionThread *mInvokingExecEnv = nullptr;

    CIEC_ANY* getVariable(TForteUInt32 paId);

    int recalculateID(int paEIID) {
      return CLuaBFB::scmLuaFBAdpFlag | ((((paEIID >> 8) - 1) << 16) & 0xFF0000) | (paEIID & 0x00FF);
    }

  public:
    static const char LUA_NAME[];
    static const luaL_Reg LUA_FUNCS[];

    CLuaBFB(CStringDictionary::TStringId paInstanceNameId, const CLuaBFBTypeEntry* paTypeEntry, forte::core::CFBContainer &paContainer);
    ~CLuaBFB() override;

    bool initialize() override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override {
      paInterfaceSpec = *mInterfaceSpec;
      return true;
    }

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) override;

    CStringDictionary::TStringId getFBTypeId() const override {
      return mTypeEntry->getTypeNameId();
    }

    friend int CLuaFB_index(lua_State *paLuaState);
    friend int CLuaFB_newindex(lua_State *paLuaState);
    friend int CLuaFB_call(lua_State *paLuaState);

  private:
    virtual void readInputData(TEventID paEIID);
    virtual void writeOutputData(TEventID paEO);
};

#endif /* SRC_CORE_LUABFB_H_ */
