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

#include "esi_io_configurator.h"

#include "../device/ec_device.h"
#include "../device/ec_module.h"
#include "../handler/bus.h"
#include "../types/ECDevice.h"
#include "../types/ECModule.h"
#include "esi_xml_utils.h"
#include "forte/util/string_utils.h"

#include <string>
#include <tinyxml.h>

namespace forte::eclipse4diac::io::ethercat {

  namespace {

    void applyVendorModulePdoEntryIndexCorrection(const std::string &paVendorName, uint16_t &paEntryIndex) {
      if (paVendorName == "SiChuan Odot Automation System Co.,Ltd.") {
        paEntryIndex = static_cast<uint16_t>(paEntryIndex + 1U);
      } else if (paVendorName == "Inovance") {
        paEntryIndex = static_cast<uint16_t>(paEntryIndex - 1U);
      }
    }

  } // namespace

  EsiIoConfigurator::EsiIoConfigurator(EsiCatalog &paCatalog) : mCatalog(paCatalog) {}

  bool EsiIoConfigurator::validateDevice(uint32_t paProductCode, std::string &paErrMsg) {
    return mCatalog.validateDevice(paProductCode, paErrMsg);
  }

  bool EsiIoConfigurator::validateModule(uint32_t paModuleIdent, std::string &paErrMsg) {
    return mCatalog.validateModule(paModuleIdent, paErrMsg);
  }

