/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, fortiss GmbH, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#ifndef _MODBUSHANDLER_H_
#define _MODBUSHANDLER_H_

#include <forte_config.h>
#include "extevhan.h"

namespace forte {
  namespace com_infra {
    class CModbusComLayer;
  }
}

class CModbusHandler : public CExternalEventHandler{
    DECLARE_HANDLER(CModbusHandler)
  public:
    typedef int TCallbackDescriptor;

    void enableHandler() override {
    }
    ;
    void disableHandler() override {
    }
    ;

    void setPriority(int) override {
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority() const override {
      //the same as for setPriority
      return 0;
    }

    void executeComCallback(forte::com_infra::CModbusComLayer* paComCallback);
};

#endif // _MODBUSHANDLER_H_
