/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef MODBUSPOLL_H_
#define MODBUSPOLL_H_

#include "modbustimedevent.h"
#include <vector>

class CModbusIOBlock;
class CModbusHandler;

class CModbusPoll : public CModbusTimedEvent{
  public:
    CModbusPoll(CModbusHandler* pa_pModbusHandler, long pa_nPollInterval);
    ~CModbusPoll() override;

    int executeEvent(modbus_t *pa_pModbusConn, void *pa_pRetVal) override;

    void addPollBlock(CModbusIOBlock *pa_pIOBlock);

  private:
    CModbusHandler *const m_pModbusHandler;
    std::vector<CModbusIOBlock*> m_lPolls;

    int readOneBlock(modbus_t *pa_pModbusConn, CModbusIOBlock *pa_pIOBlock);
};

#endif /* MODBUSPOLL_H_ */
