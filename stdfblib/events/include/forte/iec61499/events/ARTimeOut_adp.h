/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/adapter.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_ARTimeOut : public CAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_ARTimeOut)

    private:
    public:
      static const TEventID scmEventTimeOutID = 0;

    private:
    public:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;

    private:
    protected:
      void setInitialValues() override;

    public:
      CIEC_TIME var_DT;

      TEventID evt_TimeOut() {
        return getParentAdapterListEventID() + scmEventTimeOutID;
      }

      TEventID evt_START() {
        return getParentAdapterListEventID() + scmEventSTARTID;
      }

      TEventID evt_STOP() {
        return getParentAdapterListEventID() + scmEventSTOPID;
      }

      ~FORTE_ARTimeOut() override = default;

    protected:
      FORTE_ARTimeOut(CFBContainer &paContainer,
                      const SFBInterfaceSpec &paInterfaceSpec,
                      const StringId paInstanceNameId,
                      TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_ARTimeOut_Plug final : public FORTE_ARTimeOut {
    public:
      FORTE_ARTimeOut_Plug(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_ARTimeOut_Plug() override = default;

      CEventConnection conn_TimeOut;

      CDataConnection *conn_DT;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  class FORTE_ARTimeOut_Socket final : public FORTE_ARTimeOut {
    public:
      FORTE_ARTimeOut_Socket(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_ARTimeOut_Socket() override = default;

      CEventConnection conn_START;
      CEventConnection conn_STOP;

      COutDataConnection<CIEC_TIME> conn_DT;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
