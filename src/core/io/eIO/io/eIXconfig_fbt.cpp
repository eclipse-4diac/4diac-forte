/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "eIXconfig_fbt.h"

#include "eGenAdapter_adp.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CONF);
USE_STRING_ID(Event);
USE_STRING_ID(FE);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RE);
USE_STRING_ID(STATUS);
USE_STRING_ID(WSTRING);
USE_STRING_ID(eGenAdapter);
USE_STRING_ID(eIX);
USE_STRING_ID(eIXconfig);

DEFINE_FIRMWARE_FB(FORTE_eIXconfig, STRID(eIXconfig))

const CStringDictionary::TStringId FORTE_eIXconfig::scmDataInputNames[] = {STRID(QI), STRID(FE), STRID(RE)};
const CStringDictionary::TStringId FORTE_eIXconfig::scmDataInputTypeIds[] = {STRID(BOOL), STRID(BOOL), STRID(BOOL)};
const CStringDictionary::TStringId FORTE_eIXconfig::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_eIXconfig::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_eIXconfig::scmEIWith[] = {0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_eIXconfig::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_eIXconfig::scmEventInputNames[] = {STRID(CONF)};
const TDataIOID FORTE_eIXconfig::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_eIXconfig::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_eIXconfig::scmEventOutputNames[] = {STRID(CNF)};
const SAdapterInstanceDef FORTE_eIXconfig::scmAdapterInstances[] = {
  {STRID(eGenAdapter), STRID(eIX), false}
};
const SFBInterfaceSpec FORTE_eIXconfig::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  1, scmAdapterInstances
};

FORTE_eIXconfig::FORTE_eIXconfig(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    EConfigFB(paInstanceNameId, scmFBInterfaceSpec, paContainer),
    var_QI(0_BOOL),
    var_FE(0_BOOL),
    var_RE(0_BOOL),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_eIX(STRID(eIX), *this, false),
    conn_CNF(*this, 0),
    conn_QI(nullptr),
    conn_FE(nullptr),
    conn_RE(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {
};

bool FORTE_eIXconfig::initialize() {
  if(!var_eIX.initialize()) { return false; }
  var_eIX.setParentFB(this, 0);
  return CFunctionBlock::initialize();
}

void FORTE_eIXconfig::setInitialValues() {
  var_QI = 0_BOOL;
  var_FE = 0_BOOL;
  var_RE = 0_BOOL;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_eIXconfig::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventCONFID:
      eventGen();
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_eIXconfig::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCONFID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_FE, conn_FE);
      readData(2, var_RE, conn_RE);
      break;
    }
    default:
      break;
  }
}

void FORTE_eIXconfig::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_eIXconfig::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_FE;
    case 2: return &var_RE;
  }
  return nullptr;
}

CIEC_ANY *FORTE_eIXconfig::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CAdapter *FORTE_eIXconfig::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_eIX;
  }
  return nullptr;
}

CEventConnection *FORTE_eIXconfig::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_eIXconfig::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_FE;
    case 2: return &conn_RE;
  }
  return nullptr;
}

CDataConnection *FORTE_eIXconfig::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}


/* MODIFIED */
std::forward_list<ESpecBase*> *FORTE_eIXconfig::eventGen() {
  DEVLOG_DEBUG("[eIXconfig] eventGen\r\n");
    // deregister every already registered evert-trigger condition of this FB
  deregisterFBsEventTrigger();

  if (!var_QI)
    return &eventGenList;

  auto *eIX = static_cast<FORTE_eIX * >(getIOPeer(this->getAdapterUnchecked(scmeIXAdpNum)));

  if (eIX == nullptr) {
    DEVLOG_ERROR("[eIXconfig::evenGen] IO instance is nullptr.\r\n");
    return &eventGenList;
  }


  /* REGISTER eTRIGGERS =================================================================== BEGIN === */
  // register rising edge event-trigger
  if (var_RE) {
    DEVLOG_DEBUG("[eventGen] register eRE event\r\n");
    auto *newEvent = new EIO_RisingEdge(eIX, eIX->getHandle());
    registerEventTrigger(newEvent);
  }

  // register falling edge event-trigger
  if (var_FE) {
    DEVLOG_DEBUG("[eventGen] register eFE event\r\n");
    auto *newEvent = new EIO_FallingEdge(eIX, eIX->getHandle());
    registerEventTrigger(newEvent);
  }
  /* REGISTER eTRIGGERS ===================================================================  END  === */

  return &eventGenList;
}
