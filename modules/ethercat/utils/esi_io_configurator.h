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

#include "esi_catalog.h"

#include <cstdint>
#include <string>

class TiXmlElement;

namespace forte::eclipse4diac::io::ethercat {

  class ECDeviceHandler;
  class ECModuleHandler;
  class FORTE_ECDevice;
  class FORTE_ECModule;

  class EsiIoConfigurator {
    public:
      explicit EsiIoConfigurator(EsiCatalog &paCatalog);

      bool validateDevice(uint32_t paProductCode, std::string &paErrMsg);
      bool validateModule(uint32_t paModuleIdent, std::string &paErrMsg);

      void initDeviceIOHandles(uint32_t paProductCode, ECDeviceHandler *paDeviceHandler, FORTE_ECDevice &paDevice);
      void initModuleIOHandles(uint32_t paModuleIdent,
                               ECDeviceHandler *paDeviceHandler,
                               ECModuleHandler *paModuleHandler,
                               FORTE_ECModule &paDevice);

      void remapDeviceIOHandles(uint32_t paProductCode, ECDeviceHandler *paDeviceHandler, FORTE_ECDevice &paDevice);
      void remapModuleIOHandles(uint32_t paModuleIdent,
                                ECDeviceHandler *paDeviceHandler,
                                ECModuleHandler *paModuleHandler,
                                FORTE_ECModule &paDevice);

    private:
      void getPdoSizeInfo(TiXmlElement *paElement,
                          FORTE_ECDevice &paDevice,
                          uint16_t &paRcvBufferSize,
                          uint16_t &paSendBufferSize);
      void getPdoSizeInfo(TiXmlElement *paElement,
                          FORTE_ECModule &paDevice,
                          uint16_t &paRcvBufferSize,
                          uint16_t &paSendBufferSize);

      void parseDevicePdo(const std::string &paPdoType,
                          TiXmlElement *paElement,
                          ECDeviceHandler *paDeviceHandler,
                          FORTE_ECDevice &paDevice,
                          bool paRemapOnly = false);
      void parseModulePdo(const std::string &paPdoType,
                          TiXmlElement *paElement,
                          ECDeviceHandler *paDeviceHandler,
                          ECModuleHandler *paModuleHandler,
                          FORTE_ECModule &paDevice,
                          bool paRemapOnly = false);

      EsiCatalog &mCatalog;
  };

} // namespace forte::eclipse4diac::io::ethercat
