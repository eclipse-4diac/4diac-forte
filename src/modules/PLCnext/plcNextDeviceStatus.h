/*******************************************************************************
 * Copyright (c) 2022 Peirlberger Juergen
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Peirlberger Juergen - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_PLCNEXT_DEVICESTATUS_H_
#define SRC_MODULES_PLCNEXT_DEVICESTATUS_H_

// PLCnext includes
#include "Arp/System/ModuleLib/Module.h"
#include "Arp/System/Commons/Logging.h"
#include "Arp/Plc/AnsiC/Gds/DataLayout.h"
#include "Arp/Plc/AnsiC/Io/FbIoSystem.h"
#include "Arp/Plc/AnsiC/Io/Axio.h"
#include "Arp/Plc/AnsiC/Domain/PlcOperationHandler.h"
#include "Arp/System/Rsc/ServiceManager.hpp"
#include "Arp/Device/Interface/Services/IDeviceStatusService.hpp"


using namespace Arp;
using namespace Arp::System::Rsc;
using namespace Arp::Device::Interface::Services;
using namespace Arp::System::Commons::Diagnostics::Logging;

class DeviceStatus {
  public:

    /*
     * PLCnext DeviceStatus:
     * started: if PLC is able to handle a connection
     * ready: if PLC is successfully initialized and ready to read/write.
     */
    static volatile bool started;
    static volatile bool ready;

    static bool startup();
    static bool isReady();
    static void plcCallbackOperationHandler(enum PlcOperation operation);

  private:
    DeviceStatus() {}
};

#endif /* SRC_MODULES_PLCNEXT_DEVICESTATUS_H_ */