  void EsiIoConfigurator::getPdoSizeInfo(TiXmlElement *paElement,
                                           FORTE_ECDevice &paDevice,
                                           uint16_t &paRcvBufferSize,
                                           uint16_t &paSendBufferSize) {
    int inputIndex = 0;
    for (auto *tx = paElement->FirstChildElement("TxPdo"); tx; tx = tx->NextSiblingElement("TxPdo")) {
      if (!tx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = tx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paDevice.mappingDi(inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paRcvBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }

    inputIndex = static_cast<int>(paDevice.numInMappings());
    for (auto *rx = paElement->FirstChildElement("RxPdo"); rx; rx = rx->NextSiblingElement("RxPdo")) {
      if (!rx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = rx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paDevice.mappingDi(inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paSendBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }
    paRcvBufferSize /= 8;
    paSendBufferSize /= 8;
  }

  void EsiIoConfigurator::getPdoSizeInfo(TiXmlElement *paElement,
                                           FORTE_ECModule &paDevice,
                                           uint16_t &paRcvBufferSize,
                                           uint16_t &paSendBufferSize) {
    int inputIndex = 0;
    for (auto *tx = paElement->FirstChildElement("TxPdo"); tx; tx = tx->NextSiblingElement("TxPdo")) {
      if (!tx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = tx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paDevice.mappingDi(inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paRcvBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }

    inputIndex = static_cast<int>(paDevice.numInMappings());
    for (auto *rx = paElement->FirstChildElement("RxPdo"); rx; rx = rx->NextSiblingElement("RxPdo")) {
      if (!rx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = rx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paDevice.mappingDi(inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paSendBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }
    paRcvBufferSize /= 8;
    paSendBufferSize /= 8;
  }

  void EsiIoConfigurator::parseDevicePdo(const std::string &paPdoType,
                                         TiXmlElement *paElement,
                                         ECDeviceHandler *paDeviceHandler,
                                         FORTE_ECDevice &paDevice,
                                         bool paRemapOnly) {
    const SyncDir dir = (paPdoType == "RxPdo") ? SyncDir::Out : SyncDir::In;
    int diIndex = (dir == SyncDir::In) ? 0 : static_cast<int>(paDevice.numInMappings());
    int handleIndex = 0;
    int entryIndexInInterface = 0;
    int offset = 0;
    for (auto *pdoElement = paElement->FirstChildElement(paPdoType.c_str()); pdoElement;
         pdoElement = pdoElement->NextSiblingElement(paPdoType.c_str())) {
      if (!pdoElement->Attribute("Sm")) {
        continue;
      }
      const char *indexStr = pdoElement->FirstChildElement("Index")->GetText();
      const uint16_t pdoIndex = static_cast<uint16_t>(parseEcNumber(indexStr));
      if (!paRemapOnly) {
        paDeviceHandler->mECDeviceModel.addPdo(pdoIndex, dir);
      }

      for (auto *entry = pdoElement->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const char *entryIndexText = entry->FirstChildElement("Index")->GetText();
        const uint16_t entryIndex = static_cast<uint16_t>(parseEcNumber(entryIndexText));
        const uint8_t subIndex =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("SubIndex")->GetText(), nullptr, 10));
        const uint8_t bitLen =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("BitLen")->GetText(), nullptr, 10));
        if (!paRemapOnly) {
          paDeviceHandler->mECDeviceModel.addPdoEntry(pdoIndex, entryIndex, subIndex, bitLen);
        }

        auto *idVar = static_cast<CIEC_WSTRING *>(paDevice.mappingDi(diIndex + entryIndexInInterface++));
        const char *idText = idVar->getValue();
        if (nullptr == idText || idText[0] == '\0') {
          continue;
        }

        std::string handleId{idText};
        ECBusHandler::HandleDescriptor desc(handleId,
                                            dir == SyncDir::In ? forte::io::IOMapper::Direction::In
                                                               : forte::io::IOMapper::Direction::Out,
                                            paDeviceHandler->index(),
                                            static_cast<uint8_t>(offset),
                                            static_cast<uint8_t>(bitLen / 8));
        paDevice.registerMappedHandle(desc);
        offset += bitLen / 8;

        ECDeviceHandle *handle = (dir == SyncDir::In) ? paDeviceHandler->getInputHandle(handleIndex)
                                                      : paDeviceHandler->getOutputHandle(handleIndex);
        if (handle) {
          if (paRemapOnly) {
            paDeviceHandler->mECDeviceModel.restoreHandleDomainOffset(entryIndex, subIndex, handle->ecDomainOffsetPtr());
          } else {
            paDeviceHandler->mECDeviceModel.addEntryReg(entryIndex, subIndex, handle->ecDomainOffsetPtr());
          }
        }
        handleIndex++;
      }
    }
  }

  void EsiIoConfigurator::parseModulePdo(const std::string &paPdoType,
                                         TiXmlElement *paElement,
                                         ECDeviceHandler *paDeviceHandler,
                                         ECModuleHandler *paModuleHandler,
                                         FORTE_ECModule &paDevice,
                                         bool paRemapOnly) {
    const std::string &vendorName = mCatalog.vendorForModule(paModuleHandler->moduleIdent());

    const SyncDir dir = (paPdoType == "RxPdo") ? SyncDir::Out : SyncDir::In;
    int diIndex = (dir == SyncDir::In) ? 0 : static_cast<int>(paDevice.numInMappings());
    int handleIndex = 0;
    int entryIndexInInterface = 0;
    int offset = 0;
    for (auto *pdoElement = paElement->FirstChildElement(paPdoType.c_str()); pdoElement;
         pdoElement = pdoElement->NextSiblingElement(paPdoType.c_str())) {
      if (!pdoElement->Attribute("Sm")) {
        continue;
      }
      uint16_t pdoIndex = static_cast<uint16_t>(parseEcNumber(pdoElement->FirstChildElement("Index")->GetText()));
      pdoIndex = static_cast<uint16_t>(pdoIndex + paDeviceHandler->mECDeviceModel.mSlotPdoInc * paModuleHandler->slot());
      if (!paRemapOnly) {
        paDeviceHandler->mECDeviceModel.addPdo(pdoIndex, dir);
      }

      for (auto *entry = pdoElement->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const char *entryIndexText = entry->FirstChildElement("Index")->GetText();
        uint16_t entryIndex = static_cast<uint16_t>(parseEcNumber(entryIndexText));
        const uint8_t subIndex =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("SubIndex")->GetText(), nullptr, 10));
        const uint8_t bitLen =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("BitLen")->GetText(), nullptr, 10));
        applyVendorModulePdoEntryIndexCorrection(vendorName, entryIndex);
        entryIndex =
            static_cast<uint16_t>(entryIndex + paDeviceHandler->mECDeviceModel.mSlotIndexInc * paModuleHandler->slot());
        if (!paRemapOnly) {
          paDeviceHandler->mECDeviceModel.addPdoEntry(pdoIndex, entryIndex, subIndex, bitLen);
        }

        auto *idVar = static_cast<CIEC_WSTRING *>(paDevice.mappingDi(diIndex + entryIndexInInterface++));
        const char *idText = idVar->getValue();
        if (nullptr == idText || idText[0] == '\0') {
          continue;
        }

        std::string handleId{idText};
        ECBusHandler::HandleDescriptor desc(handleId,
                                            dir == SyncDir::In ? forte::io::IOMapper::Direction::In
                                                               : forte::io::IOMapper::Direction::Out,
                                            paModuleHandler->index(),
                                            static_cast<uint8_t>(offset),
                                            static_cast<uint8_t>(bitLen / 8));
        paDevice.registerMappedHandle(desc);
        offset += bitLen / 8;

        ECDeviceHandle *handle = (dir == SyncDir::In) ? paModuleHandler->getInputHandle(handleIndex)
                                                      : paModuleHandler->getOutputHandle(handleIndex);
        if (handle) {
          if (paRemapOnly) {
            paDeviceHandler->mECDeviceModel.restoreHandleDomainOffset(entryIndex, subIndex, handle->ecDomainOffsetPtr());
          } else {
            paDeviceHandler->mECDeviceModel.addEntryReg(entryIndex, subIndex, handle->ecDomainOffsetPtr());
          }
        }
        handleIndex++;
      }
    }
  }

