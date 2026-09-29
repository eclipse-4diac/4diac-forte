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

#include "forte/eclipse4diac/opcua/OpcUaCreatePaths_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_CONCAT.h"
#include "forte/iec61131_functions/func_EQ.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::opcua {
  namespace {
    const auto cEventInputNames = std::array{"Create"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_OpcUaCreatePaths, "eclipse4diac::opcua::OpcUaCreatePaths"_STRID)
  const CIEC_WSTRING FORTE_OpcUaCreatePaths::var_const_BROWSE_SEPARATOR = u"/"_WSTRING;
  const CIEC_WSTRING FORTE_OpcUaCreatePaths::var_const_NODE_SEPARATOR = u"."_WSTRING;


  FORTE_OpcUaCreatePaths::FORTE_OpcUaCreatePaths(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_browsePrefix(u"/Objects/skills/"_WSTRING),
      var_nodePrefix(u"skills."_WSTRING),
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

  void FORTE_OpcUaCreatePaths::setInitialValues() {
    CBasicFB::setInitialValues();
    var_browsePrefix = u"/Objects/skills/"_WSTRING;
    var_nodePrefix = u"skills."_WSTRING;
    var_containerName = u""_WSTRING;
    var_objectPrefix = u""_WSTRING;
    var_objectName = u""_WSTRING;
    var_browseName = u""_WSTRING;
    var_nodeId = u""_WSTRING;
  }

  void FORTE_OpcUaCreatePaths::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch(mECCState) {
        case scmStateSTART:
          if(scmEventCreateID == paEIID) enterStatecreateIt(paECET);
          else return; //no transition cleared
          break;
        case scmStatecreateIt:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", mECCState.operator TForteUInt16 ());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while(true);
  }

  void FORTE_OpcUaCreatePaths::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_OpcUaCreatePaths::enterStatecreateIt(CEventChainExecutionThread *const paECET) {
    mECCState = scmStatecreateIt;
    alg_CreateIds();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_OpcUaCreatePaths::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCreateID: {
        readData(2, var_containerName, conn_containerName);
        readData(0, var_browsePrefix, conn_browsePrefix);
        readData(1, var_nodePrefix, conn_nodePrefix);
        readData(3, var_objectPrefix, conn_objectPrefix);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_OpcUaCreatePaths::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_OpcUaCreatePaths::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_browsePrefix;
      case 1: return &var_nodePrefix;
      case 2: return &var_containerName;
      case 3: return &var_objectPrefix;
      case 4: return &var_objectName;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_OpcUaCreatePaths::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_browseName;
      case 1: return &var_nodeId;
    }
    return nullptr;
  }

  CEventConnection *FORTE_OpcUaCreatePaths::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_OpcUaCreatePaths::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_browsePrefix;
      case 1: return &conn_nodePrefix;
      case 2: return &conn_containerName;
      case 3: return &conn_objectPrefix;
      case 4: return &conn_objectName;
    }
    return nullptr;
  }

  CDataConnection *FORTE_OpcUaCreatePaths::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_browseName;
      case 1: return &conn_nodeId;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_OpcUaCreatePaths::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_OpcUaCreatePaths::alg_CreateIds(void) {

    #line 2 "OpcUaCreatePaths.fbt"
    if (func_EQ(var_objectPrefix, u""_WSTRING)) {
      #line 3 "OpcUaCreatePaths.fbt"
      var_browseName = func_CONCAT(var_browsePrefix, var_containerName, var_const_BROWSE_SEPARATOR, var_objectName);
      #line 4 "OpcUaCreatePaths.fbt"
      var_nodeId = func_CONCAT(var_nodePrefix, var_containerName, var_const_NODE_SEPARATOR, var_objectName);
    }
    else {
      #line 6 "OpcUaCreatePaths.fbt"
      var_browseName = func_CONCAT(var_browsePrefix, var_containerName, var_const_BROWSE_SEPARATOR, var_objectPrefix, var_const_BROWSE_SEPARATOR, var_objectName);
      #line 7 "OpcUaCreatePaths.fbt"
      var_nodeId = func_CONCAT(var_nodePrefix, var_containerName, var_const_NODE_SEPARATOR, var_objectPrefix, var_const_NODE_SEPARATOR, var_objectName);
    }
  }

}
