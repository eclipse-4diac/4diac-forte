/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextBusAdapter
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#pragma once

#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/io/configFB/io_adapter_multi.h"

class FORTE_PLCnextBusAdapter : public forte::io::IOConfigFBMultiAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_PLCnextBusAdapter)

  public:
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINITID = 0;

  protected:
    void setInitialValues() override;

  public:
    CIEC_BOOL var_QO;

    CIEC_BOOL var_QI;
    CIEC_UINT var_MasterId;
    CIEC_UINT var_Index;
    CIEC_UINT var_UpdateInterval;

    TEventID evt_INITO() {
      return getParentAdapterListEventID() + scmEventINITOID;
    }

    TEventID evt_INIT() {
      return getParentAdapterListEventID() + scmEventINITID;
    }

    CIEC_ANY *getDeviceConfigPin(int) override {
      return nullptr;
    }

    ~FORTE_PLCnextBusAdapter() override = default;

  protected:
    FORTE_PLCnextBusAdapter(CFBContainer &paContainer,
                            const SFBInterfaceSpec &paInterfaceSpec,
                            const forte::StringId paInstanceNameId,
                            TForteUInt8 paParentAdapterlistID);
};

class FORTE_PLCnextBusAdapter_Plug final : public FORTE_PLCnextBusAdapter {
  public:
    FORTE_PLCnextBusAdapter_Plug(forte::StringId paInstanceNameId,
                                 CFBContainer &paContainer,
                                 TForteUInt8 paParentAdapterlistID);
    ~FORTE_PLCnextBusAdapter_Plug() override = default;

    CEventConnection conn_INITO;

    CDataConnection *conn_QI;
    CDataConnection *conn_MasterId;
    CDataConnection *conn_Index;
    CDataConnection *conn_UpdateInterval;

    COutDataConnection<CIEC_BOOL> conn_QO;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};

class FORTE_PLCnextBusAdapter_Socket final : public FORTE_PLCnextBusAdapter {
  public:
    FORTE_PLCnextBusAdapter_Socket(forte::StringId paInstanceNameId,
                                   CFBContainer &paContainer,
                                   TForteUInt8 paParentAdapterlistID);
    ~FORTE_PLCnextBusAdapter_Socket() override = default;

    CEventConnection conn_INIT;

    CDataConnection *conn_QO;

    COutDataConnection<CIEC_BOOL> conn_QI;
    COutDataConnection<CIEC_UINT> conn_MasterId;
    COutDataConnection<CIEC_UINT> conn_Index;
    COutDataConnection<CIEC_UINT> conn_UpdateInterval;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
