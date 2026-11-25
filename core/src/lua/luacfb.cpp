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

#include "luacfb.h"

extern "C" {
#include <lua.h>
}

#include <algorithm>

#include "forte/adapter.h"
#include "forte/resource.h"
#include "forte/util/devlog.h"
#include "luaengine.h"
#include "luatypehash.h"

namespace forte::lua {
  CLuaCFBTypeEntry::CLuaCFBTypeEntry(StringId paTypeNameId, std::string paSource) :
      CRuntimeFBTypeEntry(paTypeNameId, getInternedTypeHash(paSource)),
      mSource(std::move(paSource)) {
  }

  EMGMResponse CLuaCFBTypeEntry::create(CLuaEngine &paEngine,
                                        StringId paTypeNameId,
                                        std::string paSource,
                                        std::shared_ptr<CLuaCFBTypeEntry> &paEntry,
                                        std::string &paError) {
    if (!paEngine.isValid()) {
      paError = "could not set up a Lua state";
      return EMGMResponse::Overflow;
    }
    if (!paEngine.loadModule(paSource, paTypeNameId.data(), paError)) {
      return EMGMResponse::InvalidObject;
    }
    lua_State *const state = paEngine.getState();
    const EMGMResponse response = create(state, lua_gettop(state), paTypeNameId, std::move(paSource), paEntry, paError);
    lua_pop(state, 1);
    return response;
  }

  EMGMResponse CLuaCFBTypeEntry::create(lua_State *paState,
                                        int paModule,
                                        StringId paTypeNameId,
                                        std::string paSource,
                                        std::shared_ptr<CLuaCFBTypeEntry> &paEntry,
                                        std::string &paError) {
    std::shared_ptr<CLuaCFBTypeEntry> entry(new CLuaCFBTypeEntry(paTypeNameId, std::move(paSource)));

    lua_pushliteral(paState, "interfaceSpec");
    lua_rawget(paState, paModule);
    EMGMResponse response = entry->mInterface.parse(paState, -1, true, paError);
    lua_pop(paState, 1);

    if (response == EMGMResponse::Ready) {
      lua_pushliteral(paState, "fbnSpec");
      lua_rawget(paState, paModule);
      response = entry->mNetwork.parse(paState, -1, entry->mInterface, paTypeNameId, paError);
      lua_pop(paState, 1);
    }
    lua_settop(paState, paModule);

    if (response == EMGMResponse::Ready) {
      paEntry = std::move(entry);
    }
    return response;
  }

  CFunctionBlock *CLuaCFBTypeEntry::createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const {
    auto instance = std::make_unique<CLuaCFB>(shared_from_this(), paInstanceNameId, paContainer);
    if (std::string error; !instance->setUp(error)) {
      DEVLOG_ERROR("[CLuaCFB] Could not create %s of type %s: %s\n", paInstanceNameId.data(), getTypeNameId().data(),
                   error.c_str());
      return nullptr;
    }
    return instance.release();
  }

  CLuaCFB::CLuaCFB(std::shared_ptr<const CLuaCFBTypeEntry> paTypeEntry,
                   StringId paInstanceNameId,
                   CFBContainer &paContainer) :
      CLuaTypeHolder(std::move(paTypeEntry)),
      CCompositeFB(
          paContainer, mTypeEntry->getInterface().getSpec(), paInstanceNameId, mTypeEntry->getNetwork().getFBNData()),
      mDOs(CLuaInterface::createValues(mTypeEntry->getInterface().getDOTypes())),
      mDIConns(mTypeEntry->getInterface().getDITypes().size(), nullptr),
      mDIOs(CLuaInterface::createValues(mTypeEntry->getInterface().getDIOTypes())),
      mDIOInConns(mDIOs.size(), nullptr) {
    const SFBInterfaceSpec &spec = getFBInterfaceSpec();
    const auto diValues = CLuaInterface::createValues(mTypeEntry->getInterface().getDITypes());
    const auto isMissing = [](const std::unique_ptr<CIEC_ANY> &paValue) { return paValue == nullptr; };
    if (std::none_of(diValues.begin(), diValues.end(), isMissing) &&
        std::none_of(mDOs.begin(), mDOs.end(), isMissing) && std::none_of(mDIOs.begin(), mDIOs.end(), isMissing)) {
      // the interface to internal connections hold the values of the data inputs
      mIf2InConns.reserve(diValues.size());
      for (TPortId i = 0; i < diValues.size(); ++i) {
        mIf2InConns.emplace_back(*this, i, *diValues[i]);
      }
      mDOConns.reserve(mDOs.size());
      for (TPortId i = 0; i < mDOs.size(); ++i) {
        mDOConns.emplace_back(*this, i, *mDOs[i]);
        mDOValues.push_back(mDOs[i].get());
      }
      // both in-out connections start out on the composite's own value, only the one onwards is redirected to the
      // value of its input connection
      mDIOOutConns.reserve(mDIOs.size());
      mDIOIf2InConns.reserve(mDIOs.size());
      for (TPortId i = 0; i < mDIOs.size(); ++i) {
        mDIOOutConns.push_back(std::make_unique<CInOutDataConnection>(*this, i, mDIOs[i].get()));
        mDIOIf2InConns.push_back(std::make_unique<CInOutDataConnection>(*this, i, mDIOs[i].get()));
      }
    }
    mEOConns.reserve(spec.getNumEOs());
    for (TPortId i = 0; i < spec.getNumEOs(); ++i) {
      mEOConns.emplace_back(*this, i);
    }
    // events of a composite's own adapter go to the adapter FB inside the network, not to the composite
    for (const SLuaAdapterDefinition &adapter : mTypeEntry->getInterface().getAdapters()) {
      if (adapter.mIsPlug) {
        mPlugs.push_back(
            std::make_unique<CTypedPlugPin>(adapter.mName, *this, adapter.mTypeName, cgCFBParentAdapterlistIDMarker));
      } else {
        mSockets.push_back(
            std::make_unique<CTypedSocketPin>(adapter.mName, *this, adapter.mTypeName, cgCFBParentAdapterlistIDMarker));
      }
    }
  }

