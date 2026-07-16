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

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

class TiXmlDocument;
class TiXmlElement;

namespace forte::eclipse4diac::io::ethercat {

  class EsiCatalog {
    public:
      EsiCatalog();
      ~EsiCatalog();

      EsiCatalog(const EsiCatalog &) = delete;
      EsiCatalog &operator=(const EsiCatalog &) = delete;

      bool validateDevice(uint32_t paProductCode, std::string &paErrMsg);
      bool validateModule(uint32_t paModuleIdent, std::string &paErrMsg);

      TiXmlElement *resolveDevice(uint32_t paProductCode, std::string &paErrMsg);
      TiXmlElement *resolveModule(uint32_t paModuleIdent, std::string &paErrMsg);

      const std::string &vendorForDevice(uint32_t paProductCode) const;
      const std::string &vendorForModule(uint32_t paModuleIdent) const;

    private:
      void buildIndex();
      void indexEsiFile(const std::string &paFilePath);
      TiXmlDocument *loadDocument(const std::string &paFilePath);
      bool findDeviceInDocument(TiXmlDocument *paDocument,
                                uint32_t paProductCode,
                                TiXmlElement *&paDeviceElement,
                                std::string &paErrMsg);
      bool findModuleInDocument(TiXmlDocument *paDocument,
                                uint32_t paModuleIdent,
                                TiXmlElement *&paModuleElement,
                                std::string &paErrMsg);

      std::string mDevicesFolder;
      std::map<uint32_t, std::string> mDeviceKeyToPath;
      std::map<uint32_t, std::string> mModuleKeyToPath;
      std::map<uint32_t, std::string> mDeviceVendorMap;
      std::map<uint32_t, std::string> mModuleVendorMap;
      std::map<std::string, std::unique_ptr<TiXmlDocument>> mDocuments;
      std::map<uint32_t, TiXmlElement *> mDeviceElements;
      std::map<uint32_t, TiXmlElement *> mModuleElements;
  };

} // namespace forte::eclipse4diac::io::ethercat
