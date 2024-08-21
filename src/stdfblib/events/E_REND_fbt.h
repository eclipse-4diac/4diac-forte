/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "basicfb.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_REND final : public CBasicFB {
  DECLARE_FIRMWARE_FB(FORTE_E_REND)

private:
  static const TEventID scmEventEI1ID = 0;
  static const TEventID scmEventEI2ID = 1;
  static const TEventID scmEventRID = 2;
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];
  static const TEventID scmEventEOID = 0;
  static const TForteInt16 scmEOWithIndexes[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  CIEC_ANY *getVarInternal(size_t) override;

  static const TForteInt16 scmStateSTART = 0;
  static const TForteInt16 scmStateEI1 = 1;
  static const TForteInt16 scmStateEO = 2;
  static const TForteInt16 scmStateEI2 = 3;

  void enterStateSTART(CEventChainExecutionThread *const paECET);
  void enterStateEI1(CEventChainExecutionThread *const paECET);
  void enterStateEO(CEventChainExecutionThread *const paECET);
  void enterStateEI2(CEventChainExecutionThread *const paECET);

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;

public:
  FORTE_E_REND(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

  CEventConnection conn_EO;

  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;

  void evt_EI1() {
    receiveInputEvent(scmEventEI1ID, nullptr);
  }

  void evt_EI2() {
    receiveInputEvent(scmEventEI2ID, nullptr);
  }

  void evt_R() {
    receiveInputEvent(scmEventRID, nullptr);
  }

  void operator()() {
    evt_EI1();
  }
};


