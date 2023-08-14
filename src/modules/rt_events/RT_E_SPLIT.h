/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
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

#include "ecet.h"

class FORTE_RT_E_SPLIT final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_RT_E_SPLIT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventEIID = 1;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventEO1ID = 1;
  static const TEventID scm_nEventEO2ID = 2;
  static const TDataIOID scm_anEOWith[];
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  CEventChainExecutionThread m_oECEO1, m_oECEO2;
  bool m_bInitialized;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_RT_E_SPLIT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_QI;
  CIEC_TIME var_Tmin;
  CIEC_TIME var_Deadline_EO1;
  CIEC_TIME var_WCET_EO1;
  CIEC_TIME var_Deadline_EO2;
  CIEC_TIME var_WCET_EO2;
  CIEC_BOOL var_QO;
  CIEC_BOOL var_conn_QO;
  CEventConnection conn_INITO;
  CEventConnection conn_EO1;
  CEventConnection conn_EO2;
  CDataConnection *conn_QI;
  CDataConnection *conn_Tmin;
  CDataConnection *conn_Deadline_EO1;
  CDataConnection *conn_WCET_EO1;
  CDataConnection *conn_Deadline_EO2;
  CDataConnection *conn_WCET_EO2;
  CDataConnection conn_QO;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;
  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline_EO1, const CIEC_TIME &pa_WCET_EO1, const CIEC_TIME &pa_Deadline_EO2, const CIEC_TIME &pa_WCET_EO2, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_Tmin = pa_Tmin;
    var_Deadline_EO1 = pa_Deadline_EO1;
    var_WCET_EO1 = pa_WCET_EO1;
    var_Deadline_EO2 = pa_Deadline_EO2;
    var_WCET_EO2 = pa_WCET_EO2;
    receiveInputEvent(scm_nEventINITID, nullptr);
    pa_QO = var_QO;
  }
  void evt_EI(const CIEC_BOOL &pa_QI, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline_EO1, const CIEC_TIME &pa_WCET_EO1, const CIEC_TIME &pa_Deadline_EO2, const CIEC_TIME &pa_WCET_EO2, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_Tmin = pa_Tmin;
    var_Deadline_EO1 = pa_Deadline_EO1;
    var_WCET_EO1 = pa_WCET_EO1;
    var_Deadline_EO2 = pa_Deadline_EO2;
    var_WCET_EO2 = pa_WCET_EO2;
    receiveInputEvent(scm_nEventEIID, nullptr);
    pa_QO = var_QO;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline_EO1, const CIEC_TIME &pa_WCET_EO1, const CIEC_TIME &pa_Deadline_EO2, const CIEC_TIME &pa_WCET_EO2, CIEC_BOOL &pa_QO) {
    evt_INIT(pa_QI, pa_Tmin, pa_Deadline_EO1, pa_WCET_EO1, pa_Deadline_EO2, pa_WCET_EO2, pa_QO);
  }
};



