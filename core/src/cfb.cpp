/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University Linz
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gunnar Grabmaier, Gerhard Ebenhofer,
 *    Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - added support for adapter connections in CFBs
 *    Martin Jobst - add smart pointer for internal FBs
 *******************************************************************************/
#include <string.h>
#include "forte/cfb.h"
#include "forte/adapter.h"
#include "forte/stringid.h"

namespace forte {
  namespace {
    [[nodiscard]] constexpr StringId getId(const std::variant<StringId, std::span<const StringId>> &paId) {
      return std::visit(
          []<typename T>(T &&arg) -> StringId {
            if constexpr (std::is_same_v<std::decay_t<T>, StringId>) {
              return arg;
            } else {
              return arg.size() == 1 ? arg.front() : StringId{};
            }
          },
          paId);
    }

    [[nodiscard]] constexpr std::span<const StringId>
    getIds(const std::variant<StringId, std::span<const StringId>> &paId) {
      return std::visit(
          []<typename T>(T &&arg) -> std::span<const StringId> {
            if constexpr (std::is_same_v<std::decay_t<T>, StringId>) {
              return {&arg, 1};
            } else {
              return arg;
            }
          },
          paId);
    }

    bool forwardGenericDI(CFunctionBlock &dstFB, const StringId dstId, const CIEC_ANY &paRefValue) {
      const TPortId dstDIPortId = dstFB.getFBInterfaceSpec().getDIID(dstId);
      if (dstDIPortId == cgInvalidPortId) {
        return false;
      }
      dstFB.configureGenericDI(dstDIPortId, paRefValue);
      return true;
    }

    bool forwardGenericDIO(CFunctionBlock &dstFB, const StringId dstId, const CIEC_ANY &paRefValue) {
      const TPortId dstDIOPortId = dstFB.getFBInterfaceSpec().getDIOID(dstId);
      if (dstDIOPortId == cgInvalidPortId) {
        return false;
      }
      dstFB.configureGenericDIO(dstDIOPortId, paRefValue);
      return true;
    }

    bool forwardGenericDO(CFunctionBlock &srcFB, const StringId srcId, const CIEC_ANY &paRefValue) {
      const TPortId dstDOPortId = srcFB.getFBInterfaceSpec().getDOID(srcId);
      if (dstDOPortId == cgInvalidPortId) {
        return false;
      }
      srcFB.configureGenericDO(dstDOPortId, paRefValue);
      return true;
    }
  } // namespace

  CCompositeFB::CCompositeFB(CFBContainer &paContainer,
                             const SFBInterfaceSpec &paInterfaceSpec,
                             StringId paInstanceNameId,
                             const SCFB_FBNData &paFBNData) :
      CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
      cmFBNData(paFBNData) {
  }

  bool CCompositeFB::initialize() {
    if (!CFunctionBlock::initialize()) {
      return false;
    }

    if (!createInternalFBs()) {
      return false;
    }

    setInitialValues();
    prepareIf2InEventCons();
    if (!createEventConnections()) {
      return false;
    }
    if (!createDataConnections()) {
      return false;
    }
    if (!createAdapterConnections()) {
      return false;
    }
    setFBNetworkInitialValues();

    return true;
  }

  bool CCompositeFB::connectDI(TPortId paDIPortId, CDataConnection *paDataCon) {
    if (cgInternal2InterfaceMarker & paDIPortId) { // internal-to-interface connection?
      return (paDIPortId & cgInternal2InterfaceRemovalMask) < getFBInterfaceSpec().getNumDOs();
    }
    return CFunctionBlock::connectDI(paDIPortId, paDataCon);
  }

  bool CCompositeFB::configureGenericDI(const TPortId paDIPortId, const CIEC_ANY &paRefValue) {
    const StringId ifSrcId = getFBInterfaceSpec().mDINames[paDIPortId];
    for (const auto &[srcFBNameId, srcId, dstFBNameId, dstId] : cmFBNData.mDataConnections) {
      if (!srcFBNameId && getId(srcId) == ifSrcId) {
        if (CFunctionBlock *dstFB = getFB(dstFBNameId); !dstFB || !forwardGenericDI(*dstFB, getId(dstId), paRefValue)) {
          return false;
        }
      }
    }
    return CFunctionBlock::configureGenericDI(paDIPortId, paRefValue);
  }

  bool CCompositeFB::configureGenericDIO(const TPortId paDIOPortId, const CIEC_ANY &paRefValue) {
    const StringId ifSrcId = getFBInterfaceSpec().mDIONames[paDIOPortId];
    for (const auto &[srcFBNameId, srcId, dstFBNameId, dstId] : cmFBNData.mDataConnections) {
      if (!srcFBNameId && getId(srcId) == ifSrcId) {
        if (CFunctionBlock *dstFB = getFB(dstFBNameId);
            !dstFB || (!forwardGenericDI(*dstFB, getId(dstId), paRefValue) &&
                       !forwardGenericDIO(*dstFB, getId(dstId), paRefValue))) {
          return false;
        }
      }
    }
    return CFunctionBlock::configureGenericDIO(paDIOPortId, paRefValue);
  }

