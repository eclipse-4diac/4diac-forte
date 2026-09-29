/*************************************************************************
 * Copyright (c) 2026 Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_wstring.h"

namespace forte::eclipse4diac::opcua {
  class FORTE_F_OpcUaCreateObjectId final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_OpcUaCreateObjectId)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_OpcUaCreateObjectId(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WSTRING var_instanceBrowseName;
      CIEC_WSTRING var_instanceNodeId;
      CIEC_WSTRING var_typeBrowseName;

      CIEC_WSTRING var_;

      CEventConnection conn_CNF;

      CDataConnection *conn_instanceBrowseName;
      CDataConnection *conn_instanceNodeId;
      CDataConnection *conn_typeBrowseName;

      COutDataConnection<CIEC_WSTRING> conn_;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &patypeBrowseName) {
        var_instanceBrowseName = painstanceBrowseName;
        var_instanceNodeId = painstanceNodeId;
        var_typeBrowseName = patypeBrowseName;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &patypeBrowseName) {
        evt_REQ(std::forward<const CIEC_WSTRING &>(painstanceBrowseName), std::forward<const CIEC_WSTRING &>(painstanceNodeId), std::forward<const CIEC_WSTRING &>(patypeBrowseName));
      }
  };

  CIEC_WSTRING func_F_OpcUaCreateObjectId(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId, const CIEC_WSTRING &st_lv_typeBrowseName);
}

