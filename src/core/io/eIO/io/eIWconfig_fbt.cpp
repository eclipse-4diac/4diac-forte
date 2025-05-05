/*****************EConfigFB**************************************************************
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

#include "eIWconfig_fbt.h"

#include "eGenAdapter_adp.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BT);
USE_STRING_ID(CNF);
USE_STRING_ID(CONF);
USE_STRING_ID(Event);
USE_STRING_ID(GRAD);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(ST);
USE_STRING_ID(STATUS);
USE_STRING_ID(WORD);
USE_STRING_ID(WSTRING);
USE_STRING_ID(eGenAdapter);
USE_STRING_ID(eIW);
USE_STRING_ID(eIWconfig);

DEFINE_FIRMWARE_FB(FORTE_eIWconfig, STRID(eIWconfig))

const CStringDictionary::TStringId FORTE_eIWconfig::scmDataInputNames[] = {STRID(QI), STRID(ST), STRID(BT), STRID(GRAD)};
const CStringDictionary::TStringId FORTE_eIWconfig::scmDataInputTypeIds[] = {STRID(BOOL), STRID(WORD), STRID(WORD), STRID(WORD)};
const CStringDictionary::TStringId FORTE_eIWconfig::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_eIWconfig::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_eIWconfig::scmEIWith[] = {0, 1, 2, 3, scmWithListDelimiter};
const TForteInt16 FORTE_eIWconfig::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_eIWconfig::scmEventInputNames[] = {STRID(CONF)};
const TDataIOID FORTE_eIWconfig::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_eIWconfig::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_eIWconfig::scmEventOutputNames[] = {STRID(CNF)};
const SAdapterInstanceDef FORTE_eIWconfig::scmAdapterInstances[] = {
  {STRID(eGenAdapter), STRID(eIW), false}
};
const SFBInterfaceSpec FORTE_eIWconfig::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  4, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  1, scmAdapterInstances
};

FORTE_eIWconfig::FORTE_eIWconfig(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    EConfigFB(paInstanceNameId, scmFBInterfaceSpec, paContainer),
    var_QI(0_BOOL),
    var_ST(0_WORD),
    var_BT(0_WORD),
    var_GRAD(0_WORD),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_eIW(STRID(eIW), *this, false),
    conn_CNF(*this, 0),
    conn_QI(nullptr),
    conn_ST(nullptr),
    conn_BT(nullptr),
    conn_GRAD(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {
};

bool FORTE_eIWconfig::initialize() {
  if(!var_eIW.initialize()) { return false; }
  var_eIW.setParentFB(this, 0);
  return CFunctionBlock::initialize();
}

void FORTE_eIWconfig::setInitialValues() {
  var_QI = 0_BOOL;
  var_ST = 0_WORD;
  var_BT = 0_WORD;
  var_GRAD = 0_WORD;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_eIWconfig::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventCONFID:
      eventGen();
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_eIWconfig::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCONFID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_ST, conn_ST);
      readData(2, var_BT, conn_BT);
      readData(3, var_GRAD, conn_GRAD);
      break;
    }
    default:
      break;
  }
}

void FORTE_eIWconfig::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_eIWconfig::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_ST;
    case 2: return &var_BT;
    case 3: return &var_GRAD;
  }
  return nullptr;
}

CIEC_ANY *FORTE_eIWconfig::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CAdapter *FORTE_eIWconfig::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_eIW;
  }
  return nullptr;
}

CEventConnection *FORTE_eIWconfig::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_eIWconfig::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_ST;
    case 2: return &conn_BT;
    case 3: return &conn_GRAD;
  }
  return nullptr;
}

CDataConnection *FORTE_eIWconfig::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

std::forward_list<ESpecBase*> *FORTE_eIWconfig::eventGen() {
  DEVLOG_DEBUG("[eIWconfig] eventGen\r\n");

  // deregister every already registered evert-trigger condition of this FB
  deregisterFBsEventTrigger();

  if (!var_QI)
    return &eventGenList;

  /* connecting to adapter peer */
  CAdapter* peerAdapter = nullptr;
  FORTE_eGenAdapter* eGenAdapter = nullptr;
  FORTE_eIW* eIW = nullptr;

  /* CONNECTING TO PEER =================================================================== BEGIN === */
  if ((peerAdapter = getAdapterUnchecked(scmeIWAdpNum)->getPeer()) != nullptr){
    eGenAdapter = static_cast<FORTE_eGenAdapter*>(peerAdapter);
    eIW = static_cast<FORTE_eIW*>(eGenAdapter->parentFB);
  } else {
    DEVLOG_ERROR("[eIWconfig] Could not reach eIW FB via adapter!\r\n");
    return &eventGenList;
  }

  if (eIW == nullptr) {
    DEVLOG_ERROR("[eIWconfig] eIW is nullptr\r\n");
    return &eventGenList;
  }
  /* CONNECTING TO PEER ===================================================================  END  === */

  /* REGISTER eTRIGGERS =================================================================== BEGIN === */
  // deregister every already registered evert-trigger condition of this FB
  deregisterFBsEventTrigger();

  ESpecBase* newEvent;

  // register bounded area event-trigger
  if (var_ST != 0 && var_BT  != 0) {
    DEVLOG_DEBUG("[eventGen] register eBA event\r\n");
    newEvent = new EIO_BoundedArea(eIW, eIW->getHandle(), var_BT, var_ST);
    registerEventTrigger(newEvent);
  } else {
    // register upper-threshold event-trigger
    if (var_ST != 0) {
      DEVLOG_DEBUG("[eventGen] register eUT event\r\n");
      newEvent = new EIO_UpperThreshold(eIW, eIW->getHandle(), var_ST);
      registerEventTrigger(newEvent);
    }
    // register bottum/lower-threshold event-trigger
    if (var_BT != 0) {
      DEVLOG_DEBUG("[eventGen] register eGR event\r\n");
      newEvent = new EIO_LowerThreshold(eIW, eIW->getHandle(), var_BT);
      registerEventTrigger(newEvent);
    }
  }
  // register gradient event-trigger
  if (var_GRAD != 0) {
    DEVLOG_DEBUG("[eventGen] register eGR event\r\n");
    newEvent = new EIO_Gradient(eIW, eIW->getHandle(), var_GRAD);
    registerEventTrigger(newEvent);
  }
  /* REGISTER eTRIGGERS ===================================================================  END  === */

  return &eventGenList;
}