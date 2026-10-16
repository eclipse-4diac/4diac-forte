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

#include "forte/eclipse4diac/opcua/F_OpcUaCreateWriteId_fct.h"

#include "forte/datatypes/forte_wstring.h"
#include "forte/eclipse4diac/opcua/F_OpcUaCreateWriteId_fct.h"
#include "forte/iec61131_functions/func_CONCAT.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"instanceBrowseName"_STRID, "instanceNodeId"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_OpcUaCreateWriteId, "eclipse4diac::opcua::F_OpcUaCreateWriteId"_STRID)

  FORTE_F_OpcUaCreateWriteId::FORTE_F_OpcUaCreateWriteId(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_instanceBrowseName(u""_WSTRING),
      var_instanceNodeId(u""_WSTRING),
      var_(u""_WSTRING),
      conn_CNF(*this, 0),
      conn_instanceBrowseName(nullptr),
      conn_instanceNodeId(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_OpcUaCreateWriteId::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_instanceBrowseName = u""_WSTRING;
    var_instanceNodeId = u""_WSTRING;
    var_ = u""_WSTRING;
  }

  void FORTE_F_OpcUaCreateWriteId::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_instanceBrowseName, conn_instanceBrowseName);
        readData(1, var_instanceNodeId, conn_instanceNodeId);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_F_OpcUaCreateWriteId::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(2, var_, conn_);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_F_OpcUaCreateWriteId::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_instanceBrowseName;
      case 1: return &var_instanceNodeId;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_OpcUaCreateWriteId::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_OpcUaCreateWriteId::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_OpcUaCreateWriteId::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_instanceBrowseName;
      case 1: return &conn_instanceNodeId;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_OpcUaCreateWriteId::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_F_OpcUaCreateWriteId::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_F_OpcUaCreateWriteId(var_instanceBrowseName, var_instanceNodeId);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WSTRING func_F_OpcUaCreateWriteId(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId) {
    CIEC_WSTRING st_ret_val = u""_WSTRING;
    const CIEC_WSTRING st_lv_PREFIX = u"opc_ua[WRITE;"_WSTRING;
    const CIEC_WSTRING st_lv_NODEID_TYPE = u",s="_WSTRING;
    const CIEC_WSTRING st_lv_SUFFIX = u"]"_WSTRING;

    #line 14 "F_OpcUaCreateWriteId.fct"
    st_ret_val = func_CONCAT(st_lv_PREFIX, st_lv_instanceBrowseName, st_lv_NODEID_TYPE, st_lv_instanceNodeId, st_lv_SUFFIX);

    return st_ret_val;
  }

}
