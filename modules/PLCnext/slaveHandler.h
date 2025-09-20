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

#ifndef SRC_MODULES_PLCNEXT_SLAVEHANDLER_H_
#define SRC_MODULES_PLCNEXT_SLAVEHANDLER_H_

#include <cstring>
#include "forte/arch/forte_sync.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/util/devlog.h"

#include "forte/io/configFB/io_slave_multi.h"
#include "slaveHandle/slaveHandle.h"
#include "deviceController.h"
#include "forte/io/mapper/io_mapper.h"
#include "types/PLCnextBusAdapter.h"
#include "plcNextDeviceInterface.h"

class PLCnextSlaveHandler : public forte::io::IOConfigFBMultiSlave {

  public:
    enum SlaveType { UnknownSlave = -1, NoUsage = 0, Input = 1, Output = 2 };

    int update();

    PLCnextSlaveHandle *getHandle(size_t paIndex);
    void addHandle(PLCnextSlaveHandle *paHandle);
    void dropHandles();
    void initBufferImage(size_t imageSize);

    forte::CPlugPin<FORTE_PLCnextBusAdapter_Plug> var_BusAdapterOut;
    forte::CSocketPin<FORTE_PLCnextBusAdapter_Socket> var_BusAdapterIn;

    const SlaveType slaveType = UnknownSlave;

    size_t imageSize = 0;
    size_t imageOffset = 0;
    char *imageBuffer;

    CSyncObject handleMutex;

    PLCnextSlaveHandler(int paType,
                        CFBContainer &paContainer,
                        const SFBInterfaceSpec *paInterfaceSpec,
                        const forte::StringId paInstanceNameId);

    ~PLCnextSlaveHandler() override;

  protected:
    PLCnextDeviceInterface plcNextDevice;

    void initHandles() override = 0;
    const char *init() override = 0;

    forte::IPlugPin *getPlugPinUnchecked(size_t) override;
    forte::ISocketPin *getSocketPinUnchecked(size_t) override;

    std::vector<PLCnextSlaveHandle *> mDeviceHandles;

    static const TForteUInt8 scmSlaveConfigurationIO[];
    static const TForteUInt8 scmSlaveConfigurationIONum;
};

#endif /* SRC_MODULES_PLCNEXT_SLAVEHANDLER_H_ */