  bool CLuaCFB::setUp(std::string &paError) {
    const auto hasNoBlock = [](const auto &paPin) { return paPin->getAdapterBlock() == nullptr; };
    if (std::any_of(mPlugs.begin(), mPlugs.end(), hasNoBlock) ||
        std::any_of(mSockets.begin(), mSockets.end(), hasNoBlock)) {
      paError = "could not create an adapter";
      return false;
    }
    if (mIf2InConns.size() != mDIConns.size() || mDOConns.size() != mDOs.size() ||
        mDIOOutConns.size() != mDIOs.size()) {
      paError = "could not create the variables";
      return false;
    }
    return true;
  }

  CIEC_ANY *CLuaCFB::getDI(TPortId paDINum) {
    return (paDINum < mIf2InConns.size()) ? &mIf2InConns[paDINum].getValue() : nullptr;
  }

  CIEC_ANY *CLuaCFB::getDO(TPortId paDONum) {
    return (paDONum < mDOValues.size()) ? mDOValues[paDONum] : nullptr;
  }

  CIEC_ANY *CLuaCFB::getDIO(TPortId paDIONum) {
    return (paDIONum < mDIOs.size()) ? mDIOs[paDIONum].get() : nullptr;
  }

  void CLuaCFB::setInitialValues() {
    CCompositeFB::setInitialValues();
    const auto diTypes = mTypeEntry->getInterface().getDITypes();
    for (std::size_t i = 0; i < diTypes.size() && i < mIf2InConns.size(); ++i) {
      diTypes[i].setInitialValue(mIf2InConns[i].getValue());
    }
    CLuaInterface::setInitialValues(mTypeEntry->getInterface().getDOTypes(), mDOs);
    CLuaInterface::setInitialValues(mTypeEntry->getInterface().getDIOTypes(), mDIOs);
  }

  CEventConnection *CLuaCFB::getEOConUnchecked(TPortId paEONum) {
    return &mEOConns[paEONum];
  }

  CDataConnection *CLuaCFB::getDOConUnchecked(TPortId paDONum) {
    return &mDOConns[paDONum];
  }

  CDataConnection **CLuaCFB::getDIConUnchecked(TPortId paDINum) {
    return &mDIConns[paDINum];
  }

  CInOutDataConnection **CLuaCFB::getDIOInConUnchecked(TPortId paDIONum) {
    return &mDIOInConns[paDIONum];
  }

  CInOutDataConnection *CLuaCFB::getDIOOutConUnchecked(TPortId paDIONum) {
    return mDIOOutConns[paDIONum].get();
  }

  IPlugPin *CLuaCFB::getPlugPinUnchecked(TPortId paPlugNum) {
    return mPlugs[paPlugNum].get();
  }

  ISocketPin *CLuaCFB::getSocketPinUnchecked(TPortId paSocketNum) {
    return mSockets[paSocketNum].get();
  }

  CDataConnection *CLuaCFB::getIf2InConUnchecked(TPortId paDINum) {
    return &mIf2InConns[paDINum];
  }

  CInOutDataConnection *CLuaCFB::getDIOOutConInternalUnchecked(TPortId paDIONum) {
    return mDIOIf2InConns[paDIONum].get();
  }

