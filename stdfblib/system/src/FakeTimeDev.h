/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH,
 *                                Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - Copied and modified from RMT_DEV.h
 *******************************************************************************/
#pragma once

#include "forte/device.h"
#include "forte/dataconn.h"
#include "RMT_RES.h"

namespace forte::iec61499::system {
  class FakeTimeDev : public CDevice {
    public:
      explicit FakeTimeDev(std::string_view paMGR_ID = "localhost:61499");
      ~FakeTimeDev() override;

      bool initialize() override;

      /*! \brief Adds additional functionality to the originals execute func of the device.
       *
       * This is that it waits till the thread of the MGR resource has anded
       */
      int startDevice() override;

      void awaitShutdown() override;

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

      EMGMResponse writeValue(TNameIdentifier &paNameList, const std::string &paValue, bool paForce = false) override;

    private:
      CDataConnection *conn_MGR_ID;
      CDataConnection *conn_FakeTime;

      COutDataConnection<CIEC_WSTRING> conn_MGR_ID_int;
      COutDataConnection<CIEC_TIME> conn_FakeTime_int;

      CIEC_ANY *getDI(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CConnection *getResIf2InConnectionUnchecked(TPortId) override;

      RMT_RES MGR;

      // allows to set the fake timer before calling the parent CDevice which
      // will create the timer already at construction
      StringId initializeTimer();
  };
} // namespace forte::iec61499::system
