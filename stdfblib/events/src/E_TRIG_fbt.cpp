/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Mario Kastner - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - reworked to not overload the event queue
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TRIG
 *** Description: Trigger unconnected input events of the specified type inside a resource
 *** Version:
 ***     1.0: 2025-01-08/Mario Kastner -  -
 *************************************************************************/

#include "forte/iec61499/events/E_TRIG_fbt.h"
#include "forte/event.h"
#include "forte/util/devlog.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

#include "forte/resource.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"EVENTTYPE"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TRIG, "iec61499::events::E_TRIG"_STRID)

  FORTE_E_TRIG::FORTE_E_TRIG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_EVENTTYPE("EInit"_STRING),
      conn_CNF(*this, 0),
      conn_EVENTTYPE(nullptr) {};

  void FORTE_E_TRIG::setInitialValues() {
    var_EVENTTYPE = "EInit"_STRING;
    mCursor = CTriggerEventCursor();
  }

  void FORTE_E_TRIG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: handleREQ(paECET); break;
      case cgExternalEventID: handleOneTriggerEvent(paECET); break;
      default: break;
    }
  }

  void FORTE_E_TRIG::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_EVENTTYPE, conn_EVENTTYPE);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_TRIG::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_TRIG::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_EVENTTYPE;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TRIG::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_TRIG::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_TRIG::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EVENTTYPE;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_TRIG::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  void FORTE_E_TRIG::handleREQ(CEventChainExecutionThread *const paECET) {
    if (mCursor.isActive()) {
      // we are still processing the previous request, ignore it.
      DEVLOG_WARNING("E_TRIG received REQ while still processing previous REQ. REQ event dropped!\n");
      return;
    }

    const TEventTypeID eventTypeId = StringId::lookup(var_EVENTTYPE.c_str());
    if (eventTypeId) {
      mCursor = CTriggerEventCursor(getResource(), eventTypeId);
      // process the one entry of the list or send CNF to indicate that we are done
      handleOneTriggerEvent(paECET);
    }
  }

  void FORTE_E_TRIG::handleOneTriggerEvent(CEventChainExecutionThread *const paECET) {
    if (auto entry = mCursor.next()) {
      entry->getFB().receiveInputEvent(entry->getPortId(), paECET);
      // we may not be done yet, so come back and ask the cursor again
      paECET->addEventEntry(TEventEntry(*this, cgExternalEventID));
    } else {
      // cursor exhausted: inform about completion
      mCursor = CTriggerEventCursor();
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  FORTE_E_TRIG::CTriggerEventCursor::CTriggerEventCursor(CFBContainer *paRoot, TEventTypeID paEventType) :
      mEventType(paEventType) {
    enter(paRoot);
  }

  std::optional<TEventEntry> FORTE_E_TRIG::CTriggerEventCursor::next() {
    while (true) {
      if (mFb != nullptr) {
        const SFBInterfaceSpec &spec = mFb->getFBInterfaceSpec();
        while (mEventId < spec.getNumEIs()) {
          const TEventID id = mEventId++;
          if (spec.getEIType(id) == mEventType && !mFb->isInputEventConnected(id)) {
            return TEventEntry{*mFb, id};
          }
        }
        mFb = nullptr;
      }
      if (mStack.empty()) {
        return std::nullopt;
      }
      std::span<CFBContainer *const> &remaining = mStack.back();
      if (remaining.empty()) {
        mStack.pop_back();
        continue;
      }
      CFBContainer *child = remaining.front();
      remaining = remaining.subspan(1);
      enter(child);
    }
  }

  void FORTE_E_TRIG::CTriggerEventCursor::enter(CFBContainer *paContainer) {
    if (paContainer == nullptr) {
      return;
    }
    if (paContainer->isFB()) {
      auto *fb = static_cast<CFunctionBlock *>(paContainer);
      if (!fb->getFBInterfaceSpec().mEITypeNames.empty()) {
        mFb = fb;
        mEventId = 0;
      }
    }
    if (paContainer->isDynamicContainer()) {
      mStack.emplace_back(paContainer->getChildren());
    }
  }
} // namespace forte::iec61499::events
