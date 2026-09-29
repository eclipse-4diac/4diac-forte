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

#include "forte/eclipse4diac/opcua/F_OpcUaCreateSubscribeId_fct.h"

#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/eclipse4diac/opcua/F_OpcUaCreateSubscribeId_fct.h"
#include "forte/iec61131_functions/func_CONCAT.h"
#include "forte/iec61131_functions/func_USINT_AS_WSTRING.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"instanceBrowseName"_STRID, "instanceNodeId"_STRID, "host"_STRID, "port"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_OpcUaCreateSubscribeId, "eclipse4diac::opcua::F_OpcUaCreateSubscribeId"_STRID)

  FORTE_F_OpcUaCreateSubscribeId::FORTE_F_OpcUaCreateSubscribeId(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_instanceBrowseName(u""_WSTRING),
      var_instanceNodeId(u""_WSTRING),
      var_host(u""_WSTRING),
      var_port(0_USINT),
      var_(u""_WSTRING),
      conn_CNF(*this, 0),
      conn_instanceBrowseName(nullptr),
      conn_instanceNodeId(nullptr),
      conn_host(nullptr),
      conn_port(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_OpcUaCreateSubscribeId::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_instanceBrowseName = u""_WSTRING;
    var_instanceNodeId = u""_WSTRING;
    var_host = u""_WSTRING;
    var_port = 0_USINT;
    var_ = u""_WSTRING;
  }

  void FORTE_F_OpcUaCreateSubscribeId::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_instanceBrowseName, conn_instanceBrowseName);
        readData(1, var_instanceNodeId, conn_instanceNodeId);
        readData(2, var_host, conn_host);
        readData(3, var_port, conn_port);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_F_OpcUaCreateSubscribeId::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(4, var_, conn_);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_F_OpcUaCreateSubscribeId::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_instanceBrowseName;
      case 1: return &var_instanceNodeId;
      case 2: return &var_host;
      case 3: return &var_port;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_OpcUaCreateSubscribeId::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_OpcUaCreateSubscribeId::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_OpcUaCreateSubscribeId::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_instanceBrowseName;
      case 1: return &conn_instanceNodeId;
      case 2: return &conn_host;
      case 3: return &conn_port;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_OpcUaCreateSubscribeId::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_F_OpcUaCreateSubscribeId::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_F_OpcUaCreateSubscribeId(var_instanceBrowseName, var_instanceNodeId, var_host, var_port);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WSTRING func_F_OpcUaCreateSubscribeId(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId, const CIEC_WSTRING &st_lv_host, const CIEC_USINT &st_lv_port) {
    CIEC_WSTRING st_ret_val = u""_WSTRING;
    const CIEC_WSTRING st_lv_PREFIX = u"opc_ua[SUBSCRIBE;opc.tcp://"_WSTRING;
    const CIEC_WSTRING st_lv_NODEID_TYPE = u",s="_WSTRING;
    const CIEC_WSTRING st_lv_SUFFIX = u"]"_WSTRING;
    const CIEC_WSTRING st_lv_SEPARATOR_HASH = u"#;"_WSTRING;
    const CIEC_WSTRING st_lv_SEPARATOR_HOST = u":"_WSTRING;

    #line 18 "F_OpcUaCreateSubscribeId.fct"
    st_ret_val = func_CONCAT(st_lv_PREFIX, st_lv_host, st_lv_SEPARATOR_HOST, func_USINT_AS_WSTRING(st_lv_port), st_lv_SEPARATOR_HASH, st_lv_instanceBrowseName, st_lv_NODEID_TYPE, st_lv_instanceNodeId, st_lv_SUFFIX);

    return st_ret_val;
  }

}
