/*******************************************************************************
 * Copyright (c) 2019, 2025 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "plc01a1_config_fb.h"
#include "plc01a1_controller.h"

using namespace forte::literals;
namespace forte::eclipse4diac::io::plc01a1 {

  namespace {
    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames =
        std::array{"QI"_STRID,   "IN1"_STRID,  "IN2"_STRID,  "IN3"_STRID,  "IN4"_STRID,  "IN5"_STRID,
                   "IN6"_STRID,  "IN7"_STRID,  "IN8"_STRID,  "OUT1"_STRID, "OUT2"_STRID, "OUT3"_STRID,
                   "OUT4"_STRID, "OUT5"_STRID, "OUT6"_STRID, "OUT7"_STRID, "OUT8"_STRID, "UpdateInterval"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_PLC01A1, "eclipse4diac::io::plc01a1::PLC01A1"_STRID)

  FORTE_PLC01A1::FORTE_PLC01A1(const StringId paInstanceNameId, CFBContainer &paContainer) :
      forte::io::IOConfigFBController(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_IN1(u""_WSTRING),
      var_IN2(u""_WSTRING),
      var_IN3(u""_WSTRING),
      var_IN4(u""_WSTRING),
      var_IN5(u""_WSTRING),
      var_IN6(u""_WSTRING),
      var_IN7(u""_WSTRING),
      var_IN8(u""_WSTRING),
      var_OUT1(u""_WSTRING),
      var_OUT2(u""_WSTRING),
      var_OUT3(u""_WSTRING),
      var_OUT4(u""_WSTRING),
      var_OUT5(u""_WSTRING),
      var_OUT6(u""_WSTRING),
      var_OUT7(u""_WSTRING),
      var_OUT8(u""_WSTRING),
      var_UpdateInterval(25_UINT),
      var_QO(0_BOOL),
      var_STATUS(u""_WSTRING),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_IN1(nullptr),
      conn_IN2(nullptr),
      conn_IN3(nullptr),
      conn_IN4(nullptr),
      conn_IN5(nullptr),
      conn_IN6(nullptr),
      conn_IN7(nullptr),
      conn_IN8(nullptr),
      conn_OUT1(nullptr),
      conn_OUT2(nullptr),
      conn_OUT3(nullptr),
      conn_OUT4(nullptr),
      conn_OUT5(nullptr),
      conn_OUT6(nullptr),
      conn_OUT7(nullptr),
      conn_OUT8(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {};

  void FORTE_PLC01A1::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_IN1 = u""_WSTRING;
    var_IN2 = u""_WSTRING;
    var_IN3 = u""_WSTRING;
    var_IN4 = u""_WSTRING;
    var_IN5 = u""_WSTRING;
    var_IN6 = u""_WSTRING;
    var_IN7 = u""_WSTRING;
    var_IN8 = u""_WSTRING;
    var_OUT1 = u""_WSTRING;
    var_OUT2 = u""_WSTRING;
    var_OUT3 = u""_WSTRING;
    var_OUT4 = u""_WSTRING;
    var_OUT5 = u""_WSTRING;
    var_OUT6 = u""_WSTRING;
    var_OUT7 = u""_WSTRING;
    var_OUT8 = u""_WSTRING;
    var_UpdateInterval = 25_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_PLC01A1::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(17, var_UpdateInterval, conn_UpdateInterval);
        readData(1, var_IN1, conn_IN1);
        readData(2, var_IN2, conn_IN2);
        readData(3, var_IN3, conn_IN3);
        readData(4, var_IN4, conn_IN4);
        readData(5, var_IN5, conn_IN5);
        readData(6, var_IN6, conn_IN6);
        readData(7, var_IN7, conn_IN7);
        readData(8, var_IN8, conn_IN8);
        readData(9, var_OUT1, conn_OUT1);
        readData(10, var_OUT2, conn_OUT2);
        readData(11, var_OUT3, conn_OUT3);
        readData(12, var_OUT4, conn_OUT4);
        readData(13, var_OUT5, conn_OUT5);
        readData(14, var_OUT6, conn_OUT6);
        readData(15, var_OUT7, conn_OUT7);
        readData(16, var_OUT8, conn_OUT8);
        break;
      }
      default: break;
    }
  }

  void FORTE_PLC01A1::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(18, var_QO, conn_QO);
        writeData(19, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventINDID: {
        writeData(18, var_QO, conn_QO);
        writeData(19, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_PLC01A1::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_IN1;
      case 2: return &var_IN2;
      case 3: return &var_IN3;
      case 4: return &var_IN4;
      case 5: return &var_IN5;
      case 6: return &var_IN6;
      case 7: return &var_IN7;
      case 8: return &var_IN8;
      case 9: return &var_OUT1;
      case 10: return &var_OUT2;
      case 11: return &var_OUT3;
      case 12: return &var_OUT4;
      case 13: return &var_OUT5;
      case 14: return &var_OUT6;
      case 15: return &var_OUT7;
      case 16: return &var_OUT8;
      case 17: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_PLC01A1::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_PLC01A1::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_PLC01A1::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_IN1;
      case 2: return &conn_IN2;
      case 3: return &conn_IN3;
      case 4: return &conn_IN4;
      case 5: return &conn_IN5;
      case 6: return &conn_IN6;
      case 7: return &conn_IN7;
      case 8: return &conn_IN8;
      case 9: return &conn_OUT1;
      case 10: return &conn_OUT2;
      case 11: return &conn_OUT3;
      case 12: return &conn_OUT4;
      case 13: return &conn_OUT5;
      case 14: return &conn_OUT6;
      case 15: return &conn_OUT7;
      case 16: return &conn_OUT8;
      case 17: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_PLC01A1::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::io::IODeviceController *FORTE_PLC01A1::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new PLC01A1Controller(paDeviceExecution);
  }

  void FORTE_PLC01A1::setConfig() {
    PLC01A1Controller::Config config;
    config.mUpdateInterval = var_UpdateInterval.operator TForteUInt16();
    getDeviceController()->setConfig(&config);
  }

  void FORTE_PLC01A1::onStartup(CEventChainExecutionThread *const paECET) {
    // Initialize handles
    size_t initialDIOffset = 1;
    size_t numberOfInputs = 8;
    size_t numberOfOutputs = 8;

    for (size_t i = 0; i < numberOfInputs; i++) {
      PLC01A1Controller::HandleDescriptor desc = PLC01A1Controller::HandleDescriptor(
          static_cast<CIEC_WSTRING *>(getDI(initialDIOffset + i))->getValue(), forte::io::IOMapper::In,
          0 /*offset is always 0 */, static_cast<uint8_t>(i));
      initHandle(desc);
    }

    for (size_t i = 0; i < numberOfOutputs; i++) {
      PLC01A1Controller::HandleDescriptor desc = PLC01A1Controller::HandleDescriptor(
          static_cast<CIEC_WSTRING *>(getDI(initialDIOffset + numberOfInputs + i))->getValue(),
          forte::io::IOMapper::Out, 0 /*offset is always 0 */, static_cast<uint8_t>(numberOfOutputs - i - 1));
      initHandle(desc);
    }

    started(paECET);
  }

} // namespace forte::eclipse4diac::io::plc01a1
