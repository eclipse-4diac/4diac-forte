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

#include "rtesingle.h"

class FORTE_RT_E_SELECT final : public CRTEventSingle {
  DECLARE_FIRMWARE_FB(FORTE_RT_E_SELECT)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventEI0ID = 1;
  static const TEventID scm_nEventEI1ID = 2;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventEOID = 1;
  static const TDataIOID scm_anEOWith[];
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  bool checkActivation(TEventID pa_nEIID) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_RT_E_SELECT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_G;
  CIEC_TIME var_Tmin;
  CIEC_TIME var_WCET;
  CIEC_BOOL var_conn_QO;
  CEventConnection conn_INITO;
  CEventConnection conn_EO;
  CDataConnection *conn_QI;
  CDataConnection *conn_G;
  CDataConnection *conn_Tmin;
  CDataConnection *conn_Deadline;
  CDataConnection *conn_WCET;
  CDataConnection conn_QO;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;
  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_BOOL &pa_G, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline, const CIEC_TIME &pa_WCET, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_G = pa_G;
    var_Tmin = pa_Tmin;
    var_Deadline = pa_Deadline;
    var_WCET = pa_WCET;
    receiveInputEvent(scm_nEventINITID, nullptr);
    pa_QO = var_QO;
  }
  void evt_EI0(const CIEC_BOOL &pa_QI, const CIEC_BOOL &pa_G, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline, const CIEC_TIME &pa_WCET, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_G = pa_G;
    var_Tmin = pa_Tmin;
    var_Deadline = pa_Deadline;
    var_WCET = pa_WCET;
    receiveInputEvent(scm_nEventEI0ID, nullptr);
    pa_QO = var_QO;
  }
  void evt_EI1(const CIEC_BOOL &pa_QI, const CIEC_BOOL &pa_G, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline, const CIEC_TIME &pa_WCET, CIEC_BOOL &pa_QO) {
    var_QI = pa_QI;
    var_G = pa_G;
    var_Tmin = pa_Tmin;
    var_Deadline = pa_Deadline;
    var_WCET = pa_WCET;
    receiveInputEvent(scm_nEventEI1ID, nullptr);
    pa_QO = var_QO;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_BOOL &pa_G, const CIEC_TIME &pa_Tmin, const CIEC_TIME &pa_Deadline, const CIEC_TIME &pa_WCET, CIEC_BOOL &pa_QO) {
    evt_INIT(pa_QI, pa_G, pa_Tmin, pa_Deadline, pa_WCET, pa_QO);
  }
};