  bool CLuaCFB::createInternalFBs() {
    // a composite that contains itself, directly or through another type, would never finish creating
    // a resource is a function block as well, the search ends there
    for (CFBContainer *container = &getParent(); container->isFB() && container != container->getResource();
         container = &container->getParent()) {
      // instances of one type entry share its interface spec
      if (&static_cast<CFunctionBlock *>(container)->getFBInterfaceSpec() == &getFBInterfaceSpec()) {
        DEVLOG_ERROR("[CLuaCFB] %s: the type %s contains itself\n", getInstanceName(), getFBTypeId().data());
        return false;
      }
    }

    mCreatingNetwork = true;
    bool success = true;
    for (const SLuaInternalFB &internalFB : mTypeEntry->getNetwork().getInternalFBs()) {
      if (const EMGMResponse response = createFB(internalFB.mName, internalFB.mTypeName, {});
          response != EMGMResponse::Ready) {
        DEVLOG_ERROR("[CLuaCFB] %s: could not create the internal FB %s of type %s\n", getInstanceName(),
                     internalFB.mName.data(), internalFB.mTypeName.data());
        success = false;
        break;
      }
    }
    mCreatingNetwork = false;
    return success && checkParameters() && bindInternalOutputs();
  }

  bool CLuaCFB::checkParameters() {
    for (const SLuaParameter &parameter : mTypeEntry->getNetwork().getParameters()) {
      CFunctionBlock *const internalFB = getFB(parameter.mFBName);
      const CIEC_ANY *const input = (internalFB != nullptr) ? internalFB->getDataInput(parameter.mDIName) : nullptr;
      if (input == nullptr) {
        DEVLOG_ERROR("[CLuaCFB] %s: the internal FB %s has no data input %s\n", getInstanceName(),
                     parameter.mFBName.data(), parameter.mDIName.data());
        return false;
      }
      const std::unique_ptr<CIEC_ANY> probe(input->clone(nullptr));
      if (probe->fromString(parameter.mValue.c_str()) != static_cast<int>(parameter.mValue.size())) {
        DEVLOG_ERROR("[CLuaCFB] %s: invalid value %s for %s.%s\n", getInstanceName(), parameter.mValue.c_str(),
                     parameter.mFBName.data(), parameter.mDIName.data());
        return false;
      }
    }
    return true;
  }

  bool CLuaCFB::bindInternalOutputs() {
    for (const SCFB_FBConnectionData &connection : mTypeEntry->getNetwork().getFBNData().mDataConnections) {
      if (connection.mDstFBNameId) {
        continue;
      }
      const StringId srcName = std::get<StringId>(connection.mSrcId);
      const TPortId doNum = getFBInterfaceSpec().getDOID(std::get<StringId>(connection.mDstId));
      CIEC_ANY *source = nullptr;
      if (connection.mSrcFBNameId) {
        CFunctionBlock *const srcFB = getFB(connection.mSrcFBNameId);
        source = (srcFB != nullptr) ? srcFB->getDataOutput(srcName) : nullptr;
      } else {
        source = getDataInput(srcName);
      }
      if (source == nullptr || doNum >= mDOValues.size()) {
        DEVLOG_ERROR("[CLuaCFB] %s: no data output %s to drive %s\n", getInstanceName(), srcName.data(),
                     std::get<StringId>(connection.mDstId).data());
        return false;
      }
      mDOValues[doNum] = source;
    }
    return true;
  }

  void CLuaCFB::setFBNetworkInitialValues() {
    for (const SLuaParameter &parameter : mTypeEntry->getNetwork().getParameters()) {
      if (CFunctionBlock *const internalFB = getFB(parameter.mFBName); internalFB != nullptr) {
        if (CIEC_ANY *const input = internalFB->getDataInput(parameter.mDIName); input != nullptr) {
          input->fromString(parameter.mValue.c_str());
        }
      }
    }
  }

  void CLuaCFB::readInputData(TEventID paEIID) {
    const std::size_t numDIs = mDIConns.size();
    for (const TPortId diNum : mTypeEntry->getInterface().getEIWith(paEIID)) {
      if (diNum < numDIs) {
        readData(diNum, mIf2InConns[diNum].getValue(), mDIConns[diNum]);
      } else {
        // an in-out variable takes the value its input connection refers to
        const std::size_t dio = diNum - numDIs;
        readData(numDIs + mDOs.size() + dio, *mDIOs[dio], mDIOOutConns[dio].get());
      }
    }
  }

  void CLuaCFB::writeOutputData(TEventID paEO) {
    const std::size_t numDIs = mDIConns.size();
    const std::size_t numDOs = mDOs.size();
    for (const TPortId doNum : mTypeEntry->getInterface().getEOWith(paEO)) {
      if (doNum < numDOs) {
        writeData(numDIs + doNum, *mDOValues[doNum], mDOConns[doNum]);
      } else {
        // an in-out variable writes back to the value its input connection refers to
        const std::size_t dio = doNum - numDOs;
        writeData(numDIs + numDOs + dio, *mDIOs[dio], *mDIOOutConns[dio]);
      }
    }
  }

} // namespace forte::lua
