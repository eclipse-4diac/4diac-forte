/*************************************************************************
 * Copyright (c) 2026 Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * This file was generated using the 4DIAC FORTE Export Filter 3.2.100.qualifier!
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::opcua {
  class FORTE_OpcUaCreateId final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_OpcUaCreateId)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventOpcUaMethodID = 0;
      static const TEventID scmEventOpcUaWriteID = 1;
      static const TEventID scmEventOpcUaSubscribeID = 2;
      static const TEventID scmEventOpcUaCreateObjectID = 3;

      static const CIEC_WSTRING var_const_PREFIX;
      static const CIEC_WSTRING var_const_SUFFIX;
      static const CIEC_WSTRING var_const_OPCUA_CREATE_METHOD;
      static const CIEC_WSTRING var_const_OPCUA_WRITE;
      static const CIEC_WSTRING var_const_OPCUA_SUBSCRIBE;
      static const CIEC_WSTRING var_const_OPCUA_CREATE_OBJECT;
      static const CIEC_WSTRING var_const_NODEID_TYPE;
      static const CIEC_WSTRING var_const_SEPARATOR_PAIR;
      static const CIEC_WSTRING var_const_SEPARATOR_HOST;
      static const CIEC_WSTRING var_const_SEPARATOR_HASH;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_MethodId(void);
      void alg_WriteId(void);
      void alg_SubscribeId(void);
      void alg_CreateObjectId(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStatecreateMethod = 1;
      static const TForteInt16 scmStatewrite = 2;
      static const TForteInt16 scmStatesubscribe = 3;
      static const TForteInt16 scmStatecreateObject = 4;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStatecreateMethod(CEventChainExecutionThread *const paECET);
      void enterStatewrite(CEventChainExecutionThread *const paECET);
      void enterStatesubscribe(CEventChainExecutionThread *const paECET);
      void enterStatecreateObject(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_OpcUaCreateId(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_WSTRING var_typeBrowseName;
      CIEC_WSTRING var_instanceBrowseName;
      CIEC_WSTRING var_instanceNodeId;
      CIEC_WSTRING var_host;
      CIEC_USINT var_port;

      CIEC_WSTRING var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_typeBrowseName;
      CDataConnection *conn_instanceBrowseName;
      CDataConnection *conn_instanceNodeId;
      CDataConnection *conn_host;
      CDataConnection *conn_port;

      COutDataConnection<CIEC_WSTRING> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_OpcUaMethod(const CIEC_WSTRING &patypeBrowseName, const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &pahost, const CIEC_USINT &paport, COutputParameter<CIEC_WSTRING> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_typeBrowseName = patypeBrowseName;
        var_instanceBrowseName = painstanceBrowseName;
        var_instanceNodeId = painstanceNodeId;
        var_host = pahost;
        var_port = paport;
        executeEvent(scmEventOpcUaMethodID, nullptr);
        *paOUT = var_OUT;
      }

      void evt_OpcUaWrite(const CIEC_WSTRING &patypeBrowseName, const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &pahost, const CIEC_USINT &paport, COutputParameter<CIEC_WSTRING> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_typeBrowseName = patypeBrowseName;
        var_instanceBrowseName = painstanceBrowseName;
        var_instanceNodeId = painstanceNodeId;
        var_host = pahost;
        var_port = paport;
        executeEvent(scmEventOpcUaWriteID, nullptr);
        *paOUT = var_OUT;
      }

      void evt_OpcUaSubscribe(const CIEC_WSTRING &patypeBrowseName, const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &pahost, const CIEC_USINT &paport, COutputParameter<CIEC_WSTRING> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_typeBrowseName = patypeBrowseName;
        var_instanceBrowseName = painstanceBrowseName;
        var_instanceNodeId = painstanceNodeId;
        var_host = pahost;
        var_port = paport;
        executeEvent(scmEventOpcUaSubscribeID, nullptr);
        *paOUT = var_OUT;
      }

      void evt_OpcUaCreateObject(const CIEC_WSTRING &patypeBrowseName, const CIEC_WSTRING &painstanceBrowseName, const CIEC_WSTRING &painstanceNodeId, const CIEC_WSTRING &pahost, const CIEC_USINT &paport, COutputParameter<CIEC_WSTRING> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_typeBrowseName = patypeBrowseName;
        var_instanceBrowseName = painstanceBrowseName;
        var_instanceNodeId = painstanceNodeId;
        var_host = pahost;
        var_port = paport;
        executeEvent(scmEventOpcUaCreateObjectID, nullptr);
        *paOUT = var_OUT;
      }
  };
}

