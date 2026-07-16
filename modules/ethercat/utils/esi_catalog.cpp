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

#include "esi_catalog.h"
#include "esi_xml_utils.h"

#include "forte/util/devlog.h"

#include <cstring>
#include <filesystem>
#include <tinyxml.h>
#include <unistd.h>

namespace forte::eclipse4diac::io::ethercat {

  namespace {
    const std::string scmEmptyVendor;

    std::string resolveDevicesFolder() {
      char exePath[4096];
      const ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
      if (len <= 0) {
        return {};
      }
      exePath[len] = '\0';
      char *lastSlash = strrchr(exePath, '/');
      if (nullptr == lastSlash) {
        return {};
      }
      *lastSlash = '\0';
      return std::string(exePath) + "/devices/";
    }
  } // namespace

  EsiCatalog::EsiCatalog() {
    mDevicesFolder = resolveDevicesFolder();
    buildIndex();
  }

  EsiCatalog::~EsiCatalog() = default;

  void EsiCatalog::buildIndex() {
    if (mDevicesFolder.empty() || !std::filesystem::exists(mDevicesFolder)) {
      if (!mDevicesFolder.empty()) {
        DEVLOG_WARNING("ethercat[EsiCatalog]: ESI folder not found: %s\n", mDevicesFolder.c_str());
      }
      return;
    }

    for (const auto &entry : std::filesystem::directory_iterator(mDevicesFolder)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      const std::string extension = entry.path().extension().string();
      if (!(extension == ".xml" || extension == ".XML")) {
        continue;
      }
      indexEsiFile(entry.path().string());
    }
  }

  void EsiCatalog::indexEsiFile(const std::string &paFilePath) {
    TiXmlDocument doc(paFilePath.c_str());
    if (!doc.LoadFile()) {
      return;
    }
    auto *root = doc.RootElement();
    if (nullptr == root) {
      return;
    }
    auto *descriptions = root->FirstChildElement("Descriptions");
    if (nullptr == descriptions) {
      return;
    }

    std::string vendorName;
    if (auto *vendor = root->FirstChildElement("Vendor")) {
      if (auto *name = vendor->FirstChildElement("Name"); name && name->GetText()) {
        vendorName = name->GetText();
      }
    }

    if (auto *devices = descriptions->FirstChildElement("Devices")) {
      for (auto *device = devices->FirstChildElement("Device"); device != nullptr;
           device = device->NextSiblingElement("Device")) {
        if (auto *type = device->FirstChildElement("Type")) {
          const char *pc = type->Attribute("ProductCode");
          if (pc) {
            const uint32_t key = parseEcNumber(pc);
            mDeviceKeyToPath[key] = paFilePath;
            mDeviceVendorMap[key] = vendorName;
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
            mModuleKeyToPath[key] = paFilePath;
            mModuleVendorMap[key] = vendorName;
          }
        }
      }
    }
  }

  TiXmlDocument *EsiCatalog::loadDocument(const std::string &paFilePath) {
    if (auto existing = mDocuments.find(paFilePath); existing != mDocuments.end()) {
      return existing->second.get();
    }
    auto doc = std::make_unique<TiXmlDocument>(paFilePath.c_str());
    if (!doc->LoadFile()) {
      return nullptr;
    }
    auto *rawDoc = doc.get();
    mDocuments.emplace(paFilePath, std::move(doc));
    return rawDoc;
  }

  bool EsiCatalog::findDeviceInDocument(TiXmlDocument *paDocument,
                                          uint32_t paProductCode,
                                          TiXmlElement *&paDeviceElement,
                                          std::string &paErrMsg) {
    if (nullptr == paDocument || nullptr == paDocument->RootElement()) {
      paErrMsg = "Invalid ESI XML root";
      return false;
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
        paDeviceElement = device;
        return true;
      }
    }
    paErrMsg = std::string("Device ProductCode ") + std::to_string(paProductCode) + " not found in ESI";
    return false;
  }

  bool EsiCatalog::findModuleInDocument(TiXmlDocument *paDocument,
                                        uint32_t paModuleIdent,
                                        TiXmlElement *&paModuleElement,
                                        std::string &paErrMsg) {
    if (nullptr == paDocument || nullptr == paDocument->RootElement()) {
      paErrMsg = "Invalid ESI XML root";
      return false;
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
        paModuleElement = module;
        return true;
      }
    }
    paErrMsg = std::string("ModuleIdent ") + std::to_string(paModuleIdent) + " not found in ESI";
    return false;
  }

  TiXmlElement *EsiCatalog::resolveDevice(uint32_t paProductCode, std::string &paErrMsg) {
    if (auto cached = mDeviceElements.find(paProductCode); cached != mDeviceElements.end()) {
      return cached->second;
    }

    auto pathIt = mDeviceKeyToPath.find(paProductCode);
    if (pathIt == mDeviceKeyToPath.end()) {
      paErrMsg = std::string("No ESI file for ProductCode ") + std::to_string(paProductCode);
      return nullptr;
    }

    TiXmlDocument *doc = loadDocument(pathIt->second);
    if (nullptr == doc) {
      paErrMsg = std::string("Failed to load ESI file: ") + pathIt->second;
      return nullptr;
    }

    TiXmlElement *deviceElement = nullptr;
    if (!findDeviceInDocument(doc, paProductCode, deviceElement, paErrMsg)) {
      return nullptr;
    }

    mDeviceElements[paProductCode] = deviceElement;
    return deviceElement;
  }

  TiXmlElement *EsiCatalog::resolveModule(uint32_t paModuleIdent, std::string &paErrMsg) {
    if (auto cached = mModuleElements.find(paModuleIdent); cached != mModuleElements.end()) {
      return cached->second;
    }

    auto pathIt = mModuleKeyToPath.find(paModuleIdent);
    if (pathIt == mModuleKeyToPath.end()) {
      paErrMsg = std::string("No ESI file for ModuleIdent ") + std::to_string(paModuleIdent);
      return nullptr;
    }

    TiXmlDocument *doc = loadDocument(pathIt->second);
    if (nullptr == doc) {
      paErrMsg = std::string("Failed to load ESI file: ") + pathIt->second;
      return nullptr;
    }

    TiXmlElement *moduleElement = nullptr;
    if (!findModuleInDocument(doc, paModuleIdent, moduleElement, paErrMsg)) {
      return nullptr;
    }

    mModuleElements[paModuleIdent] = moduleElement;
    return moduleElement;
  }

  bool EsiCatalog::validateDevice(uint32_t paProductCode, std::string &paErrMsg) {
    return resolveDevice(paProductCode, paErrMsg) != nullptr;
  }

  bool EsiCatalog::validateModule(uint32_t paModuleIdent, std::string &paErrMsg) {
    return resolveModule(paModuleIdent, paErrMsg) != nullptr;
  }

  const std::string &EsiCatalog::vendorForDevice(uint32_t paProductCode) const {
    auto it = mDeviceVendorMap.find(paProductCode);
    return it != mDeviceVendorMap.end() ? it->second : scmEmptyVendor;
  }

  const std::string &EsiCatalog::vendorForModule(uint32_t paModuleIdent) const {
    auto it = mModuleVendorMap.find(paModuleIdent);
    return it != mModuleVendorMap.end() ? it->second : scmEmptyVendor;
  }

} // namespace forte::eclipse4diac::io::ethercat
