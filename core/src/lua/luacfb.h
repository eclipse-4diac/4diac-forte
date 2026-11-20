/*******************************************************************************
 * Copyright (c) 2017, 2026 fortiss GmbH, Johannes Kepler University Linz
 *                          Martin Erich Jobst, Aimirim STI
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - upgraded to new FB memory layout
 *   Martin Jobst  - added dynamic internal FB creation from CCompositeFB
 *   Pedro Ricardo C. Souza - ported to the current core, type entry merged in
 *******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "forte/cfb.h"
#include "forte/dataconn.h"
#include "forte/eventconn.h"
#include "forte/inoutdataconn.h"
#include "luainterface.h"
#include "../anyadapter.h"
#include "../typelib_internal.h"

namespace forte::lua {

  class CLuaEngine;

  /*!\brief A composite FB type defined at run time by a Lua module
   *
   * The module returns {interfaceSpec = …, fbnSpec = …} and holds no code, so its instances need no Lua state.
   */
  class CLuaCFBTypeEntry final : public CRuntimeFBTypeEntry, public std::enable_shared_from_this<CLuaCFBTypeEntry> {
    public:
      /*!\brief Reads the module in paEngine and creates the type entry
       *
       * The types of the internal FBs are looked up when an instance is created.
       *
       * @return Ready, InvalidObject if the module does not load, BadParams for a malformed module,
       *         UnsupportedType for an unknown data or adapter type
       */
      static EMGMResponse create(CLuaEngine &paEngine,
                                 StringId paTypeNameId,
                                 std::string paSource,
                                 std::shared_ptr<CLuaCFBTypeEntry> &paEntry,
                                 std::string &paError);

      //! Creates the type entry from the loaded module table at paModule of paState, which stays there
      static EMGMResponse create(lua_State *paState,
                                 int paModule,
                                 StringId paTypeNameId,
                                 std::string paSource,
                                 std::shared_ptr<CLuaCFBTypeEntry> &paEntry,
                                 std::string &paError);

      std::string_view getDefinition() const override {
        return mSource;
      }

      CFunctionBlock *createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const override;

      [[nodiscard]] const CLuaInterface &getInterface() const {
        return mInterface;
      }

      [[nodiscard]] const CLuaFBNetwork &getNetwork() const {
        return mNetwork;
      }

    private:
      CLuaCFBTypeEntry(StringId paTypeNameId, std::string paSource);

      const std::string mSource;
      CLuaInterface mInterface;
      CLuaFBNetwork mNetwork;
  };

  /*!\brief An instance of a Lua composite FB type
   *
   * The internal FBs are created by type name. An output driven from inside the network is the value of its source,
   * as in a compiled composite. The composite's own adapters are FBs of the network named like the adapter.
   *
   * An in-out variable is passed into the network as in a compiled composite: the internal FBs connected to it
   * work on the composite's own value, which takes the value its input connection refers to with an input event
   * and writes it back there with an output event. The in-out connection onwards refers to that same value.
   */
  class CLuaCFB final : private CLuaTypeHolder<CLuaCFBTypeEntry>, public CCompositeFB {
    public:
      CLuaCFB(std::shared_ptr<const CLuaCFBTypeEntry> paTypeEntry,
              StringId paInstanceNameId,
              CFBContainer &paContainer);

      //! False if a variable or an adapter could not be created
      bool setUp(std::string &paError);

      StringId getFBTypeId() const override {
        return mTypeEntry->getTypeNameId();
      }

      CIEC_ANY *getDI(TPortId paDINum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;
      CIEC_ANY *getDIO(TPortId paDIONum) override;

    protected:
      void setInitialValues() override;

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CInOutDataConnection **getDIOInConUnchecked(TPortId paDIONum) override;
      CInOutDataConnection *getDIOOutConUnchecked(TPortId paDIONum) override;
      IPlugPin *getPlugPinUnchecked(TPortId paPlugNum) override;
      ISocketPin *getSocketPinUnchecked(TPortId paSocketNum) override;

    private:
      //! Internal FBs can only be added while the network is created
      bool isDynamicContainer() override {
        return mCreatingNetwork;
      }

      bool createInternalFBs() override;
      void setFBNetworkInitialValues() override;
      CDataConnection *getIf2InConUnchecked(TPortId paDINum) override;
      CInOutDataConnection *getDIOOutConInternalUnchecked(TPortId paDIONum) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEO) override;

      //! Checks the parameters against the created internal FBs
      bool checkParameters();
      //! Points every output driven from inside the network to the value of its source
      bool bindInternalOutputs();

      std::vector<std::unique_ptr<CIEC_ANY>> mDOs;
      std::vector<CIEC_ANY *> mDOValues;
      std::vector<CDataConnection *> mDIConns;
      std::vector<CGenDataConnection> mIf2InConns;
      std::vector<CEventConnection> mEOConns;
      std::vector<CGenDataConnection> mDOConns;
      //! The composite's own values of the in-out variables, the internal FBs connected to them work on these
      std::vector<std::unique_ptr<CIEC_ANY>> mDIOs;
      //! The connection each in-out variable gets its value from, nullptr while it has none
      std::vector<CInOutDataConnection *> mDIOInConns;
      //! The in-out connections onwards, each refers to the value of its input connection or else to mDIOs
      std::vector<std::unique_ptr<CInOutDataConnection>> mDIOOutConns;
      //! The in-out connections into the network, each refers to mDIOs
      std::vector<std::unique_ptr<CInOutDataConnection>> mDIOIf2InConns;
      std::vector<std::unique_ptr<CTypedPlugPin>> mPlugs;
      std::vector<std::unique_ptr<CTypedSocketPin>> mSockets;
      bool mCreatingNetwork = false;
  };

} // namespace forte::lua
