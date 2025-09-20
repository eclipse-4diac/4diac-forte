/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/adapter.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

class FORTE_PortAdapter : public forte::CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_PortAdapter)

  private:
  public:
    static const TEventID scmEventMAPID = 0;

  private:
  public:
    static const TEventID scmEventMAPOID = 0;

  private:
    void setInitialValues() override;

  public:
    CIEC_DWORD var_GPIO_Port_Addr;

    TEventID evt_MAPO() {
      return getParentAdapterListEventID() + scmEventMAPOID;
    }

    TEventID evt_MAP() {
      return getParentAdapterListEventID() + scmEventMAPID;
    }

    ~FORTE_PortAdapter() override = default;

  protected:
    FORTE_PortAdapter(CFBContainer &paContainer,
                      const SFBInterfaceSpec &paInterfaceSpec,
                      const forte::StringId paInstanceNameId,
                      TForteUInt8 paParentAdapterlistID);
};

class FORTE_PortAdapter_Plug final : public FORTE_PortAdapter {
  public:
    FORTE_PortAdapter_Plug(forte::StringId paInstanceNameId,
                           CFBContainer &paContainer,
                           TForteUInt8 paParentAdapterlistID);
    ~FORTE_PortAdapter_Plug() override = default;

    CEventConnection conn_MAPO;

    CDataConnection *conn_GPIO_Port_Addr;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};

class FORTE_PortAdapter_Socket final : public FORTE_PortAdapter {
  public:
    FORTE_PortAdapter_Socket(forte::StringId paInstanceNameId,
                             CFBContainer &paContainer,
                             TForteUInt8 paParentAdapterlistID);
    ~FORTE_PortAdapter_Socket() override = default;

    CEventConnection conn_MAP;

    COutDataConnection<CIEC_DWORD> conn_GPIO_Port_Addr;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
