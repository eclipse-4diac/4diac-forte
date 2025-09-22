/*************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz - initial API and implementation and/or initial documentation
 *   Jose Cabral - Modification to double
 *************************************************************************/

#include "forte/io/QD_fbt.h"

using namespace forte::literals;

#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::io {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID, "OUT"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_QD, "eclipse4diac::io::QD"_STRID)

  FORTE_QD::FORTE_QD(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      CProcessInterface(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_PARAMS(""_STRING),
      var_OUT(0_DWORD),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_PARAMS(nullptr),
      conn_OUT(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {};

  void FORTE_QD::setInitialValues() {
    var_QI = 0_BOOL;
    var_PARAMS = ""_STRING;
    var_OUT = 0_DWORD;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
  }

  void FORTE_QD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventINITID:
        if (var_QI) {
          var_QO = CIEC_BOOL(CProcessInterface::initialise(false, paECET)); // initialise as output
        } else {
          var_QO = CIEC_BOOL(CProcessInterface::deinitialise());
        }
        sendOutputEvent(scmEventINITOID, paECET);
        break;
      case scmEventREQID:
        if (var_QI) {
          var_QO = CIEC_BOOL(CProcessInterface::write(var_OUT));
        } else {
          var_QO = false_BOOL;
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_QD::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_PARAMS, conn_PARAMS);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        readData(2, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  void FORTE_QD::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_QD::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PARAMS;
      case 2: return &var_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_QD::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_QD::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_QD::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PARAMS;
      case 2: return &conn_OUT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_QD::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::io
