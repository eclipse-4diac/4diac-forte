/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_FB_TP final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_FB_TP)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventREQID = 0;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventCNFID = 0;
  static const TDataIOID scm_anEOWith[];
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  bool edgeFlag;
  CIEC_TIME start;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_FB_TP(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_IN;
  CIEC_TIME var_PT;
  CIEC_BOOL var_Q;
  CIEC_TIME var_ET;
  CIEC_BOOL var_conn_Q;
  CIEC_TIME var_conn_ET;
  CEventConnection conn_CNF;
  CDataConnection *conn_IN;
  CDataConnection *conn_PT;
  CDataConnection conn_Q;
  CDataConnection conn_ET;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;
  void evt_REQ(const CIEC_BOOL &pa_IN, const CIEC_TIME &pa_PT, CIEC_BOOL &pa_Q, CIEC_TIME &pa_ET) {
    var_IN = pa_IN;
    var_PT = pa_PT;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_Q = var_Q;
    pa_ET = var_ET;
  }
  void operator()(const CIEC_BOOL &pa_IN, const CIEC_TIME &pa_PT, CIEC_BOOL &pa_Q, CIEC_TIME &pa_ET) {
    evt_REQ(pa_IN, pa_PT, pa_Q, pa_ET);
  }
};



