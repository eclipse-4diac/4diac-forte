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

#include "forte/eclipse4diac/opcua/F_OpcUaCreatePaths_fct.h"

#include "forte/datatypes/forte_wstring.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_CONCAT.h"
#include "forte/iec61131_functions/func_EQ.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"browsePrefix"_STRID, "nodePrefix"_STRID, "containerName"_STRID, "objectPrefix"_STRID, "objectName"_STRID};
    const auto cDataOutputNames = std::array{"browseName"_STRID, "nodeId"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_OpcUaCreatePaths, "eclipse4diac::opcua::F_OpcUaCreatePaths"_STRID)

  FORTE_F_OpcUaCreatePaths::FORTE_F_OpcUaCreatePaths(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_browsePrefix(u""_WSTRING),
      var_nodePrefix(u""_WSTRING),
      var_containerName(u""_WSTRING),
      var_objectPrefix(u""_WSTRING),
      var_objectName(u""_WSTRING),
      var_browseName(u""_WSTRING),
      var_nodeId(u""_WSTRING),
      conn_CNF(*this, 0),
      conn_browsePrefix(nullptr),
      conn_nodePrefix(nullptr),
      conn_containerName(nullptr),
      conn_objectPrefix(nullptr),
      conn_objectName(nullptr),
      conn_browseName(*this, 0, var_browseName),
      conn_nodeId(*this, 1, var_nodeId) {
  }

  void FORTE_F_OpcUaCreatePaths::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_browsePrefix = u""_WSTRING;
    var_nodePrefix = u""_WSTRING;
    var_containerName = u""_WSTRING;
    var_objectPrefix = u""_WSTRING;
    var_objectName = u""_WSTRING;
    var_browseName = u""_WSTRING;
    var_nodeId = u""_WSTRING;
  }

  void FORTE_F_OpcUaCreatePaths::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_browsePrefix, conn_browsePrefix);
        readData(1, var_nodePrefix, conn_nodePrefix);
        readData(2, var_containerName, conn_containerName);
        readData(3, var_objectPrefix, conn_objectPrefix);
        readData(4, var_objectName, conn_objectName);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_F_OpcUaCreatePaths::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(5, var_browseName, conn_browseName);
        writeData(6, var_nodeId, conn_nodeId);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_F_OpcUaCreatePaths::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_browsePrefix;
      case 1: return &var_nodePrefix;
      case 2: return &var_containerName;
      case 3: return &var_objectPrefix;
      case 4: return &var_objectName;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_OpcUaCreatePaths::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_browseName;
      case 1: return &var_nodeId;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_OpcUaCreatePaths::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_OpcUaCreatePaths::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_browsePrefix;
      case 1: return &conn_nodePrefix;
      case 2: return &conn_containerName;
      case 3: return &conn_objectPrefix;
      case 4: return &conn_objectName;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_OpcUaCreatePaths::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_browseName;
      case 1: return &conn_nodeId;
    }
    return nullptr;
  }

  void FORTE_F_OpcUaCreatePaths::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_F_OpcUaCreatePaths(var_browsePrefix, var_nodePrefix, var_containerName, var_objectPrefix, var_objectName, var_browseName, var_nodeId);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_F_OpcUaCreatePaths(const CIEC_WSTRING &st_lv_browsePrefix, const CIEC_WSTRING &st_lv_nodePrefix, const CIEC_WSTRING &st_lv_containerName, const CIEC_WSTRING &st_lv_objectPrefix, const CIEC_WSTRING &st_lv_objectName, COutputParameter<CIEC_WSTRING> st_lv_browseName, COutputParameter<CIEC_WSTRING> st_lv_nodeId) {
    COutputGuard st_guard_browseName(st_lv_browseName);
    COutputGuard st_guard_nodeId(st_lv_nodeId);
    (*st_lv_browseName) = u""_WSTRING;
    (*st_lv_nodeId) = u""_WSTRING;
    const CIEC_WSTRING st_lv_BROWSE_SEPARATOR = u"/"_WSTRING;
    const CIEC_WSTRING st_lv_NODE_SEPARATOR = u"."_WSTRING;

    #line 20 "F_OpcUaCreatePaths.fct"
    if (func_EQ(st_lv_objectPrefix, u""_WSTRING)) {
      #line 21 "F_OpcUaCreatePaths.fct"
      (*st_lv_browseName) = func_CONCAT(st_lv_browsePrefix, st_lv_containerName, st_lv_BROWSE_SEPARATOR, st_lv_objectName);
      #line 22 "F_OpcUaCreatePaths.fct"
      (*st_lv_nodeId) = func_CONCAT(st_lv_nodePrefix, st_lv_containerName, st_lv_NODE_SEPARATOR, st_lv_objectName);
    }
    else {
      #line 24 "F_OpcUaCreatePaths.fct"
      (*st_lv_browseName) = func_CONCAT(st_lv_browsePrefix, st_lv_containerName, st_lv_BROWSE_SEPARATOR, st_lv_objectPrefix, st_lv_BROWSE_SEPARATOR, st_lv_objectName);
      #line 25 "F_OpcUaCreatePaths.fct"
      (*st_lv_nodeId) = func_CONCAT(st_lv_nodePrefix, st_lv_containerName, st_lv_NODE_SEPARATOR, st_lv_objectPrefix, st_lv_NODE_SEPARATOR, st_lv_objectName);
    }

  }

}
