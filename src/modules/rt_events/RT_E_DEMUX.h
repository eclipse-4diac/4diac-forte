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
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_uint.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "ecet.h"

class FORTE_RT_E_DEMUX final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_RT_E_DEMUX)

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
  static const TEventID scm_nEventEO0ID = 1;
  static const TEventID scm_nEventEO1ID = 2;
  static const TEventID scm_nEventEO2ID = 3;
  static const TEventID scm_nEventEO3ID = 4;
  static const TDataIOID scm_anEOWith[];
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  CEventChainExecutionThread m_oECEO0;
  CEventChainExecutionThread m_oECEO1;
  CEventChainExecutionThread m_oECEO2;
  CEventChainExecutionThread m_oECEO3;
  bool m_bInitialized;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_RT_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_QI;
  CIEC_UINT var_K;
  CIEC_TIME var_Tmin;
  CIEC_TIME var_Deadline0;
  CIEC_TIME var_WCET0;
  CIEC_TIME var_Deadline1;
  CIEC_TIME var_WCET1;
  CIEC_TIME var_Deadline2;
  CIEC_TIME var_WCET2;
  CIEC_TIME var_Deadline3;
  CIEC_TIME var_WCET3;
  CIEC_BOOL var_QO;
  CIEC_BOOL var_conn_QO;
  CEventConnection conn_INITO;
  CEventConnection conn_EO0;
  CEventConnection conn_EO1;
  CEventConnection conn_EO2;
  CEventConnection conn_EO3;
  CDataConnection *conn_QI;
  CDataConnection *conn_K;
  CDataConnection *conn_Tmin;
  CDataConnection *conn_Deadline0;
  CDataConnection *conn_WCET0;
  CDataConnection *conn_Deadline1;
  CDataConnection *conn_WCET1;
  CDataConnection *conn_Deadline2;
  CDataConnection *conn_WCET2;
  CDataConnection *conn_Deadline3;
  CDataConnection *conn_WCET3;
  CDataConnection conn_QO;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;
  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_UINT &pa_K, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline0, const CIEC_TIME &pa_WCET0, const CIEC_TIME &pa_Deadline1, const CIEC_TIME &pa_WCET1, const CIEC_TIME &pa_Deadline2, const CIEC_TIME &pa_WCET2, const CIEC_TIME &pa_Deadline3, const CIEC_TIME &pa_WCET3, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_K = pa_K;
    var_Tmin = pa_Tmin;
    var_Deadline0 = pa_Deadline0;
    var_WCET0 = pa_WCET0;
    var_Deadline1 = pa_Deadline1;
    var_WCET1 = pa_WCET1;
    var_Deadline2 = pa_Deadline2;
    var_WCET2 = pa_WCET2;
    var_Deadline3 = pa_Deadline3;
    var_WCET3 = pa_WCET3;
    receiveInputEvent(scm_nEventINITID, nullptr);
    pa_QO = var_QO;
  }
  void evt_EI(const CIEC_BOOL &pa_QI, const CIEC_UINT &pa_K, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline0, const CIEC_TIME &pa_WCET0, const CIEC_TIME &pa_Deadline1, const CIEC_TIME &pa_WCET1, const CIEC_TIME &pa_Deadline2, const CIEC_TIME &pa_WCET2, const CIEC_TIME &pa_Deadline3, const CIEC_TIME &pa_WCET3, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_K = pa_K;
    var_Tmin = pa_Tmin;
    var_Deadline0 = pa_Deadline0;
    var_WCET0 = pa_WCET0;
    var_Deadline1 = pa_Deadline1;
    var_WCET1 = pa_WCET1;
    var_Deadline2 = pa_Deadline2;
    var_WCET2 = pa_WCET2;
    var_Deadline3 = pa_Deadline3;
    var_WCET3 = pa_WCET3;
    receiveInputEvent(scm_nEventEIID, nullptr);
    pa_QO = var_QO;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_UINT &pa_K, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline0, const CIEC_TIME &pa_WCET0, const CIEC_TIME &pa_Deadline1, const CIEC_TIME &pa_WCET1, const CIEC_TIME &pa_Deadline2, const CIEC_TIME &pa_WCET2, const CIEC_TIME &pa_Deadline3, const CIEC_TIME &pa_WCET3, CIEC_BOOL &pa_QO) {
    evt_INIT(pa_QI, pa_K, pa_Tmin, pa_Deadline0, pa_WCET0, pa_Deadline1, pa_WCET1, pa_Deadline2, pa_WCET2, pa_Deadline3, pa_WCET3, pa_QO);
  }
};



