/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 *               2020 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ben Schneider - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - Changed to a full basic FB implementation utilizing the new
 *                   NOW_MONOTONIC function
 *******************************************************************************/

#pragma once

#include "basicfb.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_STOPWATCH final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_STOPWATCH)

private:
  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  static const TEventID scmEventSTARTID = 0;
  static const TEventID scmEventSTOPID = 1;
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];
  static const TEventID scmEventEOID = 0;
  static const TDataIOID scmEOWith[];
  static const TForteInt16 scmEOWithIndexes[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  static const CStringDictionary::TStringId scmInternalsNames[];
  static const CStringDictionary::TStringId scmInternalsTypeIds[];
  static const SInternalVarsInformation scmInternalVars;

  CIEC_TIME var_startTime;

  CIEC_ANY *getVarInternal(size_t) override;

  void alg_captureStartTime(void);
  void alg_calcDiff(void);

  static const TForteInt16 scmStateSTART = 0;
  static const TForteInt16 scmStateMeasure = 1;
  static const TForteInt16 scmStateSTOP = 2;

  void enterStateSTART(CEventChainExecutionThread *const paECET);
  void enterStateMeasure(CEventChainExecutionThread *const paECET);
  void enterStateSTOP(CEventChainExecutionThread *const paECET);

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_E_STOPWATCH(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

  CIEC_TIME var_TD;

  CIEC_TIME var_conn_TD;

  CEventConnection conn_EO;

  CDataConnection conn_TD;

  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;

  void evt_START(CIEC_TIME &paTD) {
    receiveInputEvent(scmEventSTARTID, nullptr);
    paTD = var_TD;
  }

  void evt_STOP(CIEC_TIME &paTD) {
    receiveInputEvent(scmEventSTOPID, nullptr);
    paTD = var_TD;
  }

  void operator()(CIEC_TIME &paTD) {
    evt_START(paTD);
  }
};


