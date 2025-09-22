/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/io/IX_fbt.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::io {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID, "IN"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_IX, "eclipse4diac::io::IX"_STRID)

  FORTE_IX::FORTE_IX(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      CProcessInterface(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_PARAMS(""_STRING),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      var_IN(0_BOOL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_IND(*this, 2),
      conn_QI(nullptr),
      conn_PARAMS(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      conn_IN(*this, 2, var_IN) {};

  void FORTE_IX::setInitialValues() {
    var_QI = 0_BOOL;
    var_PARAMS = ""_STRING;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
    var_IN = 0_BOOL;
  }

  void FORTE_IX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case cgExternalEventID: sendOutputEvent(scmEventINDID, paECET); break;
      case scmEventINITID:
        if (var_QI) {
          var_QO = CIEC_BOOL(CProcessInterface::initialise(true, paECET)); // initialise as input
        } else {
          var_QO = CIEC_BOOL(CProcessInterface::deinitialise());
        }
        sendOutputEvent(scmEventINITOID, paECET);
        break;
      case scmEventREQID:
        if (var_QI) {
          var_QO = CIEC_BOOL(CProcessInterface::read(var_IN));
        } else {
          var_QO = false_BOOL;
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_IX::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_PARAMS, conn_PARAMS);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_IX::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_IN, conn_IN);
        break;
      }
      case scmEventINDID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_IX::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PARAMS;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_IX::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
      case 2: return &var_IN;
    }
    return nullptr;
  }

  CEventConnection *FORTE_IX::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
      case 2: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_IX::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PARAMS;
    }
    return nullptr;
  }

  CDataConnection *FORTE_IX::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
      case 2: return &conn_IN;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::io
