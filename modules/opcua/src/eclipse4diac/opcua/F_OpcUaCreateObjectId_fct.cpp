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

#include "forte/eclipse4diac/opcua/F_OpcUaCreateObjectId_fct.h"

#include "forte/datatypes/forte_wstring.h"
#include "forte/eclipse4diac/opcua/F_OpcUaCreateObjectId_fct.h"
#include "forte/iec61131_functions/func_CONCAT.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"instanceBrowseName"_STRID, "instanceNodeId"_STRID, "typeBrowseName"_STRID};
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

    CIEC_WSTRING func_F_OpcUaCreateObjectId__impl(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId, const CIEC_WSTRING &st_lv_typeBrowseName);
  }

  DEFINE_FIRMWARE_FB(FORTE_F_OpcUaCreateObjectId, "eclipse4diac::opcua::F_OpcUaCreateObjectId"_STRID, TypeHash)

  FORTE_F_OpcUaCreateObjectId::FORTE_F_OpcUaCreateObjectId(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_instanceBrowseName(u""_WSTRING),
      var_instanceNodeId(u""_WSTRING),
      var_typeBrowseName(u""_WSTRING),
      var_(u""_WSTRING),
      conn_CNF(*this, 0),
      conn_instanceBrowseName(nullptr),
      conn_instanceNodeId(nullptr),
      conn_typeBrowseName(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_F_OpcUaCreateObjectId::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_instanceBrowseName = u""_WSTRING;
    var_instanceNodeId = u""_WSTRING;
    var_typeBrowseName = u""_WSTRING;
    var_ = u""_WSTRING;
  }

  void FORTE_F_OpcUaCreateObjectId::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_instanceBrowseName, conn_instanceBrowseName);
        readData(1, var_instanceNodeId, conn_instanceNodeId);
        readData(2, var_typeBrowseName, conn_typeBrowseName);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_F_OpcUaCreateObjectId::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(3, var_, conn_);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_F_OpcUaCreateObjectId::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_instanceBrowseName;
      case 1: return &var_instanceNodeId;
      case 2: return &var_typeBrowseName;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_OpcUaCreateObjectId::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_OpcUaCreateObjectId::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_OpcUaCreateObjectId::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_instanceBrowseName;
      case 1: return &conn_instanceNodeId;
      case 2: return &conn_typeBrowseName;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_OpcUaCreateObjectId::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_F_OpcUaCreateObjectId::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_F_OpcUaCreateObjectId__impl(var_instanceBrowseName, var_instanceNodeId, var_typeBrowseName);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WSTRING func_F_OpcUaCreateObjectId(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId, const CIEC_WSTRING &st_lv_typeBrowseName) {
    return func_F_OpcUaCreateObjectId__impl(st_lv_instanceBrowseName, st_lv_instanceNodeId, st_lv_typeBrowseName);
  }

  namespace {
    CIEC_WSTRING func_F_OpcUaCreateObjectId__impl(const CIEC_WSTRING &st_lv_instanceBrowseName, const CIEC_WSTRING &st_lv_instanceNodeId, const CIEC_WSTRING &st_lv_typeBrowseName) {
      CIEC_WSTRING st_ret_val = u""_WSTRING;
      const CIEC_WSTRING st_lv_PREFIX = u"opc_ua[CREATE_OBJECT;"_WSTRING;
      const CIEC_WSTRING st_lv_NODEID_TYPE = u",s="_WSTRING;
      const CIEC_WSTRING st_lv_SUFFIX = u"]"_WSTRING;
      const CIEC_WSTRING st_lv_SEPARATOR_PAIR = u";"_WSTRING;

      #line 16 "F_OpcUaCreateObjectId.fct"
      st_ret_val = func_CONCAT(st_lv_PREFIX, st_lv_typeBrowseName, st_lv_SEPARATOR_PAIR, st_lv_instanceBrowseName, st_lv_NODEID_TYPE, st_lv_instanceNodeId, st_lv_SUFFIX);

      return st_ret_val;
    }
  }
}
