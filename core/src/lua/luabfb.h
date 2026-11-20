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

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "forte/basicfb.h"
#include "forte/dataconn.h"
#include "forte/eventconn.h"
#include "forte/inoutdataconn.h"
#include "luainterface.h"
#include "../anyadapter.h"
#include "../typelib_internal.h"

struct lua_State;

namespace forte::lua {

  class CLuaEngine;

  /*!\brief A basic FB type defined at run time by a Lua module
   *
   * The module returns {ECC = function(fb, id), interfaceSpec = …, internalVarsInformation = …}.
   */
  class CLuaBFBTypeEntry final : public CRuntimeFBTypeEntry, public std::enable_shared_from_this<CLuaBFBTypeEntry> {
    public:
      /*!\brief Reads the module in paEngine and creates the type entry
       *
       * @return Ready, InvalidObject if the module does not load, BadParams for a malformed module,
       *         UnsupportedType for an unknown data or adapter type
       */
      static EMGMResponse create(CLuaEngine &paEngine,
                                 StringId paTypeNameId,
                                 std::string paSource,
                                 std::shared_ptr<CLuaBFBTypeEntry> &paEntry,
                                 std::string &paError);

      //! Creates the type entry from the loaded module table at paModule of paState, which stays there
      static EMGMResponse create(lua_State *paState,
                                 int paModule,
                                 StringId paTypeNameId,
                                 std::string paSource,
                                 std::shared_ptr<CLuaBFBTypeEntry> &paEntry,
                                 std::string &paError);

      std::string_view getDefinition() const override {
        return mSource;
      }

      CFunctionBlock *createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const override;

      [[nodiscard]] const CLuaInterface &getInterface() const {
        return mInterface;
      }

    private:
      CLuaBFBTypeEntry(StringId paTypeNameId, std::string paSource);

      const std::string mSource;
      CLuaInterface mInterface;
  };

  /*!\brief An instance of a Lua basic FB type
   *
   * Every input event calls the module's ECC with a handle of the instance and the event ID. The handle reads and
   * writes variables by access constant (fb[k]) and sends events (fb(k)). Adapter events arrive with the ID
   * FB_AD_FLAG | (adapter ID << 16) | event, the adapter ID being the adapter's list ID here.
   */
  class CLuaBFB final : private CLuaTypeHolder<CLuaBFBTypeEntry>, public CBasicFB {
    public:
      CLuaBFB(std::shared_ptr<const CLuaBFBTypeEntry> paTypeEntry,
              std::shared_ptr<CLuaEngine> paEngine,
              StringId paInstanceNameId,
              CFBContainer &paContainer);
      ~CLuaBFB() override;

      //! Binds the instance to the module in the resource's Lua state, false if that or an adapter failed
      bool setUp(std::string &paError);

      StringId getFBTypeId() const override {
        return mTypeEntry->getTypeNameId();
      }

      CIEC_ANY *getDI(TPortId paDINum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;
      CIEC_ANY *getDIO(TPortId paDIONum) override;

      static constexpr TForteUInt32 scmStateKey = 0;
      static constexpr TForteUInt32 scmDIFlag = 1U << 25;
      static constexpr TForteUInt32 scmDOFlag = 1U << 26;
      static constexpr TForteUInt32 scmAdapterFlag = 1U << 27;
      static constexpr TForteUInt32 scmInternalFlag = 1U << 28;
      //! In-out variables are both inputs and outputs
      static constexpr TForteUInt32 scmDIOFlag = scmDIFlag | scmDOFlag;

    protected:
      void setInitialValues() override;

      CIEC_ANY *getVarInternal(size_t paVarIntNum) override;

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CInOutDataConnection **getDIOInConUnchecked(TPortId paDIONum) override;
      CInOutDataConnection *getDIOOutConUnchecked(TPortId paDIONum) override;
      IPlugPin *getPlugPinUnchecked(TPortId paPlugNum) override;
      ISocketPin *getSocketPinUnchecked(TPortId paSocketNum) override;

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) override;
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEO) override;

      //! The variable an access constant names, nullptr if there is none
      CIEC_ANY *resolve(TForteUInt32 paKey);
      CAdapter *getAdapterBlock(std::size_t paAdapterID);

      static CLuaBFB *checkInstance(lua_State *paState);
      static TForteUInt32 checkKey(lua_State *paState, int paIndex);
      static int index(lua_State *paState);
      static int newIndex(lua_State *paState);
      static int call(lua_State *paState);
      //! Creates the userdata the module's ECC gets as fb, the instance is the light userdata argument
      static int createHandle(lua_State *paState);
      static void pushMetatable(lua_State *paState);

      const std::shared_ptr<CLuaEngine> mEngine;
      std::vector<std::unique_ptr<CIEC_ANY>> mDIs;
      std::vector<std::unique_ptr<CIEC_ANY>> mDOs;
      std::vector<std::unique_ptr<CIEC_ANY>> mDIOs;
      std::vector<std::unique_ptr<CIEC_ANY>> mInternals;
      std::vector<CDataConnection *> mDIConns;
      //! The connection each in-out variable gets its value from, nullptr while it has none
      std::vector<CInOutDataConnection *> mDIOInConns;
      //! The in-out connections onwards, each refers to the value of its input connection or else to the variable
      std::vector<std::unique_ptr<CInOutDataConnection>> mDIOOutConns;
      std::vector<CEventConnection> mEOConns;
      std::vector<CGenDataConnection> mDOConns;
      std::vector<std::unique_ptr<CTypedPlugPin>> mPlugs;
      std::vector<std::unique_ptr<CTypedSocketPin>> mSockets;
      int mECCRef;
      int mHandleRef;
      CEventChainExecutionThread *mInvokingECET = nullptr;
  };

} // namespace forte::lua
