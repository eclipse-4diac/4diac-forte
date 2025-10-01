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
    createEventConnections();
    createDataConnections();
    createAdapterConnections();
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
    for (auto &currentConn : cmFBNData.mDataConnections) {
      if (!currentConn.mSrcFBNameId && currentConn.mSrcId == ifSrcId) {
        CFunctionBlock *dstFB = getFunctionBlock(currentConn.mDstFBNameId);
        if (!dstFB || !forwardGenericDI(*dstFB, currentConn.mDstId, paRefValue)) {
          return false;
        };
      }
    }
    return CFunctionBlock::configureGenericDI(paDIPortId, paRefValue);
  }

  bool CCompositeFB::configureGenericDIO(const TPortId paDIOPortId, const CIEC_ANY &paRefValue) {
    const StringId ifSrcId = getFBInterfaceSpec().mDIONames[paDIOPortId];
    for (auto &currentConn : cmFBNData.mDataConnections) {
      if (!currentConn.mSrcFBNameId && currentConn.mSrcId == ifSrcId) {
        CFunctionBlock *dstFB = getFunctionBlock(currentConn.mDstFBNameId);
        if (!dstFB || (!forwardGenericDI(*dstFB, currentConn.mDstId, paRefValue) &&
                       !forwardGenericDIO(*dstFB, currentConn.mDstId, paRefValue))) {
          return false;
        };
      }
    }
    return CFunctionBlock::configureGenericDIO(paDIOPortId, paRefValue);
  }

  bool CCompositeFB::configureGenericDO(const TPortId paDOPortId, const CIEC_ANY &paRefValue) {
    const StringId ifDstId = getFBInterfaceSpec().mDONames[paDOPortId];
    for (auto &currentConn : cmFBNData.mDataConnections) {
      if (!currentConn.mDstFBNameId && currentConn.mDstId == ifDstId) {
        CFunctionBlock *srcFB = getFunctionBlock(currentConn.mSrcFBNameId);
        if (!srcFB || !forwardGenericDO(*srcFB, currentConn.mSrcId, paRefValue)) {
          return false;
        };
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

  void CCompositeFB::createEventConnections() {
    prepareIf2InEventCons(); // the interface to internal event connections are needed even if they are not connected
                             // therefore we have to create them correctly in any case

    for (auto &currentConn : cmFBNData.mEventConnections) {
      CFunctionBlock *srcFB = getFunctionBlock(currentConn.mSrcFBNameId);
      CFunctionBlock *dstFB = getFunctionBlock(currentConn.mDstFBNameId);

      if ((nullptr != srcFB) && (nullptr != dstFB)) {
        CEventConnection *evConn =
            (this == srcFB) ? mInterface2InternalEventCons[getFBInterfaceSpec().getEIID(currentConn.mSrcId)].get()
                            : srcFB->getEOConnection(currentConn.mSrcId);
        establishConnection(evConn, *dstFB, std::array{currentConn.mDstId});
      } else {
        // FIXME implement way to inform FB creator that creation failed
        DEVLOG_ERROR("Could not create event connection in CFB");
      }
    }
  }

  void CCompositeFB::prepareIf2InEventCons() {
    mInterface2InternalEventCons.reserve(getFBInterfaceSpec().getNumEIs());
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumEIs(); i++) {
      mInterface2InternalEventCons.emplace_back(std::make_unique<CEventConnection>(*this, i));
    }
  }

  void CCompositeFB::establishConnection(CConnection *paCon,
                                         CFunctionBlock &paDstFb,
                                         const std::span<const StringId> paDstNameId) {
    if (this == &paDstFb) {
      paCon->connectToCFBInterface(paDstFb, paDstNameId);
    } else {
      paCon->connect(paDstFb, paDstNameId);
    }
  }

  void CCompositeFB::createDataConnections() {
    for (auto &currentConn : cmFBNData.mDataConnections) {
      // FIXME implement way to inform FB creator that creation failed
      CFunctionBlock *srcFB = getFunctionBlock(currentConn.mSrcFBNameId);
      CFunctionBlock *dstFB = getFunctionBlock(currentConn.mDstFBNameId);

      if ((srcFB != nullptr) && (dstFB != nullptr)) {
        establishConnection(getDataConn(srcFB, currentConn.mSrcId), *dstFB, std::array{currentConn.mDstId});
        if (srcFB == this) {
          // Data connections track on the source side the number of destinations. For interface to internals we don't
          // want that. Therefore, we have to revert that ref count change.
          decConnRefCount();
        }
      } else {
        DEVLOG_ERROR("Could not create data connection in CFB");
      }
    }
  }

  CDataConnection *CCompositeFB::getDataConn(CFunctionBlock *paSrcFB, StringId paSrcNameId) {
    if (this == paSrcFB) {
      TPortId diId = getFBInterfaceSpec().getDIID(paSrcNameId);
      if (diId != cgInvalidPortId) {
        return getIf2InConUnchecked(diId);
      } else {
        TPortId dioId = getFBInterfaceSpec().getDIOID(paSrcNameId);
        return getDIOOutConInternalUnchecked(dioId);
      }
    }

    CDataConnection *con = paSrcFB->getDOConnection(paSrcNameId);
    if (con == nullptr) {
      con = paSrcFB->getDIOOutConnection(paSrcNameId);
    }
    return con;
  }

  void CCompositeFB::createAdapterConnections() {
    for (auto &currentConn : cmFBNData.mAdapterConnections) {
      // FIXME implement way to inform FB creator that creation failed
      CFunctionBlock *const srcFB = getFunctionBlock(currentConn.mSrcFBNameId);
      CFunctionBlock *const dstFB = getFunctionBlock(currentConn.mDstFBNameId);

      if ((nullptr != srcFB) && (nullptr != dstFB)) {
        if (auto plug = srcFB->getPlugPin(currentConn.mSrcId); plug != nullptr) {
          plug->getAdapterCon().connect(*dstFB, std::array{currentConn.mDstId});
        } else {
          DEVLOG_ERROR("[CFB Creation] Adapter source is not a plug!");
        }
      } else {
        DEVLOG_ERROR("[CFB Creation] Source or destination not found in adapter connection!");
      }
    }
  }

  void CCompositeFB::setFBNetworkInitialValues() {
    // per default we do not have initial values of internal blocks
  }

  CFunctionBlock *CCompositeFB::getFunctionBlock(StringId paFBNameId) {
    if (!paFBNameId) {
      return this;
    }
    return getFB(paFBNameId);
  }
} // namespace forte