  bool CCompositeFB::configureGenericDO(const TPortId paDOPortId, const CIEC_ANY &paRefValue) {
    const StringId ifDstId = getFBInterfaceSpec().mDONames[paDOPortId];
    for (const auto &[srcFBNameId, srcId, dstFBNameId, dstId] : cmFBNData.mDataConnections) {
      if (!dstFBNameId && getId(dstId) == ifDstId) {
        if (CFunctionBlock *srcFB = getFB(srcFBNameId); !srcFB || !forwardGenericDO(*srcFB, getId(srcId), paRefValue)) {
          return false;
        }
      }
    }
    return CFunctionBlock::configureGenericDO(paDOPortId, paRefValue);
  }

  EMGMResponse CCompositeFB::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse nRetVal = CFunctionBlock::changeExecutionState(paCommand);

    // Update FB parameters that maybe got overwritten by default values of the FB
    if ((EMGMCommandType::Reset == paCommand) && (E_FBStates::Idle == getState())) {
      setFBNetworkInitialValues();
    }
    return nRetVal;
  }

  void CCompositeFB::setInitialValues() {
    CFunctionBlock::setInitialValues();
    // currently nothing to do
  }

  void CCompositeFB::setFBNetworkInitialValues() {
    // per default we do not have initial values of internal blocks
  }

  void CCompositeFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (cgInternal2InterfaceMarker & paEIID) {
      TEventID internalEvent = static_cast<TEventID>(paEIID & cgInternal2InterfaceRemovalMask);
      sendOutputEvent(internalEvent, paECET);
    } else {
      if (paEIID < getFBInterfaceSpec().getNumEIs() && mInterface2InternalEventCons[paEIID]) {
        mInterface2InternalEventCons[paEIID]->triggerEvent(paECET);
      }
    }
  }

  void CCompositeFB::prepareIf2InEventCons() {
    mInterface2InternalEventCons.reserve(getFBInterfaceSpec().getNumEIs());
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumEIs(); i++) {
      mInterface2InternalEventCons.emplace_back(std::make_unique<CEventConnection>(*this, i));
    }
  }

  bool CCompositeFB::createEventConnections() {
    return std::ranges::all_of(cmFBNData.mEventConnections,
                               [this](const auto &conn) { return establishConnection(conn); });
  }

  bool CCompositeFB::createDataConnections() {
    return std::ranges::all_of(cmFBNData.mDataConnections, [this](const auto &conn) {
      if (!establishConnection(conn)) {
        return false;
      }
      if (!conn.mSrcFBNameId) {
        // Data connections track on the source side the number of destinations. For interface to internals we don't
        // want that. Therefore, we have to revert that ref count change.
        decConnRefCount();
      }
      return true;
    });
  }

  bool CCompositeFB::createAdapterConnections() {
    return std::ranges::all_of(cmFBNData.mAdapterConnections,
                               [this](const auto &conn) { return establishConnection(conn); });
  }

  bool CCompositeFB::establishConnection(const SCFB_FBConnectionData &paConnectionData) {
    CConnection::Wrapper con = getInternalConnection(paConnectionData.mSrcFBNameId, getIds(paConnectionData.mSrcId));
    if (!con) {
      return false;
    }
    EMGMResponse retVal;
    if (paConnectionData.mDstFBNameId) {
      CFunctionBlock *dstFB = getFB(paConnectionData.mDstFBNameId);
      if (!dstFB) {
        return false;
      }
      retVal = con->connect(*dstFB, getIds(paConnectionData.mDstId));
    } else {
      retVal = con->connectToCFBInterface(*this, getIds(paConnectionData.mDstId));
    }
    if (retVal != EMGMResponse::Ready) {
      return false;
    }
    // the connection has registered itself in the destination FB, so release it here
    con.release(); // NOLINT(bugprone-unused-return-value)
    return true;
  }

  CConnection::Wrapper CCompositeFB::getInternalConnection(const StringId paSrcFBNameId,
                                                           const std::span<const StringId> paSrcNameList) {
    if (paSrcFBNameId) {
      CFunctionBlock *srcFB = getFB(paSrcFBNameId);
      if (!srcFB) {
        return {};
      }
      return srcFB->getOutputConnection(paSrcNameList);
    }
    return getInternalConnection(paSrcNameList);
  }

  CConnection::Wrapper CCompositeFB::getInternalConnection(const std::span<const StringId> paSrcNameList) {
    if (paSrcNameList.empty()) {
      return {};
    }
    const StringId name = paSrcNameList.front();
    if (const TPortId id = getFBInterfaceSpec().getEIID(name); id != cgInvalidPortId) {
      return mInterface2InternalEventCons[id]->getDelegatingConnection(paSrcNameList.subspan(1));
    }
    if (const TPortId id = getFBInterfaceSpec().getDIID(name); id != cgInvalidPortId) {
      return getIf2InConUnchecked(id)->getDelegatingConnection(paSrcNameList.subspan(1));
    }
    if (const TPortId id = getFBInterfaceSpec().getDIOID(name); id != cgInvalidPortId) {
      return getDIOOutConInternalUnchecked(id)->getDelegatingConnection(paSrcNameList.subspan(1));
    }
    if (const auto plug = getPlugPin(name)) {
      return plug->getAdapterCon().getDelegatingConnection(paSrcNameList.subspan(1));
    }
    return {};
  }
} // namespace forte
