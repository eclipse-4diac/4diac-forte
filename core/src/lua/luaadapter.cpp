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

#include "luaadapter.h"

extern "C" {
#include <lua.h>
}

#include "luaengine.h"
#include "luatypehash.h"

namespace forte::lua {

  CLuaAdapterTypeEntry::CLuaAdapterTypeEntry(StringId paTypeNameId, std::string paSource) :
      CRuntimeAdapterTypeEntry(paTypeNameId, getInternedTypeHash(paSource)),
      mSource(std::move(paSource)) {
  }

  EMGMResponse CLuaAdapterTypeEntry::create(CLuaEngine &paEngine,
                                            StringId paTypeNameId,
                                            std::string paSource,
                                            std::shared_ptr<CLuaAdapterTypeEntry> &paEntry,
                                            std::string &paError) {
    std::shared_ptr<CLuaAdapterTypeEntry> entry(new CLuaAdapterTypeEntry(paTypeNameId, std::move(paSource)));
    if (!paEngine.isValid()) {
      paError = "could not set up a Lua state";
      return EMGMResponse::Overflow;
    }
    if (!paEngine.loadModule(entry->mSource, paTypeNameId.data(), paError)) {
      return EMGMResponse::InvalidObject;
    }
    lua_State *const state = paEngine.getState();
    lua_pushliteral(state, "interfaceSpec");
    lua_rawget(state, -2);
    EMGMResponse response = entry->mInterface.parse(state, -1, false, paError);
    lua_pop(state, 2);
    if (response == EMGMResponse::Ready && !entry->mInterface.getDIOTypes().empty()) {
      paError = "adapter types have no in-out variables";
      response = EMGMResponse::BadParams;
    }
    if (response == EMGMResponse::Ready) {
      paEntry = std::move(entry);
    }
    return response;
  }

  CAdapter *CLuaAdapterTypeEntry::createAdapterInstance(StringId paInstanceNameId,
                                                        CFBContainer &paContainer,
                                                        bool paIsPlug,
                                                        TForteUInt8 paParentAdapterlistID) const {
    return new CLuaAdapter(shared_from_this(), paInstanceNameId, paContainer, paIsPlug, paParentAdapterlistID);
  }

  CLuaAdapter::CLuaAdapter(std::shared_ptr<const CLuaAdapterTypeEntry> paTypeEntry,
                           StringId paInstanceNameId,
                           CFBContainer &paContainer,
                           bool paIsPlug,
                           TForteUInt8 paParentAdapterlistID) :
      CLuaTypeHolder(std::move(paTypeEntry)),
      CAdapter(paContainer,
               paIsPlug ? mTypeEntry->getInterface().getMirroredSpec() : mTypeEntry->getInterface().getSpec(),
               paInstanceNameId,
               paParentAdapterlistID),
      mIsPlug(paIsPlug),
      mDIs(CLuaInterface::createValues(paIsPlug ? mTypeEntry->getInterface().getDOTypes()
                                                : mTypeEntry->getInterface().getDITypes())),
      mDOs(CLuaInterface::createValues(paIsPlug ? mTypeEntry->getInterface().getDITypes()
                                                : mTypeEntry->getInterface().getDOTypes())),
      mDIConns(mDIs.size(), nullptr) {
    const SFBInterfaceSpec &spec = getFBInterfaceSpec();
    mEOConns.reserve(spec.getNumEOs());
    for (TPortId i = 0; i < spec.getNumEOs(); ++i) {
      mEOConns.emplace_back(*this, i);
    }
    mDOConns.reserve(mDOs.size());
    for (TPortId i = 0; i < mDOs.size(); ++i) {
      mDOConns.emplace_back(*this, i, *mDOs[i]);
    }
  }

  CLuaAdapter::~CLuaAdapter() = default;

  CIEC_ANY *CLuaAdapter::getDI(TPortId paDINum) {
    return (paDINum < mDIs.size()) ? mDIs[paDINum].get() : nullptr;
  }

  CIEC_ANY *CLuaAdapter::getDO(TPortId paDONum) {
    return (paDONum < mDOs.size()) ? mDOs[paDONum].get() : nullptr;
  }

  void CLuaAdapter::setInitialValues() {
    CAdapter::setInitialValues();
    const CLuaInterface &interface = mTypeEntry->getInterface();
    CLuaInterface::setInitialValues(mIsPlug ? interface.getDOTypes() : interface.getDITypes(), mDIs);
    CLuaInterface::setInitialValues(mIsPlug ? interface.getDITypes() : interface.getDOTypes(), mDOs);
  }

  CEventConnection *CLuaAdapter::getEOConUnchecked(TPortId paEONum) {
    return &mEOConns[paEONum];
  }

  CDataConnection *CLuaAdapter::getDOConUnchecked(TPortId paDONum) {
    return &mDOConns[paDONum];
  }

  CDataConnection **CLuaAdapter::getDIConUnchecked(TPortId paDINum) {
    return &mDIConns[paDINum];
  }

  std::span<const TPortId> CLuaAdapter::getEIWith(TEventID paEIID) const {
    return mIsPlug ? mTypeEntry->getInterface().getEOWith(paEIID) : mTypeEntry->getInterface().getEIWith(paEIID);
  }

  std::span<const TPortId> CLuaAdapter::getEOWith(TEventID paEO) const {
    return mIsPlug ? mTypeEntry->getInterface().getEIWith(paEO) : mTypeEntry->getInterface().getEOWith(paEO);
  }

  void CLuaAdapter::readInputData(TEventID paEIID) {
    CAdapter *const peer = getPeer();
    for (const TPortId di : getEIWith(paEIID)) {
      readData(di, *mDIs[di], mDIConns[di]);
      // the peer's data output with the same index is the other end of this data input
      if (CIEC_ANY *const peerDO = (peer != nullptr) ? peer->getDO(di) : nullptr; peerDO != nullptr) {
        peerDO->setValue(*mDIs[di]);
      }
    }
  }

  void CLuaAdapter::writeOutputData(TEventID paEO) {
    const std::size_t numDIs = mDIs.size();
    for (const TPortId dataOutput : getEOWith(paEO)) {
      writeData(numDIs + dataOutput, *mDOs[dataOutput], mDOConns[dataOutput]);
    }
  }

} // namespace forte::lua
