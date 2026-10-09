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

#include "forte/eclipse4diac/opcua/OpcUaCreateId_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_CONCAT.h"
#include "forte/iec61131_functions/func_USINT_AS_WSTRING.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    const auto cEventInputNames = std::array{"OpcUaMethod"_STRID, "OpcUaWrite"_STRID, "OpcUaSubscribe"_STRID, "OpcUaCreateObject"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"typeBrowseName"_STRID, "instanceBrowseName"_STRID, "instanceNodeId"_STRID, "host"_STRID, "port"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  }

  DEFINE_FIRMWARE_FB(FORTE_OpcUaCreateId, "eclipse4diac::opcua::OpcUaCreateId"_STRID)
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_PREFIX = u"opc_ua["_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_SUFFIX = u"]"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_OPCUA_CREATE_METHOD = u"CREATE_METHOD;"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_OPCUA_WRITE = u"WRITE;"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_OPCUA_SUBSCRIBE = u"SUBSCRIBE;opc.tcp://"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_OPCUA_CREATE_OBJECT = u"CREATE_OBJECT;"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_NODEID_TYPE = u",s="_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_SEPARATOR_PAIR = u";"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_SEPARATOR_HOST = u":"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreateId::var_const_SEPARATOR_HASH = u"#;"_WSTRING;


  FORTE_OpcUaCreateId::FORTE_OpcUaCreateId(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_typeBrowseName(u""_WSTRING),
      var_instanceBrowseName(u""_WSTRING),
      var_instanceNodeId(u""_WSTRING),
      var_host(u""_WSTRING),
      var_port(0_USINT),
      var_OUT(u""_WSTRING),
      conn_CNF(*this, 0),
      conn_typeBrowseName(nullptr),
      conn_instanceBrowseName(nullptr),
      conn_instanceNodeId(nullptr),
      conn_host(nullptr),
      conn_port(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_OpcUaCreateId::setInitialValues() {
    CBasicFB::setInitialValues();
    var_typeBrowseName = u""_WSTRING;
    var_instanceBrowseName = u""_WSTRING;
    var_instanceNodeId = u""_WSTRING;
    var_host = u""_WSTRING;
    var_port = 0_USINT;
    var_OUT = u""_WSTRING;
  }

  void FORTE_OpcUaCreateId::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch(mECCState) {
        case scmStateSTART:
          if(scmEventOpcUaMethodID == paEIID) enterStatecreateMethod(paECET);
          else
          if(scmEventOpcUaWriteID == paEIID) enterStatewrite(paECET);
          else
          if(scmEventOpcUaSubscribeID == paEIID) enterStatesubscribe(paECET);
          else
          if(scmEventOpcUaCreateObjectID == paEIID) enterStatecreateObject(paECET);
          else return; //no transition cleared
          break;
        case scmStatecreateMethod:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        case scmStatewrite:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        case scmStatesubscribe:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        case scmStatecreateObject:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 5.", mECCState.operator TForteUInt16 ());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while(true);
  }

  void FORTE_OpcUaCreateId::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_OpcUaCreateId::enterStatecreateMethod(CEventChainExecutionThread *const paECET) {
    mECCState = scmStatecreateMethod;
    alg_MethodId();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_OpcUaCreateId::enterStatewrite(CEventChainExecutionThread *const paECET) {
    mECCState = scmStatewrite;
    alg_WriteId();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_OpcUaCreateId::enterStatesubscribe(CEventChainExecutionThread *const paECET) {
    mECCState = scmStatesubscribe;
    alg_SubscribeId();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_OpcUaCreateId::enterStatecreateObject(CEventChainExecutionThread *const paECET) {
    mECCState = scmStatecreateObject;
    alg_CreateObjectId();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_OpcUaCreateId::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventOpcUaMethodID: {
        readData(1, var_instanceBrowseName, conn_instanceBrowseName);
        readData(2, var_instanceNodeId, conn_instanceNodeId);
        break;
      }
      case scmEventOpcUaWriteID: {
        readData(1, var_instanceBrowseName, conn_instanceBrowseName);
        readData(2, var_instanceNodeId, conn_instanceNodeId);
        break;
      }
      case scmEventOpcUaSubscribeID: {
        readData(1, var_instanceBrowseName, conn_instanceBrowseName);
        readData(2, var_instanceNodeId, conn_instanceNodeId);
        readData(3, var_host, conn_host);
        readData(4, var_port, conn_port);
        break;
      }
      case scmEventOpcUaCreateObjectID: {
        readData(0, var_typeBrowseName, conn_typeBrowseName);
        readData(1, var_instanceBrowseName, conn_instanceBrowseName);
        readData(2, var_instanceNodeId, conn_instanceNodeId);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_OpcUaCreateId::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(5, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_OpcUaCreateId::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_typeBrowseName;
      case 1: return &var_instanceBrowseName;
      case 2: return &var_instanceNodeId;
      case 3: return &var_host;
      case 4: return &var_port;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_OpcUaCreateId::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_OpcUaCreateId::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_OpcUaCreateId::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_typeBrowseName;
      case 1: return &conn_instanceBrowseName;
      case 2: return &conn_instanceNodeId;
      case 3: return &conn_host;
      case 4: return &conn_port;
    }
    return nullptr;
  }

  CDataConnection *FORTE_OpcUaCreateId::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_OpcUaCreateId::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_OpcUaCreateId::alg_MethodId(void) {

    #line 2 "OpcUaCreateId.fbt"
    var_OUT = func_CONCAT(var_const_PREFIX, var_const_OPCUA_CREATE_METHOD, var_instanceBrowseName, var_const_NODEID_TYPE, var_instanceNodeId, var_const_SUFFIX);
  }

  void FORTE_OpcUaCreateId::alg_WriteId(void) {

    #line 6 "OpcUaCreateId.fbt"
    var_OUT = func_CONCAT(var_const_PREFIX, var_const_OPCUA_WRITE, var_instanceBrowseName, var_const_NODEID_TYPE, var_instanceNodeId, var_const_SUFFIX);
  }

  void FORTE_OpcUaCreateId::alg_SubscribeId(void) {

    #line 10 "OpcUaCreateId.fbt"
    var_OUT = func_CONCAT(var_const_PREFIX, var_const_OPCUA_SUBSCRIBE, var_host, var_const_SEPARATOR_HOST, func_USINT_AS_WSTRING(var_port), var_const_SEPARATOR_HASH, var_instanceBrowseName, var_const_NODEID_TYPE, var_instanceNodeId, var_const_SUFFIX);
  }

  void FORTE_OpcUaCreateId::alg_CreateObjectId(void) {

    #line 15 "OpcUaCreateId.fbt"
    var_OUT = func_CONCAT(var_const_PREFIX, var_const_OPCUA_CREATE_OBJECT, var_typeBrowseName, var_const_SEPARATOR_PAIR, var_instanceBrowseName, var_const_NODEID_TYPE, var_instanceNodeId, var_const_SUFFIX);
  }

}
