/*******************************************************************************
 * Copyright (c) 2012, 2025 AIT, fortiss GmbH, Davor Cihlar
 *
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
#pragma once

#include "forte/extevhan.h"

namespace forte::com_infra::modbus {

  class CModbusComLayer;

  class CModbusHandler : public CExternalEventHandler, public RegisterExternalEventHandler<CModbusHandler> {
    public:
      explicit CModbusHandler(CDeviceExecution &paDeviceExecution);
      ~CModbusHandler() override;

      typedef int TCallbackDescriptor;

      void enableHandler() override {};
      void disableHandler() override {};

      void executeComCallback(CModbusComLayer *paComCallback);
  };

} // namespace forte::com_infra::modbus
