/*************************************************************************
 *** Copyright (c) 2013 fortiss GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202511261532!
 ***
 *** Name: ATimeOut
 *** Description: Interface for a time out service roughly based on the definitions of ROOM
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/adapter.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_ATimeOut : public CAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_ATimeOut)

    private:
    public:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;

    private:
    public:
      static const TEventID scmEventTimeOutID = 0;

    private:
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

      ~FORTE_ATimeOut() override = default;

    protected:
      FORTE_ATimeOut(CFBContainer &paContainer,
                     const SFBInterfaceSpec &paInterfaceSpec,
                     const StringId paInstanceNameId,
                     TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_ATimeOut_Plug final : public FORTE_ATimeOut {
    public:
      FORTE_ATimeOut_Plug(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_ATimeOut_Plug() override = default;

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

  class FORTE_ATimeOut_Socket final : public FORTE_ATimeOut {
    public:
      FORTE_ATimeOut_Socket(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_ATimeOut_Socket() override = default;

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
