/*************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds intial implememtation
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

#include "core/fmi/processinterface.h"

namespace forte::eclipse4diac::io {
  class FORTE_IB final : public CProcessInterface {
      DECLARE_FIRMWARE_FB(FORTE_IB)

    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINDID = 2;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_IB(forte::StringId paInstanceNameId, forte::CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_STRING var_PARAMS;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;
      CIEC_BYTE var_IN;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_PARAMS;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;
      COutDataConnection<CIEC_BYTE> conn_IN;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paPARAMS,
                    CAnyBitOutputParameter<CIEC_BOOL> &paQO,
                    COutputParameter<CIEC_STRING> &paSTATUS,
                    CAnyBitOutputParameter<CIEC_BYTE> &paIN) {
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        receiveInputEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
        *paIN = var_IN;
      }

      void evt_REQ(const CIEC_BOOL &paQI,
                   const CIEC_STRING &paPARAMS,
                   CAnyBitOutputParameter<CIEC_BOOL> &paQO,
                   COutputParameter<CIEC_STRING> &paSTATUS,
                   CAnyBitOutputParameter<CIEC_BYTE> &paIN) {
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        receiveInputEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
        *paIN = var_IN;
      }

      void operator()(const CIEC_BOOL &paQI,
                      const CIEC_STRING &paPARAMS,
                      CAnyBitOutputParameter<CIEC_BOOL> &paQO,
                      COutputParameter<CIEC_STRING> &paSTATUS,
                      CAnyBitOutputParameter<CIEC_BYTE> &paIN) {
        evt_INIT(paQI, paPARAMS, paQO, paSTATUS, paIN);
      }
  };
} // namespace forte::eclipse4diac::io
