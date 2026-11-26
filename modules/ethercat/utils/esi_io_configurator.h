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

namespace forte::eclipse4diac::io::ethercat {

  class ECDeviceHandler;
  class ECModuleHandler;
  class FORTE_ECDevice;
  class FORTE_ECModule;

  class EsiIoConfigurator {
    public:
      explicit EsiIoConfigurator(EsiCatalog &paCatalog);

      /** Adds the device's PDOs and entries to its model and allocates the process image; only the first call changes anything. */
      bool configureDevice(uint32_t paProductCode,
                           ECDeviceHandler *paDeviceHandler,
                           FORTE_ECDevice &paDevice,
                           std::string &paErrMsg);
      /** Like configureDevice, but adds the module's PDOs to the model of its parent device. */
      bool configureModule(uint32_t paModuleIdent,
                           ECDeviceHandler *paDeviceHandler,
                           ECModuleHandler *paModuleHandler,
                           FORTE_ECModule &paModule,
                           std::string &paErrMsg);

      void mapDeviceIOHandles(ECDeviceHandler *paDeviceHandler, FORTE_ECDevice &paDevice);
      void mapModuleIOHandles(ECModuleHandler *paModuleHandler, FORTE_ECModule &paModule);

    private:
      EsiCatalog &mCatalog;
  };

} // namespace forte::eclipse4diac::io::ethercat
