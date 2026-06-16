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

#pragma once

#include "../slave/ec_device.h"
#include "../slave/ec_module.h"
#include "forte/util/singlet.h"

#include <map>
#include <string>

class TiXmlDocument;
class TiXmlElement;

namespace forte::eclipse4diac::io::ethercat {

  class FORTE_ECSlave;
  class FORTE_ECModule;

  class EsiFileParser {
      DECLARE_SINGLETON(EsiFileParser)

    public:
      bool loadDevice(uint32_t paProductCode, std::string &paErrMsg);
      bool loadModule(uint32_t paModuleIdent, std::string &paErrMsg);

      void initDeviceIOHandles(uint32_t paProductCode, ECDeviceHandler *paDeviceHandler, FORTE_ECSlave &paSlave);
      void initModuleIOHandles(uint32_t paModuleIdent,
                               ECDeviceHandler *paDeviceHandler,
                               ECModuleHandler *paModuleHandler,
                               FORTE_ECModule &paSlave);

    private:
      bool loadEsiFileByKey(uint32_t paKey, std::string &paErrMsg);
      void init();

      bool getDeviceFromDocByProductCode(TiXmlDocument *paDocument,
                                         uint32_t paProductCode,
                                         TiXmlElement *&paDeviceElement,
                                         std::string &paErrMsg);
      bool getModuleFromDocByIdentity(TiXmlDocument *paDocument,
                                      uint32_t paModuleIdent,
                                      TiXmlElement *&paModuleElement,
                                      std::string &paErrMsg);

      void parseDevicePdo(const std::string &paPdoType,
                          TiXmlElement *paDeviceElement,
                          ECDeviceHandler *paDeviceHandler,
                          FORTE_ECSlave &paSlave);
      void parseModulePdo(const std::string &paPdoType,
                          TiXmlElement *paModuleElement,
                          ECDeviceHandler *paDeviceHandler,
                          ECModuleHandler *paModuleHandler,
                          FORTE_ECModule &paSlave);

      void getPdoSizeInfo(TiXmlElement *paElement,
                          FORTE_ECSlave &paSlave,
                          uint16_t &paRcvBufferSize,
                          uint16_t &paSendBufferSize);

      void getPdoSizeInfo(TiXmlElement *paElement,
                          FORTE_ECModule &paSlave,
                          uint16_t &paRcvBufferSize,
                          uint16_t &paSendBufferSize);

      static uint32_t parseEcNumber(const char *paText);

      static std::map<uint32_t, std::string> scmEsiFilePathMap;
      static std::map<std::string, TiXmlDocument *> scmEsiFileDocMap;
      static std::map<uint32_t, TiXmlElement *> scmDeviceOrModuleMap;
      static std::map<uint32_t, std::string> scmVendorMap;
  };

} // namespace forte::eclipse4diac::io::ethercat

