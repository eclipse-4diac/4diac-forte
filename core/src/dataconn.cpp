/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Rene Smodic, Gunnar Grabmaier, Ingo Hegny,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - move allocation of data value to FB
 *******************************************************************************/
#include "forte/dataconn.h"
#include "forte/funcbloc.h"
#include "forte/mgmcmd.h"
#include "gatherdataconn.h"
#include "membdataconn.h"
#include "negdataconn.h"

using namespace forte::literals;

namespace forte {
  CDataConnection::CDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId) :
      CConnection(paSrcFB, paSrcPortId) {
  }

  EMGMResponse CDataConnection::connect(CFunctionBlock &paDstFB, const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDIID(paDstPortNameId.front());
    if (dstPortId == cgInvalidPortId) {
      return EMGMResponse::NoSuchObject;
    }

    CIEC_ANY *dstDataPoint = paDstFB.getDIFromPortId(dstPortId);
    const EMGMResponse retVal = paDstPortNameId.size() == 1
                                    ? CDataConnection::establishDataConnection(paDstFB, dstPortId, *dstDataPoint)
                                    : establishGatheringConnection(paDstFB, dstPortId, *dstDataPoint, paDstPortNameId);
    if (retVal != EMGMResponse::Ready) {
      return retVal;
    }

    getSourceId().getFB().incConnRefCount();
    return EMGMResponse::Ready;
  }

  EMGMResponse CDataConnection::connectToCFBInterface(CFunctionBlock &paDstFB,
                                                      const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDOID(paDstPortNameId.front());
    if (dstPortId == cgInvalidPortId) {
      return EMGMResponse::NoSuchObject;
    }

    CIEC_ANY *dstDataPoint = paDstFB.getDataOutput(paDstPortNameId.front());
    return paDstPortNameId.size() == 1
               ? establishDataConnection(paDstFB, dstPortId | cgInternal2InterfaceMarker, *dstDataPoint)
               : establishGatheringConnection(paDstFB, dstPortId | cgInternal2InterfaceMarker, *dstDataPoint,
                                              paDstPortNameId);
  }

  void CDataConnection::handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint) {
    if (CIEC_ANY::e_ANY != paDstDataPoint.getDataTypeID()) {
      getValue().setValue(paDstDataPoint.unwrap());
      getSourceId().getFB().configureGenericDO(getSourceId().getPortId(), paDstDataPoint);
    }
  }

  EMGMResponse CDataConnection::disconnect(CFunctionBlock &paDstFB, const std::span<const StringId> paDstPortNameId) {
    if (paDstPortNameId.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    const TPortId dstPortId = paDstFB.getFBInterfaceSpec().getDIID(paDstPortNameId.front());
    if (cgInvalidPortId == dstPortId) {
      return EMGMResponse::NoSuchObject;
    }

    CDataConnection *dstConnection = paDstFB.getDIConnection(paDstPortNameId.front());
    if (!dstConnection) {
      return EMGMResponse::NoSuchObject;
    }

    if (paDstPortNameId.size() == 1) {
      if (dstConnection != this) {
        return EMGMResponse::NoSuchObject;
      }
      paDstFB.connectDI(dstPortId, nullptr);
    } else if (dstConnection->isGathering()) {
      const auto dstGatheringConnection = static_cast<internal::CGatheringDataConnection *>(dstConnection);
      EMGMResponse retVal = dstGatheringConnection->removeMemberConnection(paDstPortNameId.subspan(1));
      if (retVal != EMGMResponse::Ready) {
        return retVal;
      }
      if (dstGatheringConnection->empty()) {
        paDstFB.connectDI(dstPortId, nullptr);
        delete dstConnection;
      }
    } else {
      return EMGMResponse::NoSuchObject;
    }
    getSourceId().getFB().decConnRefCount();
    return EMGMResponse::Ready;
  }

  bool CDataConnection::canBeConnected(const CIEC_ANY &paSrcDataPoint, const CIEC_ANY &paDstDataPoint) {
    CIEC_ANY::EDataTypeID eSrcId = paSrcDataPoint.getDataTypeID();
    CIEC_ANY::EDataTypeID eDstId = paDstDataPoint.getDataTypeID();
    bool bCanConnect = false;

    if (eSrcId == eDstId) {
      bCanConnect = true;
    } else {
      if (((eSrcId == CIEC_ANY::e_ANY) && (eDstId != CIEC_ANY::e_ANY)) ||
          ((eSrcId != CIEC_ANY::e_ANY) && (eDstId == CIEC_ANY::e_ANY))) {
        bCanConnect = true;
      } else {
        bCanConnect = CIEC_ANY::isCastable(eSrcId, eDstId);
      }
    }
    return bCanConnect;
  }

  EMGMResponse CDataConnection::establishDataConnection(CFunctionBlock &paDstFB,
                                                        const TPortId paDstPortId,
                                                        const CIEC_ANY &paDstDataPoint) {
    if (getValue().getDataTypeID() == CIEC_ANY::e_ANY) {
      handleAnySrcPortConnection(paDstDataPoint);
    } else if (!canBeConnected(getValue(), paDstDataPoint)) {
      return EMGMResponse::InvalidOperation;
    }

    if (!paDstFB.connectDI(paDstPortId, this)) {
      return EMGMResponse::InvalidState;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CDataConnection::establishGatheringConnection(CFunctionBlock &paDstFB,
                                                             const TPortId paDstPortId,
                                                             CIEC_ANY &paDstDataPoint,
                                                             const std::span<const StringId> paDstPortNameId) {
    if (paDstDataPoint.getDataTypeID() != CIEC_ANY::e_STRUCT) {
      return EMGMResponse::NoSuchObject;
    }

    const CIEC_ANY *dstMember = paDstDataPoint.getVar(paDstPortNameId.subspan(1));
    if (!dstMember) {
      return EMGMResponse::NoSuchObject;
    }

    if (getValue().getDataTypeID() == CIEC_ANY::e_ANY) {
      handleAnySrcPortConnection(*dstMember);
    } else if (!canBeConnected(getValue(), *dstMember)) {
      return EMGMResponse::InvalidOperation;
    }

    CDataConnection *dstConnection = paDstFB.getDIConnection(paDstPortNameId.front());
    if (!dstConnection) {
      dstConnection = new internal::CGatheringDataConnection(paDstFB, paDstPortId, paDstDataPoint);
      if (!paDstFB.connectDI(paDstPortId, dstConnection)) {
        delete dstConnection;
        return EMGMResponse::InvalidState;
      }
    } else if (!dstConnection->isGathering()) {
      return EMGMResponse::InvalidState;
    }

    auto dstGatheringConnection = static_cast<internal::CGatheringDataConnection *>(dstConnection);
    return dstGatheringConnection->addMemberConnection(paDstPortNameId.subspan(1), *this);
  }

  CConnection::Wrapper CDataConnection::getDelegatingConnection(const std::span<const StringId> paSrcNameList) {
    if (paSrcNameList.empty()) {
      return Wrapper(this);
    }
    if (getValue().getDataTypeID() == CIEC_ANY::e_BOOL && paSrcNameList.size() == 1 &&
        paSrcNameList.front() == "NOT"_STRID) {
      return make_delegating<internal::CNegatingDataConnection>(getSourceId().getFB(), getSourceId().getPortId(),
                                                                static_cast<CIEC_BOOL &>(getValue()));
    }
    if (CIEC_ANY *member = getValue().getVar(paSrcNameList)) {
      return make_delegating<internal::CMemberDataConnection>(getSourceId().getFB(), getSourceId().getPortId(), *member,
                                                              paSrcNameList);
    }
    return {};
  }

  void CDataConnection::getSourcePortName(TNameIdentifier &paResult) const {
    paResult.push_back(getSourceId().getFB().getFBInterfaceSpec().mDONames[getSourceId().getPortId()]);
  }
} // namespace forte
