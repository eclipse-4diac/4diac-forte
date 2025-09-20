/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0

 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/io/configFB/io_configFB_controller.h"
#include "forte/util/extevhandlerhelper.h"

class FORTE_ZephyrIOBase : public forte::io::IOConfigFBController {

  protected:
    using IOHandle = forte::io::IOHandle;
    using IOMapper = forte::io::IOMapper;

    static constexpr size_t initialDIOffset = 1;

    void onStartup(CEventChainExecutionThread *const paECET) override;

  private:
    static constexpr size_t numberOfIOs = 0;

    forte::io::IODeviceController *createDeviceController(CDeviceExecution &paDeviceExecution) override;

  public:
    FORTE_ZephyrIOBase(CFBContainer &paContainer,
                       const SFBInterfaceSpec &paInterfaceSpec,
                       const forte::StringId paInstanceNameId);
    ~FORTE_ZephyrIOBase() override;
};
