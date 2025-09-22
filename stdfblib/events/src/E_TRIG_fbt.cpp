/*************************************************************************
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
  }

  void FORTE_E_TRIG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        const TEventTypeID eventTypeId = StringId::lookup(var_EVENTTYPE.c_str());
        if (eventTypeId) {
          triggerEventsInResource(getResource(), eventTypeId, paECET);
          sendOutputEvent(scmEventCNFID, paECET);
        }
        break;
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

  void FORTE_E_TRIG::triggerEventsInResource(CFBContainer *paContainer,
                                             const TEventTypeID paEventType,
                                             CEventChainExecutionThread *const paECET) {
    if (paContainer != nullptr) {
      if (paContainer->isFB()) {
        triggerEventsOfType(paEventType, static_cast<CFunctionBlock *>(paContainer), paECET);
      }
      if (paContainer->isDynamicContainer()) {
        for (auto child : paContainer->getChildren()) {
          triggerEventsInResource(child, paEventType, paECET);
        }
      }
    }
  }

  void FORTE_E_TRIG::triggerEventsOfType(TEventTypeID paEventTypeId,
                                         CFunctionBlock *paFb,
                                         CEventChainExecutionThread *const paECET) {
    const SFBInterfaceSpec &interfaceSpec = paFb->getFBInterfaceSpec();
    // most of the FBs will only have the basic event type -> mEITypes == nullptr
    if (interfaceSpec.mEITypeNames.empty()) {
      return;
    }
    for (TEventID eventId = 0; eventId < interfaceSpec.getNumEIs(); eventId++) {
      if (interfaceSpec.getEIType(eventId) == paEventTypeId && !paFb->isInputEventConnected(eventId)) {
        paECET->addEventEntry(TEventEntry(*paFb, eventId));
      }
    }
  }

} // namespace forte::iec61499::events
