/*******************************************************************************
 * Copyright (c) 2017, 2026 fortiss GmbH, Johannes Kepler University Linz, Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - upgraded to new FB memory layout
 *   Pedro Ricardo C. Souza - pin based adapters, type entry merged in
 *******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "forte/adapter.h"
#include "forte/dataconn.h"
#include "forte/eventconn.h"
#include "luainterface.h"
#include "../typelib_internal.h"

namespace forte::lua {

  class CLuaEngine;

  //! An adapter type defined at run time by a Lua module that returns {interfaceSpec = …}
  class CLuaAdapterTypeEntry final : public CRuntimeAdapterTypeEntry,
                                     public std::enable_shared_from_this<CLuaAdapterTypeEntry> {
    public:
      /*!\brief Reads the module in paEngine and creates the type entry
       *
       * @return Ready, InvalidObject if the module does not load, BadParams for a malformed interface,
       *         UnsupportedType for an unknown data type
       */
      static EMGMResponse create(CLuaEngine &paEngine,
                                 StringId paTypeNameId,
                                 std::string paSource,
                                 std::shared_ptr<CLuaAdapterTypeEntry> &paEntry,
                                 std::string &paError);

      std::string_view getDefinition() const override {
        return mSource;
      }

      CAdapter *createAdapterInstance(StringId paInstanceNameId,
                                      CFBContainer &paContainer,
                                      bool paIsPlug,
                                      TForteUInt8 paParentAdapterlistID) const override;

      [[nodiscard]] const CLuaInterface &getInterface() const {
        return mInterface;
      }

    private:
      CLuaAdapterTypeEntry(StringId paTypeNameId, std::string paSource);

      const std::string mSource;
      CLuaInterface mInterface;
  };

  /*!\brief One side of an adapter of a Lua adapter type
   *
   * A socket has the declared interface, a plug the mirrored one. Data inputs sampled with an event input are
   * copied to the matching data outputs of the peer, as in compiled adapters.
   */
  class CLuaAdapter final : private CLuaTypeHolder<CLuaAdapterTypeEntry>, public CAdapter {
    public:
      CLuaAdapter(std::shared_ptr<const CLuaAdapterTypeEntry> paTypeEntry,
                  StringId paInstanceNameId,
                  CFBContainer &paContainer,
                  bool paIsPlug,
                  TForteUInt8 paParentAdapterlistID);
      ~CLuaAdapter() override;

      StringId getFBTypeId() const override {
        return mTypeEntry->getTypeNameId();
      }

      CIEC_ANY *getDI(TPortId paDINum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    protected:
      void setInitialValues() override;

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEO) override;

      std::span<const TPortId> getEIWith(TEventID paEIID) const;
      std::span<const TPortId> getEOWith(TEventID paEO) const;

      const bool mIsPlug;
      std::vector<std::unique_ptr<CIEC_ANY>> mDIs;
      std::vector<std::unique_ptr<CIEC_ANY>> mDOs;
      std::vector<CDataConnection *> mDIConns;
      std::vector<CEventConnection> mEOConns;
      std::vector<CGenDataConnection> mDOConns;
  };

} // namespace forte::lua
