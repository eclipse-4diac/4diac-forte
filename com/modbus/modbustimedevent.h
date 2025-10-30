/*******************************************************************************
 * Copyright (c) 2012, 2025 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#pragma once

#include "forte/datatype.h"
#include <modbus/modbus.h>

namespace forte::com_infra::modbus {

  class CModbusTimedEvent {
    public:
      explicit CModbusTimedEvent(TForteUInt32 paUpdateInterval); // UpdateInterval = 0 => single shot event
      virtual ~CModbusTimedEvent() = default;

      void setUpdateInterval(TForteUInt32 paUpdateInterval);
      TForteUInt32 getUpdateInterval() const {
        return mUpdateInterval;
      }

      void activate();
      void deactivate();

      bool isStarted() const {
        return mIsStarted;
      }

      bool readyToExecute() const;

      // Classes impementing this should call restartTimer in executeEvent
      virtual int executeEvent(modbus_t *paModbusConn, void *paRetVal) = 0;

    protected:
      void restartTimer();

    private:
      uint_fast64_t mStartTime;

      TForteUInt32 mUpdateInterval; // Polling interval in milliseconds (0 => single shot event)

      bool mSingleShotEvent;
      bool mIsStarted;
  };

} // namespace forte::com_infra::modbus