  void EsiIoConfigurator::initDeviceIOHandles(uint32_t paProductCode,
                                              ECDeviceHandler *paDeviceHandler,
                                              FORTE_ECDevice &paDevice) {
    std::string errMsg;
    TiXmlElement *deviceElement = mCatalog.resolveDevice(paProductCode, errMsg);
    if (nullptr == deviceElement) {
      return;
    }

    if (paDeviceHandler->mDeviceType == ECBusDeviceHandler::DeviceType::ECCoupler) {
      if (auto *slots = deviceElement->FirstChildElement("Slots")) {
        paDeviceHandler->mECDeviceModel.mSlotIndexInc =
            static_cast<uint32_t>(util::strtoul(slots->Attribute("SlotIndexIncrement"), nullptr, 10));
        paDeviceHandler->mECDeviceModel.mSlotPdoInc =
            static_cast<uint32_t>(util::strtoul(slots->Attribute("SlotPdoIncrement"), nullptr, 10));
      }
    }

    uint8_t smIndex = 0;
    for (auto *sm = deviceElement->FirstChildElement("Sm"); sm; sm = sm->NextSiblingElement("Sm")) {
      if (!sm->GetText()) {
        smIndex++;
        continue;
      }
      const std::string smText = sm->GetText();
      if (smText == "Outputs" || smText == "Inputs") {
        paDeviceHandler->mECDeviceModel.addSync(smIndex, smText == "Outputs" ? SyncDir::Out : SyncDir::In);
      }
      smIndex++;
    }

    uint16_t recvSize = 0;
    uint16_t sendSize = 0;
    getPdoSizeInfo(deviceElement, paDevice, recvSize, sendSize);
    paDeviceHandler->initBuffer(sendSize, recvSize);
    parseDevicePdo("TxPdo", deviceElement, paDeviceHandler, paDevice);
    parseDevicePdo("RxPdo", deviceElement, paDeviceHandler, paDevice);
  }

  void EsiIoConfigurator::initModuleIOHandles(uint32_t paModuleIdent,
                                              ECDeviceHandler *paDeviceHandler,
                                              ECModuleHandler *paModuleHandler,
                                              FORTE_ECModule &paDevice) {
    std::string errMsg;
    TiXmlElement *moduleElement = mCatalog.resolveModule(paModuleIdent, errMsg);
    if (nullptr == moduleElement) {
      return;
    }

    uint16_t recvSize = 0;
    uint16_t sendSize = 0;
    getPdoSizeInfo(moduleElement, paDevice, recvSize, sendSize);
    paModuleHandler->initBuffer(sendSize, recvSize);
    parseModulePdo("TxPdo", moduleElement, paDeviceHandler, paModuleHandler, paDevice);
    parseModulePdo("RxPdo", moduleElement, paDeviceHandler, paModuleHandler, paDevice);
  }

  void EsiIoConfigurator::remapDeviceIOHandles(uint32_t paProductCode,
                                               ECDeviceHandler *paDeviceHandler,
                                               FORTE_ECDevice &paDevice) {
    std::string errMsg;
    TiXmlElement *deviceElement = mCatalog.resolveDevice(paProductCode, errMsg);
    if (nullptr == deviceElement) {
      return;
    }
    parseDevicePdo("TxPdo", deviceElement, paDeviceHandler, paDevice, true);
    parseDevicePdo("RxPdo", deviceElement, paDeviceHandler, paDevice, true);
  }

  void EsiIoConfigurator::remapModuleIOHandles(uint32_t paModuleIdent,
                                               ECDeviceHandler *paDeviceHandler,
                                               ECModuleHandler *paModuleHandler,
                                               FORTE_ECModule &paDevice) {
    std::string errMsg;
    TiXmlElement *moduleElement = mCatalog.resolveModule(paModuleIdent, errMsg);
    if (nullptr == moduleElement) {
      return;
    }
    parseModulePdo("TxPdo", moduleElement, paDeviceHandler, paModuleHandler, paDevice, true);
    parseModulePdo("RxPdo", moduleElement, paDeviceHandler, paModuleHandler, paDevice, true);
  }

} // namespace forte::eclipse4diac::io::ethercat
