/*******************************************************************************
 * Copyright (c) 2012 - 2024 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Michael Gafert - converted to new FB style
 *******************************************************************************/

#include "POWERLINK_MN_fbt.h"

USE_STRING_ID(APP_CFG);
USE_STRING_ID(BOOL);
USE_STRING_ID(CDC_CFG);
USE_STRING_ID(DEV_NAME);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(POWERLINK_MN);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);

DEFINE_FIRMWARE_FB(FORTE_POWERLINK_MN, STRID(POWERLINK_MN))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(CDC_CFG), STRID(APP_CFG), STRID(DEV_NAME)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
  const auto cEventInputNames = std::array{STRID(INIT)};
  const auto cEventInputTypeIds = std::array{STRID(EInit)};
  const auto cEventOutputNames = std::array{STRID(INITO)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

FORTE_POWERLINK_MN::FORTE_POWERLINK_MN(const CStringDictionary::TStringId paInstanceNameId,
                                       forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_INITO(*this, 0),
    conn_QI(nullptr),
    conn_CDC_CFG(nullptr),
    conn_APP_CFG(nullptr),
    conn_DEV_NAME(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_POWERLINK_MN::setInitialValues() {
  var_QI = 0_BOOL;
  var_CDC_CFG = ""_STRING;
  var_APP_CFG = ""_STRING;
  var_DEV_NAME = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_POWERLINK_MN::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:

      CEplStackWrapper *eplStack = &CEplStackWrapper::getInstance();

      eplStack->waitUntilOperational(false);
      eplStack->eplStackInit(var_APP_CFG.c_str(), var_CDC_CFG.c_str(), var_DEV_NAME.c_str());

      // TODO check if error occured during initialization

      sendOutputEvent(scmEventINITOID, paECET);

      break;
  }
}

void FORTE_POWERLINK_MN::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_CDC_CFG, conn_CDC_CFG);
      readData(2, var_APP_CFG, conn_APP_CFG);
      readData(3, var_DEV_NAME, conn_DEV_NAME);
      break;
    }
    default: break;
  }
}

void FORTE_POWERLINK_MN::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_POWERLINK_MN::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_CDC_CFG;
    case 2: return &var_APP_CFG;
    case 3: return &var_DEV_NAME;
  }
  return nullptr;
}

CIEC_ANY *FORTE_POWERLINK_MN::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_POWERLINK_MN::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
  }
  return nullptr;
}

CDataConnection **FORTE_POWERLINK_MN::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_CDC_CFG;
    case 2: return &conn_APP_CFG;
    case 3: return &conn_DEV_NAME;
  }
  return nullptr;
}

CDataConnection *FORTE_POWERLINK_MN::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
