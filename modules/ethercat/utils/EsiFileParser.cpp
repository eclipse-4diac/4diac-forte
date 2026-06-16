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

#include "EsiFileParser.h"

#include "../handler/bus.h"
#include "../types/ECModule.h"
#include "../types/ECSlave.h"
#include "forte/util/string_utils.h"

#include <filesystem>
#include <string>
#include <cstring>
#include <tinyxml.h>
#include <unistd.h>

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

  DEFINE_SINGLETON(EsiFileParser)

  std::map<uint32_t, std::string> EsiFileParser::scmEsiFilePathMap;
  std::map<std::string, TiXmlDocument *> EsiFileParser::scmEsiFileDocMap;
  std::map<uint32_t, TiXmlElement *> EsiFileParser::scmDeviceOrModuleMap;
  std::map<uint32_t, std::string> EsiFileParser::scmVendorMap;

  EsiFileParser::EsiFileParser() {
    init();
  }

  EsiFileParser::~EsiFileParser() {
    for (auto &[_, doc] : scmEsiFileDocMap) {
      delete doc;
    }
  }

  uint32_t EsiFileParser::parseEcNumber(const char *paText) {
    if (nullptr == paText) {
      return 0;
    }
    if (strlen(paText) > 2 && paText[0] == '#' && (paText[1] == 'x' || paText[1] == 'X')) {
      return static_cast<uint32_t>(util::strtoul(paText + 2, nullptr, 16));
    }
    if (strlen(paText) > 2 && paText[0] == '0' && (paText[1] == 'x' || paText[1] == 'X')) {
      return static_cast<uint32_t>(util::strtoul(paText + 2, nullptr, 16));
    }
    return static_cast<uint32_t>(util::strtoul(paText, nullptr, 10));
  }

  void EsiFileParser::init() {
    char exePath[4096];
    const ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len <= 0) {
      return;
    }
    exePath[len] = '\0';
    char *lastSlash = strrchr(exePath, '/');
    if (nullptr == lastSlash) {
      return;
    }
    *lastSlash = '\0';
    const std::string esiFolder = std::string(exePath) + "/devices/";
    if (!std::filesystem::exists(esiFolder)) {
      DEVLOG_WARNING("ethercat[EsiFileParser]: ESI folder not found: %s\n", esiFolder.c_str());
      return;
    }

    for (const auto &entry : std::filesystem::directory_iterator(esiFolder)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      const std::string extension = entry.path().extension().string();
      if (!(extension == ".xml" || extension == ".XML")) {
        continue;
      }
      const std::string filePath = entry.path().string();
      auto *doc = new TiXmlDocument(filePath.c_str());
      if (!doc->LoadFile()) {
        delete doc;
        continue;
      }
      auto *root = doc->RootElement();
      if (nullptr == root) {
        delete doc;
        continue;
      }
      auto *descriptions = root->FirstChildElement("Descriptions");
      if (nullptr == descriptions) {
        delete doc;
        continue;
      }
      std::string vendorName;
      if (auto *vendor = root->FirstChildElement("Vendor")) {
        if (auto *name = vendor->FirstChildElement("Name"); name && name->GetText()) {
          vendorName = name->GetText();
        }
      }
      scmEsiFileDocMap[filePath] = doc;
      if (auto *devices = descriptions->FirstChildElement("Devices")) {
        for (auto *device = devices->FirstChildElement("Device"); device != nullptr;
             device = device->NextSiblingElement("Device")) {
          if (auto *type = device->FirstChildElement("Type")) {
            const char *pc = type->Attribute("ProductCode");
            if (pc) {
              const uint32_t key = parseEcNumber(pc);
              scmEsiFilePathMap[key] = filePath;
              scmVendorMap[key] = vendorName;
            }
          }
        }
      }
      if (auto *modules = descriptions->FirstChildElement("Modules")) {
        for (auto *module = modules->FirstChildElement("Module"); module != nullptr;
             module = module->NextSiblingElement("Module")) {
          if (auto *type = module->FirstChildElement("Type")) {
            const char *mi = type->Attribute("ModuleIdent");
            if (mi) {
              const uint32_t key = parseEcNumber(mi);
              scmEsiFilePathMap[key] = filePath;
              scmVendorMap[key] = vendorName;
            }
          }
        }
      }
    }
  }

  bool EsiFileParser::loadEsiFileByKey(uint32_t paKey, std::string &paErrMsg) {
    auto it = scmEsiFilePathMap.find(paKey);
    if (it == scmEsiFilePathMap.end()) {
      paErrMsg = std::string("No ESI file for key ") + std::to_string(paKey);
      return false;
    }
    return scmEsiFileDocMap.find(it->second) != scmEsiFileDocMap.end();
  }

  bool EsiFileParser::getDeviceFromDocByProductCode(TiXmlDocument *paDocument,
                                                     uint32_t paProductCode,
                                                     TiXmlElement *&paDeviceElement,
                                                     std::string &paErrMsg) {
    if (nullptr == paDocument || nullptr == paDocument->RootElement()) {
      paErrMsg = "Invalid ESI XML root";
      return false;
    }
    if (auto cached = scmDeviceOrModuleMap.find(paProductCode); cached != scmDeviceOrModuleMap.end()) {
      paDeviceElement = cached->second;
      return true;
    }
    auto *descriptions = paDocument->RootElement()->FirstChildElement("Descriptions");
    auto *devices = descriptions ? descriptions->FirstChildElement("Devices") : nullptr;
    if (nullptr == devices) {
      paErrMsg = "No Devices section in ESI";
      return false;
    }
    for (auto *device = devices->FirstChildElement("Device"); device; device = device->NextSiblingElement("Device")) {
      auto *type = device->FirstChildElement("Type");
      if (!type) {
        continue;
      }
      const char *pc = type->Attribute("ProductCode");
      if (pc && parseEcNumber(pc) == paProductCode) {
        scmDeviceOrModuleMap[paProductCode] = device;
        paDeviceElement = device;
        return true;
      }
    }
    paErrMsg = std::string("Device ProductCode ") + std::to_string(paProductCode) + " not found in ESI";
    return false;
  }

  bool EsiFileParser::getModuleFromDocByIdentity(TiXmlDocument *paDocument,
                                                 uint32_t paModuleIdent,
                                                 TiXmlElement *&paModuleElement,
                                                 std::string &paErrMsg) {
    if (nullptr == paDocument || nullptr == paDocument->RootElement()) {
      paErrMsg = "Invalid ESI XML root";
      return false;
    }
    if (auto cached = scmDeviceOrModuleMap.find(paModuleIdent); cached != scmDeviceOrModuleMap.end()) {
      paModuleElement = cached->second;
      return true;
    }
    auto *descriptions = paDocument->RootElement()->FirstChildElement("Descriptions");
    auto *modules = descriptions ? descriptions->FirstChildElement("Modules") : nullptr;
    if (nullptr == modules) {
      paErrMsg = "No Modules section in ESI";
      return false;
    }
    for (auto *module = modules->FirstChildElement("Module"); module; module = module->NextSiblingElement("Module")) {
      auto *type = module->FirstChildElement("Type");
      if (!type) {
        continue;
      }
      const char *mi = type->Attribute("ModuleIdent");
      if (mi && parseEcNumber(mi) == paModuleIdent) {
        scmDeviceOrModuleMap[paModuleIdent] = module;
        paModuleElement = module;
        return true;
      }
    }
    paErrMsg = std::string("ModuleIdent ") + std::to_string(paModuleIdent) + " not found in ESI";
    return false;
  }

  bool EsiFileParser::loadDevice(uint32_t paProductCode, std::string &paErrMsg) {
    if (!loadEsiFileByKey(paProductCode, paErrMsg)) {
      return false;
    }
    TiXmlElement *deviceElement = nullptr;
    return getDeviceFromDocByProductCode(scmEsiFileDocMap[scmEsiFilePathMap[paProductCode]],
                                         paProductCode,
                                         deviceElement,
                                         paErrMsg);
  }

  bool EsiFileParser::loadModule(uint32_t paModuleIdent, std::string &paErrMsg) {
    if (!loadEsiFileByKey(paModuleIdent, paErrMsg)) {
      return false;
    }
    TiXmlElement *moduleElement = nullptr;
    return getModuleFromDocByIdentity(scmEsiFileDocMap[scmEsiFilePathMap[paModuleIdent]],
                                      paModuleIdent,
                                      moduleElement,
                                      paErrMsg);
  }

  void EsiFileParser::getPdoSizeInfo(TiXmlElement *paElement,
                                     FORTE_ECSlave &paSlave,
                                     uint16_t &paRcvBufferSize,
                                     uint16_t &paSendBufferSize) {
    int inputIndex = 0;
    for (auto *tx = paElement->FirstChildElement("TxPdo"); tx; tx = tx->NextSiblingElement("TxPdo")) {
      if (!tx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = tx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paSlave.getDI(2 + inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paRcvBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }

    inputIndex = static_cast<int>(paSlave.numInMappings());
    for (auto *rx = paElement->FirstChildElement("RxPdo"); rx; rx = rx->NextSiblingElement("RxPdo")) {
      if (!rx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = rx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paSlave.getDI(2 + inputIndex++));
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

  void EsiFileParser::getPdoSizeInfo(TiXmlElement *paElement,
                                     FORTE_ECModule &paSlave,
                                     uint16_t &paRcvBufferSize,
                                     uint16_t &paSendBufferSize) {
    int inputIndex = 0;
    for (auto *tx = paElement->FirstChildElement("TxPdo"); tx; tx = tx->NextSiblingElement("TxPdo")) {
      if (!tx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = tx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paSlave.getDI(2 + inputIndex++));
        if (!idVar->getValue() || idVar->getValue()[0] == '\0') {
          continue;
        }
        const char *bitLen = entry->FirstChildElement("BitLen")->GetText();
        paRcvBufferSize += static_cast<uint16_t>(util::strtoul(bitLen, nullptr, 10));
      }
    }

    inputIndex = static_cast<int>(paSlave.numInMappings());
    for (auto *rx = paElement->FirstChildElement("RxPdo"); rx; rx = rx->NextSiblingElement("RxPdo")) {
      if (!rx->Attribute("Sm")) {
        continue;
      }
      for (auto *entry = rx->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paSlave.getDI(2 + inputIndex++));
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

  void EsiFileParser::parseDevicePdo(const std::string &paPdoType,
                                     TiXmlElement *paElement,
                                     ECDeviceHandler *paDeviceHandler,
                                     FORTE_ECSlave &paSlave) {
    const SyncDir dir = (paPdoType == "RxPdo") ? SyncDir::Out : SyncDir::In;
    int diIndex = (dir == SyncDir::In) ? 2 : 2 + static_cast<int>(paSlave.numInMappings());
    int handleIndex = 0;
    int entryIndexInInterface = 0;
    int offset = 0;
    for (auto *pdoElement = paElement->FirstChildElement(paPdoType.c_str()); pdoElement; pdoElement = pdoElement->NextSiblingElement(paPdoType.c_str())) {
      if (!pdoElement->Attribute("Sm")) {
        continue;
      }
      const char *indexStr = pdoElement->FirstChildElement("Index")->GetText();
      const uint16_t pdoIndex = static_cast<uint16_t>(parseEcNumber(indexStr));
      paDeviceHandler->mECDeviceModel.addPdo(pdoIndex, dir);

      for (auto *entry = pdoElement->FirstChildElement("Entry"); entry; entry = entry->NextSiblingElement("Entry")) {
        const char *entryIndexText = entry->FirstChildElement("Index")->GetText();
        const uint16_t entryIndex = static_cast<uint16_t>(parseEcNumber(entryIndexText));
        const uint8_t subIndex =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("SubIndex")->GetText(), nullptr, 10));
        const uint8_t bitLen =
            static_cast<uint8_t>(util::strtoul(entry->FirstChildElement("BitLen")->GetText(), nullptr, 10));
        paDeviceHandler->mECDeviceModel.addPdoEntry(pdoIndex, entryIndex, subIndex, bitLen);

        auto *idVar = static_cast<CIEC_WSTRING *>(paSlave.getDI(diIndex + entryIndexInInterface++));
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
        paSlave.initHandle(desc);
        offset += bitLen / 8;

        ECSlaveHandle *handle = (dir == SyncDir::In) ? paDeviceHandler->getInputHandle(handleIndex)
                                                      : paDeviceHandler->getOutputHandle(handleIndex);
        if (handle) {
          paDeviceHandler->mECDeviceModel.addEntryReg(entryIndex, subIndex, handle->ecDomainOffsetPtr());
        }
        handleIndex++;
      }
    }
  }

  void EsiFileParser::parseModulePdo(const std::string &paPdoType,
                                     TiXmlElement *paElement,
                                     ECDeviceHandler *paDeviceHandler,
                                     ECModuleHandler *paModuleHandler,
                                     FORTE_ECModule &paSlave) {
    std::string vendorName;
    if (auto vit = scmVendorMap.find(paModuleHandler->moduleIdent()); vit != scmVendorMap.end()) {
      vendorName = vit->second;
    }

    const SyncDir dir = (paPdoType == "RxPdo") ? SyncDir::Out : SyncDir::In;
    int diIndex = (dir == SyncDir::In) ? 2 : 2 + static_cast<int>(paSlave.numInMappings());
    int handleIndex = 0;
    int entryIndexInInterface = 0;
    int offset = 0;
    for (auto *pdoElement = paElement->FirstChildElement(paPdoType.c_str()); pdoElement; pdoElement = pdoElement->NextSiblingElement(paPdoType.c_str())) {
      if (!pdoElement->Attribute("Sm")) {
        continue;
      }
      uint16_t pdoIndex = static_cast<uint16_t>(parseEcNumber(pdoElement->FirstChildElement("Index")->GetText()));
      pdoIndex = static_cast<uint16_t>(pdoIndex + paDeviceHandler->mECDeviceModel.mSlotPdoInc * paModuleHandler->slot());
      paDeviceHandler->mECDeviceModel.addPdo(pdoIndex, dir);

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
        paDeviceHandler->mECDeviceModel.addPdoEntry(pdoIndex, entryIndex, subIndex, bitLen);

        auto *idVar = static_cast<CIEC_WSTRING *>(paSlave.getDI(diIndex + entryIndexInInterface++));
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
        paSlave.initHandle(desc);
        offset += bitLen / 8;

        ECSlaveHandle *handle = (dir == SyncDir::In) ? paModuleHandler->getInputHandle(handleIndex)
                                                      : paModuleHandler->getOutputHandle(handleIndex);
        if (handle) {
          paDeviceHandler->mECDeviceModel.addEntryReg(entryIndex, subIndex, handle->ecDomainOffsetPtr());
        }
        handleIndex++;
      }
    }
  }

  void EsiFileParser::initDeviceIOHandles(uint32_t paProductCode, ECDeviceHandler *paDeviceHandler, FORTE_ECSlave &paSlave) {
    auto it = scmDeviceOrModuleMap.find(paProductCode);
    if (it == scmDeviceOrModuleMap.end()) {
      return;
    }
    TiXmlElement *deviceElement = it->second;
    if (paDeviceHandler->mSlaveType == ECSlaveHandler::SlaveType::ECCoupler) {
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
    getPdoSizeInfo(deviceElement, paSlave, recvSize, sendSize);
    paDeviceHandler->initBuffer(sendSize, recvSize);
    parseDevicePdo("TxPdo", deviceElement, paDeviceHandler, paSlave);
    parseDevicePdo("RxPdo", deviceElement, paDeviceHandler, paSlave);
  }

  void EsiFileParser::initModuleIOHandles(uint32_t paModuleIdent,
                                          ECDeviceHandler *paDeviceHandler,
                                          ECModuleHandler *paModuleHandler,
                                          FORTE_ECModule &paSlave) {
    auto it = scmDeviceOrModuleMap.find(paModuleIdent);
    if (it == scmDeviceOrModuleMap.end()) {
      return;
    }
    TiXmlElement *moduleElement = it->second;
    uint16_t recvSize = 0;
    uint16_t sendSize = 0;
    getPdoSizeInfo(moduleElement, paSlave, recvSize, sendSize);
    paModuleHandler->initBuffer(sendSize, recvSize);
    parseModulePdo("TxPdo", moduleElement, paDeviceHandler, paModuleHandler, paSlave);
    parseModulePdo("RxPdo", moduleElement, paDeviceHandler, paModuleHandler, paSlave);
  }
} // namespace forte::eclipse4diac::io::ethercat

