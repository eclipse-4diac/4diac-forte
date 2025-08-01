/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
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

#include "core/adapter.h"
#include "core/typelib.h"
#include "core/datatypes/forte_time.h"
#include "core/adapterconn.h"

class FORTE_ATimeOut : public forte::CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_ATimeOut)

  public:
    static const TEventID scmEventTimeOutID = 0;

    static const TEventID scmEventSTARTID = 0;
    static const TEventID scmEventSTOPID = 1;

    void setInitialValues() override;

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

  protected:
    FORTE_ATimeOut(forte::core::CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   const forte::core::StringId paInstanceNameId,
                   TForteUInt8 paParentAdapterlistID);

    ~FORTE_ATimeOut() override = default;
};

class FORTE_ATimeOut_Socket;

class FORTE_ATimeOut_Plug final : public FORTE_ATimeOut {
  public:
    FORTE_ATimeOut_Plug(forte::core::StringId paInstanceNameId,
                        forte::core::CFBContainer &paContainer,
                        TForteUInt8 paParentAdapterlistID);
    ~FORTE_ATimeOut_Plug() override = default;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

    CEventConnection conn_TimeOUT;

    CDataConnection *conn_DT;

  protected:
    CEventConnection *getEOConUnchecked(TPortId paEONum) override;
    CIEC_ANY *getDI(TPortId paDINum) override;
    CDataConnection **getDIConUnchecked(TPortId paDINum) override;
    CDataConnection *getDOConUnchecked(TPortId paDONum) override;
    CIEC_ANY *getDO(TPortId paDONum) override;

  private:
    FORTE_ATimeOut_Socket *getSocket();
};

class FORTE_ATimeOut_Socket final : public FORTE_ATimeOut {
  public:
    FORTE_ATimeOut_Socket(forte::core::StringId paInstanceNameId,
                          forte::core::CFBContainer &paContainer,
                          TForteUInt8 paParentAdapterlistID);
    ~FORTE_ATimeOut_Socket() override = default;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

    CEventConnection conn_START;
    CEventConnection conn_STOP;

    COutDataConnection<CIEC_TIME> conn_DT;

  protected:
    CEventConnection *getEOConUnchecked(TPortId paEONum) override;
    CIEC_ANY *getDI(TPortId paDINum) override;
    CDataConnection **getDIConUnchecked(TPortId paDINum) override;
    CDataConnection *getDOConUnchecked(TPortId paDONum) override;
    CIEC_ANY *getDO(TPortId paDONum) override;

  private:
    FORTE_ATimeOut_Plug *getPlug() {
      return static_cast<FORTE_ATimeOut_Plug *>(getPeer());
    }
};
