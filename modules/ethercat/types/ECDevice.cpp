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
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#include "ECDevice.h"

#include "../handler/bus.h"
#include "../device/ec_device.h"
#include "../utils/esi_io_configurator.h"
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

  DEFINE_GENERIC_FIRMWARE_FB(FORTE_ECDevice, "eclipse4diac::io::ethercat::GEN_ECDevice"_STRID)

  const TForteUInt8 FORTE_ECDevice::scmSlaveConfigurationIO[] = {};
  const TForteUInt8 FORTE_ECDevice::scmSlaveConfigurationIONum = 0;

  FORTE_ECDevice::FORTE_ECDevice(const forte::StringId paInstanceNameId,
                                 CFBContainer &paContainer,
                                 ECBusDeviceHandler::DeviceType paDeviceType) :
      CGenFunctionBlock<forte::io::IOConfigFBMultiSlave>(paContainer,
                                                         paInstanceNameId,
                                                         scmSlaveConfigurationIO,
                                                         scmSlaveConfigurationIONum,
                                                         static_cast<int>(paDeviceType)),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_Config(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 1) {
  }

  FORTE_ECDevice::~FORTE_ECDevice() {
    const size_t numDIs = getFBInterfaceSpec().getNumDIs();
    const size_t genOffset = getGenDIOffset();
    const size_t safeGenDINums = (numDIs > genOffset) ? (numDIs - genOffset) : 0;
    for (size_t i = 0; i < safeGenDINums && mGenDIs; ++i) {
      delete mGenDIs[i];
    }
  }

  void FORTE_ECDevice::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventMAPID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_Config, conn_Config);
      for (TPortId i = 0; i < static_cast<TPortId>(getGenDINums()); ++i) {
        readData(static_cast<TPortId>(i + getGenDIOffset()), *mGenDIs[i], mGenDIConns[i]);
      }
    }
  }

  void FORTE_ECDevice::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_ECDevice::mappingDi(const TPortId paRelativeIndex) {
    return getDI(static_cast<TPortId>(getGenDIOffset() + paRelativeIndex));
  }

  void FORTE_ECDevice::registerMappedHandle(forte::io::IODeviceController::HandleDescriptor &paDesc) {
    initHandle(paDesc);
  }

  CIEC_ANY *FORTE_ECDevice::getDI(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Config;
      default: return mGenDIs[paIndex - getGenDIOffset()];
    }
  }

  CIEC_ANY *FORTE_ECDevice::getDO(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ECDevice::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ECDevice::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Config;
      default: return CGenFunctionBlock<forte::io::IOConfigFBMultiSlave>::getDIConUnchecked(paIndex);
    }
  }

  CDataConnection *FORTE_ECDevice::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_ECDevice::getPlugPinUnchecked(const size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *FORTE_ECDevice::getSocketPinUnchecked(const size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  void FORTE_ECDevice::createGenInputData() {
    const size_t n = getGenDINums();
    mGenDIs = std::make_unique<CIEC_ANY *[]>(n);
    for (size_t i = 0; i < n; ++i) {
      mGenDIs[i] = new CIEC_WSTRING();
    }
  }

  bool FORTE_ECDevice::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
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

  void FORTE_ECDevice::configureDIDOs(const char *paDIConfigString,
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

  bool FORTE_ECDevice::createSlaveHandler() {
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    auto *handler = new ECDeviceHandler(&bus, ECBusDeviceHandler::DeviceType::ECDevice, mIndex);
    ECDeviceHandler::Config cfg{};
    cfg.mAlias = static_cast<TForteUInt16>(Config().Alias);
    cfg.mPosition = static_cast<TForteUInt16>(Config().Position);
    cfg.mVendorId = static_cast<TForteUInt32>(Config().VendorId);
    cfg.mProductCode = static_cast<TForteUInt32>(Config().ProductCode);
    handler->setConfig(&cfg);
    handler->mDelegate = this;
    bus.addDevice(handler);
    return true;
  }

  const char *FORTE_ECDevice::init() {
    const auto productCode = static_cast<TForteUInt32>(Config().ProductCode);
    std::string errMsg;
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    if (!bus.esiConfigurator().validateDevice(productCode, errMsg)) {
      mLastError = errMsg;
      return mLastError.c_str();
    }
    return nullptr;
  }

  void FORTE_ECDevice::deInit() { 
    auto &bus = *static_cast<ECBusHandler *>(&getController()); 
    ECBusDeviceHandler *device = bus.getDevice(mIndex);
    if (device != nullptr && device->mDelegate == this) {
      device->mDelegate = nullptr;
    }
  }

  void FORTE_ECDevice::initHandles() {
    if (!QI()) {
      return;
    }
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    auto *device = static_cast<ECDeviceHandler *>(bus.getDevice(mIndex));
    if (nullptr == device) {
      return;
    }
    if (bus.isLoopPrepared()) {
      bus.esiConfigurator().remapDeviceIOHandles(static_cast<TForteUInt32>(Config().ProductCode), device, *this);
    } else {
      bus.esiConfigurator().initDeviceIOHandles(static_cast<TForteUInt32>(Config().ProductCode), device, *this);
    }
  }

  void FORTE_ECDevice::onDeviceStatus(ECBusDeviceHandler::DeviceStatus paStatus, ECBusDeviceHandler::DeviceStatus) {
    switch (paStatus) {
      case ECBusDeviceHandler::OK: STATUS() = scmOK; break;
      case ECBusDeviceHandler::Error: STATUS() = u"Error"_WSTRING; break;
      case ECBusDeviceHandler::NotInitialized: STATUS() = u"NotInitialized"_WSTRING; break;
      default: STATUS() = u"Unknown"_WSTRING; break;
    }
    sendOutputEvent(scmEventINDID, getEventChainExecutor());
  }

  void FORTE_ECDevice::onDeviceDestroy() {
    deInit();
    QO() = false_BOOL;
    STATUS() = u"Device destroyed"_WSTRING;
  }

} // namespace forte::eclipse4diac::io::ethercat
