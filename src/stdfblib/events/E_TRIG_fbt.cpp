/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TRIG
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2025-01-08/mario -  -
 *************************************************************************/

#include "E_TRIG_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(E_TRIG);
USE_STRING_ID(EVENTTYPE);
USE_STRING_ID(REQ);
USE_STRING_ID(STRING);

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_TRIG, STRID(E_TRIG))

namespace {
  const auto cDataInputNames = std::array{STRID(EVENTTYPE)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
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

FORTE_E_TRIG::FORTE_E_TRIG(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_EVENTTYPE(""_STRING),
    conn_CNF(*this, 0),
    conn_EVENTTYPE(nullptr) {};

void FORTE_E_TRIG::setInitialValues() {
  var_EVENTTYPE = ""_STRING;
}

void FORTE_E_TRIG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      const TEventTypeID eventTypeId = CStringDictionary::getId(var_EVENTTYPE.c_str());
      if (eventTypeId != CStringDictionary::scmInvalidStringId) {
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

void FORTE_E_TRIG::triggerEventsInResource(forte::core::CFBContainer *paContainer,
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
