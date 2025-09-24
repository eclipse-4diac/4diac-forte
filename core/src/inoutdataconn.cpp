/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation
 *******************************************************************************/

#include "forte/inoutdataconn.h"

#include "forte/funcbloc.h"

namespace forte {
  EMGMResponse CInOutDataConnection::connect(CFunctionBlock &paDstFB, const std::span<const StringId> paDstPortNameId) {
    // Check if the superclass connect is working (connection with plain IN)
    EMGMResponse retVal = CDataConnection::connect(paDstFB, paDstPortNameId);
    if (retVal != EMGMResponse::NoSuchObject) {
      return retVal; // we already have a connection
    }

    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDIOID(paDstPortNameId.front());
    if (dstPortId == cgInvalidEventID) {
      return EMGMResponse::NoSuchObject;
    }

    const CIEC_ANY *dstDataPoint = paDstFB.getDIOFromPortId(dstPortId);
    retVal = establishDataConnection(paDstFB, dstPortId, *dstDataPoint);
    if (retVal != EMGMResponse::Ready) {
      return retVal;
    }

    paDstFB.incConnRefCount();
    getSourceId().getFB().incConnRefCount();
    return EMGMResponse::Ready;
  }

  EMGMResponse CInOutDataConnection::disconnect(CFunctionBlock &paDstFB,
                                                const std::span<const StringId> paDstPortNameId) {
    EMGMResponse retVal = CDataConnection::disconnect(paDstFB, paDstPortNameId);
    if (retVal != EMGMResponse::NoSuchObject) {
      return retVal; // we already have a connection
    }

    if (paDstPortNameId.size() != 1) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDIOID(paDstPortNameId.front());
    if (dstPortId == cgInvalidEventID) {
      return EMGMResponse::NoSuchObject;
    }

    auto it =
        std::remove(mInOutDestinationIds.begin(), mInOutDestinationIds.end(), CConnectionPoint(paDstFB, dstPortId));
    if (it == mInOutDestinationIds.end()) {
      return EMGMResponse::NoSuchObject;
    }
    mInOutDestinationIds.erase(it, mInOutDestinationIds.end());
    paDstFB.connectDIO(dstPortId, nullptr);
    paDstFB.decConnRefCount();
    getSourceId().getFB().decConnRefCount();
    return EMGMResponse::Ready;
  }

  void CInOutDataConnection::setValue(CIEC_ANY *paValue) {
    mValue = paValue;
    for (auto connectionPoint : mInOutDestinationIds) {
      connectionPoint.getFB().connectDIO(connectionPoint.getPortId(), this);
    }
  }

  EMGMResponse CInOutDataConnection::establishDataConnection(CFunctionBlock &paDstFB,
                                                             const TPortId paDstPortId,
                                                             const CIEC_ANY &paDstDataPoint) {
    if (getValue().getDataTypeID() == CIEC_ANY::e_ANY) {
      handleAnySrcPortConnection(paDstDataPoint);
    } else if (!canBeConnected(getValue(), paDstDataPoint)) {
      return EMGMResponse::InvalidOperation;
    }

    CConnectionPoint dstPoint(paDstFB, paDstPortId);
    if (std::find(mInOutDestinationIds.begin(), mInOutDestinationIds.end(), dstPoint) != mInOutDestinationIds.end()) {
      return EMGMResponse::InvalidState;
    }
    if (!paDstFB.connectDIO(paDstPortId, this)) {
      return EMGMResponse::InvalidState;
    }
    mInOutDestinationIds.push_back(dstPoint);
    return EMGMResponse::Ready;
  }

  void CInOutDataConnection::getSourcePortName(TNameIdentifier &paResult) const {
    paResult.push_back(getSourceId().getFB().getFBInterfaceSpec().mDIONames[getSourceId().getPortId()]);
  }
} // namespace forte
