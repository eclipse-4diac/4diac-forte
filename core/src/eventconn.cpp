/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/eventconn.h"
#include "forte/ecet.h"
#include "forte/funcbloc.h"

namespace forte {
  CEventConnection::CEventConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId) :
      CConnection(paSrcFB, paSrcPortId) {
  }

  CEventConnection::~CEventConnection() = default;

  EMGMResponse CEventConnection::connect(CFunctionBlock &paDstFB, const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TEventID dstPortId = paDstFB.getFBInterfaceSpec().getEIID(paDstPortNameId.front());
    if (dstPortId == cgInvalidEventID) {
      return EMGMResponse::NoSuchObject;
    }

    const EMGMResponse retVal = addDestination(CConnectionPoint(paDstFB, dstPortId));
    paDstFB.addInputEventConnection(dstPortId);
    return retVal;
  }

  EMGMResponse CEventConnection::connectToCFBInterface(CFunctionBlock &paDstFB,
                                                       const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TEventID dstPortId = paDstFB.getFBInterfaceSpec().getEOID(paDstPortNameId.front());
    if (dstPortId == cgInvalidEventID) {
      return EMGMResponse::NoSuchObject;
    }

    return addDestination(CConnectionPoint(paDstFB, dstPortId | cgInternal2InterfaceMarker));
  }

  EMGMResponse CEventConnection::disconnect(CFunctionBlock &paDstFB, const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TEventID dstPortId = paDstFB.getFBInterfaceSpec().getEIID(paDstPortNameId.front());
    if (dstPortId == cgInvalidEventID) {
      return EMGMResponse::NoSuchObject;
    }

    const EMGMResponse retval = removeDestination(CConnectionPoint(paDstFB, dstPortId));
    paDstFB.removeInputEventConnection(dstPortId);
    return retval;
  }

  void CEventConnection::triggerEvent(CEventChainExecutionThread *paExecEnv) const {
    if (paExecEnv != nullptr) {
      for (const auto &runner : mDestinationIds) {
        paExecEnv->addEventEntry(runner);
      }
    }
  }

  EMGMResponse CEventConnection::addDestination(const CConnectionPoint &paDestPoint) {
    if (std::find(mDestinationIds.begin(), mDestinationIds.end(), paDestPoint) != mDestinationIds.end()) {
      return EMGMResponse::InvalidState;
    }
    mDestinationIds.push_back(paDestPoint);
    return EMGMResponse::Ready;
  }

  EMGMResponse CEventConnection::removeDestination(const CConnectionPoint &paDestPoint) {
    auto it = std::remove(mDestinationIds.begin(), mDestinationIds.end(), paDestPoint);
    if (it == mDestinationIds.end()) {
      return EMGMResponse::InvalidState;
    }
    mDestinationIds.erase(it, mDestinationIds.end());
    return EMGMResponse::Ready;
  }

  void CEventConnection::getSourcePortName(TNameIdentifier &paResult) const {
    paResult.push_back(getSourceId().getFB().getFBInterfaceSpec().mEONames[getSourceId().getPortId()]);
  }
} // namespace forte
