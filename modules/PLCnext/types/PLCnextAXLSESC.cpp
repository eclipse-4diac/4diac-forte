/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextAXLSESC
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextAXLSESC.h"

using namespace forte::literals;

#include "PLCnextBusAdapter.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_PLCnextAXLSESC, "PLCnextAXLSESC"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
  const auto cEventInputNames = std::array{"INIT"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};
  const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
  const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
  const auto cSocketNameIds = std::array{"BusAdapterIn"_STRID};
  const auto cPlugNameIds = std::array{"BusAdapterOut"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = cPlugNameIds,
  };
} // namespace

FORTE_PLCnextAXLSESC::FORTE_PLCnextAXLSESC(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    PLCnextSlaveHandler(NoUsage, paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_INITO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_PLCnextAXLSESC::setInitialValues() {
  var_QI = 0_BOOL;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_PLCnextAXLSESC::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_PLCnextAXLSESC::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_PLCnextAXLSESC::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextAXLSESC::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextAXLSESC::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextAXLSESC::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextAXLSESC::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_PLCnextAXLSESC::initHandles() {
}

const char *FORTE_PLCnextAXLSESC::init() {
  PLCnextDeviceController &controller = *static_cast<PLCnextDeviceController *>(&getController());
  controller.registerSlaveHandler(this);
  return 0;
}
