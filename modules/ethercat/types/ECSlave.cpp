/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#include "ECSlave.h"

#include "../handler/bus.h"
#include "../slave/ec_device.h"
#include "../utils/EsiFileParser.h"
#include "forte/util/string_utils.h"

#include <string>

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {
  namespace {
    const auto cSocketNameIds = std::array{"BusAdapterIn"_STRID};
    const auto cPlugNameIds = std::array{"BusAdapterOut"_STRID};

    const auto cEventInputNameIds = std::array{"MAP"_STRID};
    const auto cEventOutputNameIds = std::array{"MAPO"_STRID, "IND"_STRID};

    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(FORTE_ECSlave, "eclipse4diac::io::ethercat::GEN_ECSlave"_STRID)

  const TForteUInt8 FORTE_ECSlave::scmSlaveConfigurationIO[] = {};
  const TForteUInt8 FORTE_ECSlave::scmSlaveConfigurationIONum = 0;

  FORTE_ECSlave::FORTE_ECSlave(const forte::StringId paInstanceNameId,
                               CFBContainer &paContainer,
                               ECSlaveHandler::SlaveType paSlaveType) :
      CGenFunctionBlock<forte::io::IOConfigHandlerFBMultiSlave>(paContainer,
                                           paInstanceNameId,
                                           scmSlaveConfigurationIO,
                                           scmSlaveConfigurationIONum,
                                           static_cast<int>(paSlaveType)),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_Config(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0) {
  }

  FORTE_ECSlave::~FORTE_ECSlave() {
    const size_t numDIs = getFBInterfaceSpec().getNumDIs();
    const size_t genOffset = getGenDIOffset();
    const size_t safeGenDINums = (numDIs > genOffset) ? (numDIs - genOffset) : 0;
    for (size_t i = 0; i < safeGenDINums && mGenDIs; ++i) {
      delete mGenDIs[i];
    }
  }

  void FORTE_ECSlave::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventMAPID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_Config, conn_Config);
      for (TPortId i = 0; i < static_cast<TPortId>(getGenDINums()); ++i) {
        readData(static_cast<TPortId>(i + getGenDIOffset()), *mGenDIs[i], mGenDIConns[i]);
      }
    }
  }

  void FORTE_ECSlave::writeOutputData(const TEventID paEIID) {
    const size_t numDIs = getFBInterfaceSpec().getNumDIs();
    switch (paEIID) {
      case scmEventMAPOID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        break;
      }
      case scmEventINDID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        writeData(numDIs + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ECSlave::getDI(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Config;
      default: return mGenDIs[paIndex - getGenDIOffset()];
    }
  }

  CIEC_ANY *FORTE_ECSlave::getDO(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ECSlave::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ECSlave::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Config;
      default: return CGenFunctionBlock<forte::io::IOConfigHandlerFBMultiSlave>::getDIConUnchecked(paIndex);
    }
  }

  CDataConnection *FORTE_ECSlave::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_ECSlave::getPlugPinUnchecked(const size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *FORTE_ECSlave::getSocketPinUnchecked(const size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  void FORTE_ECSlave::createGenInputData() {
    const size_t n = getGenDINums();
    mGenDIs = std::make_unique<CIEC_ANY *[]>(n);
    for (size_t i = 0; i < n; ++i) {
      mGenDIs[i] = new CIEC_WSTRING();
    }
  }

  bool FORTE_ECSlave::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    std::string tempstring(paConfigString);
    const char *sParamA = nullptr;
    const char *sParamB = nullptr;

    const size_t firstUnderscore = tempstring.find('_');
    const size_t secondUnderscore = tempstring.find('_', firstUnderscore == std::string::npos ? 0 : firstUnderscore + 1);
    if (firstUnderscore != std::string::npos && secondUnderscore != std::string::npos &&
        firstUnderscore + 1 < tempstring.size() && secondUnderscore + 1 < tempstring.size()) {
      tempstring[secondUnderscore] = '\0';
      sParamA = &(tempstring[firstUnderscore + 1]);
      sParamB = &(tempstring[secondUnderscore + 1]);
    }

    if (sParamB == nullptr) {
      return false;
    }

    configureDIDOs(sParamA, sParamB, paInterfaceSpec);

    paInterfaceSpec.mEINames = cEventInputNameIds;
    paInterfaceSpec.mEITypeNames = cEventInputTypeIds;
    paInterfaceSpec.mEONames = cEventOutputNameIds;
    paInterfaceSpec.mEOTypeNames = cEventOutputTypeIds;
    paInterfaceSpec.mSocketNames = cSocketNameIds;
    paInterfaceSpec.mPlugNames = cPlugNameIds;
    return true;
  }

  void FORTE_ECSlave::configureDIDOs(const char *paDIConfigString,
                                     const char *paDOConfigString,
                                     SFBInterfaceSpec &paInterfaceSpec) {
    mDiNames.clear();
    mDoNames.clear();

    mDiNames.emplace_back("QI"_STRID);
    mDiNames.emplace_back("Config"_STRID);

    mNumInPdus = static_cast<size_t>(util::strtol(paDIConfigString, nullptr, 10));
    mNumOutPdus = static_cast<size_t>(util::strtol(paDOConfigString, nullptr, 10));

    generateGenericInterfacePointNameArray("IN_", mDiNames, mNumInPdus);
    generateGenericInterfacePointNameArray("OUT_", mDiNames, mNumOutPdus);

    mDoNames.emplace_back("QO"_STRID);
    mDoNames.emplace_back("STATUS"_STRID);

    paInterfaceSpec.mDINames = mDiNames;
    paInterfaceSpec.mDONames = mDoNames;
  }

  bool FORTE_ECSlave::createSlaveHandler() {
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    auto *handler = new ECDeviceHandler(&bus, ECSlaveHandler::SlaveType::ECSlave, mIndex);
    ECDeviceHandler::Config cfg{};
    cfg.mAlias = static_cast<TForteUInt16>(Config().Alias);
    cfg.mPosition = static_cast<TForteUInt16>(Config().Position);
    cfg.mVendorId = static_cast<TForteUInt32>(Config().VendorId);
    cfg.mProductCode = static_cast<TForteUInt32>(Config().ProductCode);
    handler->setConfig(&cfg);
    handler->mDelegate = this;
    bus.addSlave(handler);
    return true;
  }

  const char *FORTE_ECSlave::init() {
    const auto productCode = static_cast<TForteUInt32>(Config().ProductCode);
    std::string errMsg;
    if (!EsiFileParser::getInstance().loadDevice(productCode, errMsg)) {
      mLastError = errMsg;
      return mLastError.c_str();
    }
    return nullptr;
  }

  void FORTE_ECSlave::deInit() { 
    auto &bus = *static_cast<ECBusHandler *>(&getController()); 
    ECSlaveHandler *slave = bus.getSlave(mIndex);
    if (slave != nullptr && slave->mDelegate == this) {
      slave->mDelegate = nullptr;
    }
  }

  void FORTE_ECSlave::initHandles() {
    if (!QI()) {
      return;
    }
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    auto *device = static_cast<ECDeviceHandler *>(bus.getSlave(mIndex));
    if (nullptr == device) {
      return;
    }
    EsiFileParser::getInstance().initDeviceIOHandles(static_cast<TForteUInt32>(Config().ProductCode), device, *this);
  }

  void FORTE_ECSlave::onSlaveStatus(ECSlaveHandler::SlaveStatus paStatus, ECSlaveHandler::SlaveStatus) {
    switch (paStatus) {
      case ECSlaveHandler::OK: STATUS() = scmOK; break;
      case ECSlaveHandler::Error: STATUS() = u"Error"_WSTRING; break;
      case ECSlaveHandler::NotInitialized: STATUS() = u"NotInitialized"_WSTRING; break;
      default: STATUS() = u"Unknown"_WSTRING; break;
    }
    sendOutputEvent(scmEventINDID, getEventChainExecutor());
  }

  void FORTE_ECSlave::onSlaveDestroy() {
    deInit();
    QO() = false_BOOL;
    STATUS() = u"Slave destroyed"_WSTRING;
  }

} // namespace forte::eclipse4diac::io::ethercat
