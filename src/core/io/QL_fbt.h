/*************************************************************************
 * Copyright (c) 2018, 2025 TU Wien/ACIN, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds intial implememtation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include <processinterfacefb.h>
#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_lword.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_QL final : public forte::core::io::CProcessInterfaceFB {
  DECLARE_FIRMWARE_FB(FORTE_QL)

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
    FORTE_QL(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_LWORD var_OUT;

    CDataConnection *conn_OUT;

    CIEC_ANY *getDI(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_LWORD &paOUT, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      var_OUT = paOUT;
      receiveInputEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_LWORD &paOUT, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      var_OUT = paOUT;
      receiveInputEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_LWORD &paOUT, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paPARAMS, paOUT, paQO, paSTATUS);
    }
};

