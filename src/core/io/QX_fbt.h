/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include <processinterfacefb.h>
#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


/*! /brief generic class for QX function blocks providing access to one boolean physical output
 *
 */
class FORTE_QX final : public forte::core::io::CProcessInterfaceFB {
  DECLARE_FIRMWARE_FB(FORTE_QX)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];
  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  static const TEventID scmEventINITID = 0;
  static const TEventID scmEventREQID = 1;
  static const TDataIOID scmEIWith[];
  static const TForteInt16 scmEIWithIndexes[];
  static const CStringDictionary::TStringId scmEventInputNames[];
  static const CStringDictionary::TStringId scmEventInputTypeIds[];
  static const TEventID scmEventINITOID = 0;
  static const TEventID scmEventCNFID = 1;
  static const TDataIOID scmEOWith[]; 
  static const TForteInt16 scmEOWithIndexes[];
  static const CStringDictionary::TStringId scmEventOutputNames[];
  static const CStringDictionary::TStringId scmEventOutputTypeIds[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void setInitialValues() override;

  CIEC_BOOL write() override {
    return forte::core::io::CProcessInterfaceFB::read(var_OUT);
  }

public:
  FORTE_QX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

  CIEC_BOOL var_OUT;

  CDataConnection *conn_OUT;

  CIEC_ANY *getDI(size_t) override;
  CDataConnection **getDIConUnchecked(TPortId) override;

  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_BOOL &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    var_OUT = pa_OUT;
    receiveInputEvent(scmEventINITID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void evt_REQ(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_BOOL &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    var_OUT = pa_OUT;
    receiveInputEvent(scmEventREQID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_BOOL &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    evt_INIT(pa_QI, pa_PARAMS, pa_OUT, pa_QO, pa_STATUS);
  }
};



