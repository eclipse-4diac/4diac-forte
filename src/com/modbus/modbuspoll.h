/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#ifndef MODBUSPOLL_H_
#define MODBUSPOLL_H_

#include "modbustimedevent.h"
#include <vector>

class CModbusIOBlock;
class CModbusHandler;

class CModbusPoll : public CModbusTimedEvent{
  public:
    CModbusPoll(CModbusHandler* paModbusHandler, long paPollInterval);
    ~CModbusPoll() override;

    int executeEvent(modbus_t *paModbusConn, void *paRetVal) override;

    void addPollBlock(CModbusIOBlock *paIOBlock);

  private:
    CModbusHandler *const mModbusHandler;
    std::vector<CModbusIOBlock*> mPolls;

    int readOneBlock(modbus_t *paModbusConn, CModbusIOBlock *paIOBlock);
};

#endif /* MODBUSPOLL_H_ */
