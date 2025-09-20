/*******************************************************************************
 * Copyright (c) 2013, 2025 fortiss GmbH, TU Vienna/ACIN,
 *                          Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - fixes event chain initialisation, adds typifyAnyAdapter
 *   Alois Zoitl - introduced new CGenFB class for better handling generic FBs
                 - re-implementation for new communication infrastructure
 *******************************************************************************/
#include "anyadapter.h"
#include "forte/adapter.h"
#include "forte/funcbloc.h"
#include "typelib_internal.h"

using namespace forte::literals;

namespace forte {
  CAnyAdapterPin::CAnyAdapterPin(StringId paInstanceNameId) : mInstanceNameId(paInstanceNameId) {
  }

  CAnyAdapterPin::~CAnyAdapterPin() {
    removeConfiguredAdapter();
  }

  void CAnyAdapterPin::createConfiguredAdapter(CAdapter *paPeer,
                                               CFunctionBlock &paParent,
                                               bool paIsPlug,
                                               TForteUInt8 paParentAdapterlistID) {
    removeConfiguredAdapter();
    EMGMResponse errorMSG;
    mConfiguredAdapter = std::unique_ptr<CAdapter>(
        createAdapter(mInstanceNameId, paPeer->getFBTypeId(), paParent, paIsPlug, paParentAdapterlistID, errorMSG));

    if (mConfiguredAdapter) {
      paParent.addFB(*mConfiguredAdapter);
      mConfiguredAdapter->setPeer(paPeer);
    }
  }

  void CAnyAdapterPin::removeConfiguredAdapter() {
    if (mConfiguredAdapter) {
      mConfiguredAdapter->getParent().removeFB(*mConfiguredAdapter);
    }
  }

  CAnyPlugPin::CAnyPlugPin(StringId paInstanceNameId, CFunctionBlock &paParentFB, TForteUInt8 paParentAdapterlistID) :
      CAnyAdapterPin(paInstanceNameId),
      mAdapterCon(paParentFB, paParentAdapterlistID, *this) {
  }

  CAdapter *CAnyPlugPin::getAdapterBlock() {
    return mConfiguredAdapter.get();
  }

  StringId CAnyPlugPin::getAdapterTypeId() const {
    return (mConfiguredAdapter) ? mConfiguredAdapter->getFBTypeId() : "ANY_ADAPTER"_STRID;
  }

  bool CAnyPlugPin::isCompatible(IAdapterPin &paPeer) {
    return paPeer.getAdapterTypeId() != "ANY_ADAPTER"_STRID;
  }

  CAdapterConnection &CAnyPlugPin::getAdapterCon() {
    return mAdapterCon;
  }

  void CAnyPlugPin::setPeer(CAdapter *paPeer) {
    if (paPeer != nullptr) {
      auto srcInfo = mAdapterCon.getSourceId();
      createConfiguredAdapter(paPeer, srcInfo.getFB(), true, static_cast<TForteUInt8>(srcInfo.getPortId()));
    }
  }

  CAnySocketPin::CAnySocketPin(StringId paInstanceNameId,
                               CFunctionBlock &paParentFB,
                               TForteUInt8 paParentAdapterlistID) :
      CAnyAdapterPin(paInstanceNameId),
      mParentFB(paParentFB),
      mParentAdapterlistID(paParentAdapterlistID) {
  }

  CAdapter *CAnySocketPin::getAdapterBlock() {
    return mConfiguredAdapter.get();
  }

  StringId CAnySocketPin::getAdapterTypeId() const {
    return (mConfiguredAdapter) ? mConfiguredAdapter->getFBTypeId() : "ANY_ADAPTER"_STRID;
  }

  bool CAnySocketPin::isCompatible(IAdapterPin &paPeer) {
    return paPeer.getAdapterTypeId() != "ANY_ADAPTER"_STRID;
  }

  bool CAnySocketPin::connect(CAdapterConnection &paConn) {
    mAdapterCon = &paConn;
    createConfiguredAdapter(paConn.getPlug().getAdapterBlock(), mParentFB, true, mParentAdapterlistID);
    if (!mConfiguredAdapter) {
      return false;
    }
    paConn.getPlug().setPeer(getAdapterBlock());
    return true;
  }

  void CAnySocketPin::disconnect() {
    removeConfiguredAdapter();
    mAdapterCon = nullptr;
  }
} // namespace forte
