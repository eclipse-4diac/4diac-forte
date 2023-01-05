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

#include <stdint.h>
#include <cstring>
#include <fortelist.h>
#include <stdint.h>
#include <forte_sync.h>
#include <forte_wstring.h>
#include <devlog.h>

#include "../../core/io/configFB/io_slave_multi.h"
#include "slaveHandle/slaveHandle.h"
#include "deviceController.h"
#include "plcNextDeviceInterface.h"
#include <io/mapper/io_mapper.h>

class PLCnextSlaveHandler : public forte::core::io::IOConfigFBMultiSlave {

  public:
    enum SlaveType {
        UnknownSlave = -1,
        NoUsage = 0,
        Input = 1,
        Output = 2
    };

    int update();

    PLCnextSlaveHandle* getHandle(int paIndex);
    void addHandle(PLCnextSlaveHandle* paHandle);
    void dropHandles();
    void initBufferImage(size_t imageSize);

    const SlaveType slaveType = SlaveType::UnknownSlave;

    size_t imageSize = 0;
    size_t imageOffset = 0;
    char* imageBuffer;

    CSyncObject handleMutex;

    PLCnextSlaveHandler(int paType, CResource* paSrcRes, const SFBInterfaceSpec* paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId);

    ~PLCnextSlaveHandler();

  protected:
    PLCnextDeviceInterface plcNextDevice;

    virtual void initHandles() = 0;
    virtual const char* init() = 0;

    typedef CSinglyLinkedList<PLCnextSlaveHandle*> TSlaveHandleList;
    TSlaveHandleList *slaveHandles = new TSlaveHandleList();

    static const TForteUInt8 scmSlaveConfigurationIO[];
    static const TForteUInt8 scmSlaveConfigurationIONum;
};

#endif /* SRC_MODULES_PLCNEXT_SLAVEHANDLER_H_ */
