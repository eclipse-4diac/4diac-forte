/*******************************************************************************
 * Copyright (c) 2007 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "cfb.h"
#include "typelib.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_E_R_TRIG: public CCompositeFB {
  DECLARE_FIRMWARE_FB(FORTE_E_R_TRIG)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const TEventID scm_nEventEIID = 0;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventEOID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  static const SCFB_FBInstanceData scm_astInternalFBs[];
  static const SCFB_FBParameter scm_astParamters[];
  static const SCFB_FBConnectionData scm_astEventConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];
  static const SCFB_FBConnectionData scm_astDataConnections[];
  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

  void readInputData(size_t pa_nEIID) override;
  void writeOutputData(size_t pa_nEIID) override;

public:
  FORTE_E_R_TRIG(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_QI;
  CEventConnection conn_EO;
  CDataConnection *conn_QI;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  void evt_EI(const CIEC_BOOL &pa_QI) {
    var_QI = pa_QI;
    receiveInputEvent(scm_nEventEIID, nullptr);
  }
  void operator()(const CIEC_BOOL &pa_QI) {
    evt_EI(pa_QI);
  }
};



