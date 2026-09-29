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
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::opcua {
  class FORTE_F_OpcUaCreatePaths final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_F_OpcUaCreatePaths)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_OpcUaCreatePaths(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WSTRING var_browsePrefix;
      CIEC_WSTRING var_nodePrefix;
      CIEC_WSTRING var_containerName;
      CIEC_WSTRING var_objectPrefix;
      CIEC_WSTRING var_objectName;

      CIEC_WSTRING var_browseName;
      CIEC_WSTRING var_nodeId;

      CEventConnection conn_CNF;

      CDataConnection *conn_browsePrefix;
      CDataConnection *conn_nodePrefix;
      CDataConnection *conn_containerName;
      CDataConnection *conn_objectPrefix;
      CDataConnection *conn_objectName;

      COutDataConnection<CIEC_WSTRING> conn_browseName;
      COutDataConnection<CIEC_WSTRING> conn_nodeId;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_WSTRING &pabrowsePrefix, const CIEC_WSTRING &panodePrefix, const CIEC_WSTRING &pacontainerName, const CIEC_WSTRING &paobjectPrefix, const CIEC_WSTRING &paobjectName, COutputParameter<CIEC_WSTRING> pabrowseName, COutputParameter<CIEC_WSTRING> panodeId) {
        COutputGuard guard_browseName(pabrowseName);
        COutputGuard guard_nodeId(panodeId);
        var_browsePrefix = pabrowsePrefix;
        var_nodePrefix = panodePrefix;
        var_containerName = pacontainerName;
        var_objectPrefix = paobjectPrefix;
        var_objectName = paobjectName;
        executeEvent(scmEventREQID, nullptr);
        *pabrowseName = var_browseName;
        *panodeId = var_nodeId;
      }

      void operator()(const CIEC_WSTRING &pabrowsePrefix, const CIEC_WSTRING &panodePrefix, const CIEC_WSTRING &pacontainerName, const CIEC_WSTRING &paobjectPrefix, const CIEC_WSTRING &paobjectName, COutputParameter<CIEC_WSTRING> pabrowseName, COutputParameter<CIEC_WSTRING> panodeId) {
        evt_REQ(std::forward<const CIEC_WSTRING &>(pabrowsePrefix), std::forward<const CIEC_WSTRING &>(panodePrefix), std::forward<const CIEC_WSTRING &>(pacontainerName), std::forward<const CIEC_WSTRING &>(paobjectPrefix), std::forward<const CIEC_WSTRING &>(paobjectName), std::forward<COutputParameter<CIEC_WSTRING>>(pabrowseName), std::forward<COutputParameter<CIEC_WSTRING>>(panodeId));
      }
  };

  void func_F_OpcUaCreatePaths(const CIEC_WSTRING &st_lv_browsePrefix, const CIEC_WSTRING &st_lv_nodePrefix, const CIEC_WSTRING &st_lv_containerName, const CIEC_WSTRING &st_lv_objectPrefix, const CIEC_WSTRING &st_lv_objectName, COutputParameter<CIEC_WSTRING> st_lv_browseName, COutputParameter<CIEC_WSTRING> st_lv_nodeId);
}

