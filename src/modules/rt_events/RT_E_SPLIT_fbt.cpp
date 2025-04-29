/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Monika Wenger,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RT_E_SPLIT_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(Deadline_EO1);
USE_STRING_ID(Deadline_EO2);
USE_STRING_ID(EI);
USE_STRING_ID(EInit);
USE_STRING_ID(EO1);
USE_STRING_ID(EO2);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RT_E_SPLIT);
USE_STRING_ID(TIME);
USE_STRING_ID(Tmin);
USE_STRING_ID(WCET_EO1);
USE_STRING_ID(WCET_EO2);


DEFINE_FIRMWARE_FB(FORTE_RT_E_SPLIT, STRID(RT_E_SPLIT))

const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmDataInputNames[] = {
    STRID(QI), STRID(Tmin), STRID(Deadline_EO1), STRID(WCET_EO1), STRID(Deadline_EO2), STRID(WCET_EO2)};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME), STRID(TIME),
                                                                              STRID(TIME), STRID(TIME), STRID(TIME)};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmDataOutputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_RT_E_SPLIT::scmEIWith[] = {0, 1, 2, 3, 4, 5, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_SPLIT::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmEventInputNames[] = {STRID(INIT), STRID(EI)};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_RT_E_SPLIT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_SPLIT::scmEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmEventOutputNames[] = {STRID(INITO), STRID(EO1), STRID(EO2)};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event),
                                                                                STRID(Event)};
const SFBInterfaceSpec FORTE_RT_E_SPLIT::scmFBInterfaceSpec = {2,
                                                               scmEventInputNames,
                                                               scmEventInputTypeIds,
                                                               scmEIWith,
                                                               scmEIWithIndexes,
                                                               3,
                                                               scmEventOutputNames,
                                                               scmEventOutputTypeIds,
                                                               scmEOWith,
                                                               scmEOWithIndexes,
                                                               6,
                                                               scmDataInputNames,
                                                               scmDataInputTypeIds,
                                                               1,
                                                               scmDataOutputNames,
                                                               scmDataOutputTypeIds,
                                                               0,
                                                               nullptr,
                                                               0,
                                                               nullptr};

FORTE_RT_E_SPLIT::FORTE_RT_E_SPLIT(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_INITO(*this, 0),
    conn_EO1(*this, 1),
    conn_EO2(*this, 2),
    conn_QI(nullptr),
    conn_Tmin(nullptr),
    conn_Deadline_EO1(nullptr),
    conn_WCET_EO1(nullptr),
    conn_Deadline_EO2(nullptr),
    conn_WCET_EO2(nullptr),
    conn_QO(*this, 0, var_QO) {};

void FORTE_RT_E_SPLIT::setInitialValues() {
  var_QI = 0_BOOL;
  var_Tmin = 0_TIME;
  var_Deadline_EO1 = 0_TIME;
  var_WCET_EO1 = 0_TIME;
  var_Deadline_EO2 = 0_TIME;
  var_WCET_EO2 = 0_TIME;
  var_QO = 0_BOOL;
}

void FORTE_RT_E_SPLIT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventEIID:
      if (mInitialized) {
        CEventConnection *eoCon;
        eoCon = getEOConUnchecked(scmEventEO1ID);
        if (eoCon->isConnected()) {
          eoCon->triggerEvent(&mECEO1);
          mECEO1.resumeSelfSuspend();
        }
        eoCon = getEOConUnchecked(scmEventEO2ID);
        if (eoCon->isConnected()) {
          eoCon->triggerEvent(&mECEO2);
          mECEO2.resumeSelfSuspend();
        }
      }
      break;
    case scmEventINITID:
      if (var_QI == true) {
        if (!mInitialized) {
          mECEO1.changeExecutionState(EMGMCommandType::Start);
          mECEO2.changeExecutionState(EMGMCommandType::Start);
          mInitialized = true;
        }
        mECEO1.setDeadline(var_Deadline_EO1);
        mECEO2.setDeadline(var_Deadline_EO2);
      } else {
        mECEO1.changeExecutionState(EMGMCommandType::Stop);
        mECEO2.changeExecutionState(EMGMCommandType::Stop);
        mInitialized = false;
      }
      var_QO = var_QI;
      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_RT_E_SPLIT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_Tmin, conn_Tmin);
      readData(2, var_Deadline_EO1, conn_Deadline_EO1);
      readData(3, var_WCET_EO1, conn_WCET_EO1);
      readData(4, var_Deadline_EO2, conn_Deadline_EO2);
      readData(5, var_WCET_EO2, conn_WCET_EO2);
      break;
    }
    case scmEventEIID: {
      break;
    }
    default: break;
  }
}

void FORTE_RT_E_SPLIT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventEO1ID: {
      break;
    }
    case scmEventEO2ID: {
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_RT_E_SPLIT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Tmin;
    case 2: return &var_Deadline_EO1;
    case 3: return &var_WCET_EO1;
    case 4: return &var_Deadline_EO2;
    case 5: return &var_WCET_EO2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_SPLIT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_RT_E_SPLIT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_EO1;
    case 2: return &conn_EO2;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_SPLIT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Tmin;
    case 2: return &conn_Deadline_EO1;
    case 3: return &conn_WCET_EO1;
    case 4: return &conn_Deadline_EO2;
    case 5: return &conn_WCET_EO2;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_SPLIT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}
