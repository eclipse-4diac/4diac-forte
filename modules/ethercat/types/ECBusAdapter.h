/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::ethercat {

  class FORTE_ECBusAdapter : public forte::io::IOConfigFBMultiAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_ECBusAdapter)

    public:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINITID = 0;

      TEventID evt_INITO() {
        return getParentAdapterListEventID() + scmEventINITOID;
      }

      TEventID evt_INIT() {
        return getParentAdapterListEventID() + scmEventINITID;
      }

      ~FORTE_ECBusAdapter() override = default;

      void setInitialValues() override;

      CIEC_ANY *getDeviceConfigPin(int) override {
        return nullptr;
      }

    protected:
      FORTE_ECBusAdapter(CFBContainer &paContainer,
                         const SFBInterfaceSpec &paInterfaceSpec,
                         const forte::StringId paInstanceNameId,
                         TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_ECBusAdapter_Socket;

  class FORTE_ECBusAdapter_Plug final : public FORTE_ECBusAdapter {
    public:
      FORTE_ECBusAdapter_Plug(forte::StringId paInstanceNameId,
                              CFBContainer &paContainer,
                              TForteUInt8 paParentAdapterlistID);
      ~FORTE_ECBusAdapter_Plug() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_MasterId;
      CDataConnection *conn_MasterIndex;

      COutDataConnection<CIEC_BOOL> conn_QO;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_ECBusAdapter_Socket *getSocket();
  };

  class FORTE_ECBusAdapter_Socket final : public FORTE_ECBusAdapter {
    public:
      FORTE_ECBusAdapter_Socket(forte::StringId paInstanceNameId,
                                CFBContainer &paContainer,
                                TForteUInt8 paParentAdapterlistID);
      ~FORTE_ECBusAdapter_Socket() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CDataConnection *conn_QO;

      CEventConnection conn_INIT;

      COutDataConnection<CIEC_BOOL> conn_QI;
      COutDataConnection<CIEC_UINT> conn_MasterId;
      COutDataConnection<CIEC_UINT> conn_MasterIndex;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_ECBusAdapter_Plug *getPlug() {
        return static_cast<FORTE_ECBusAdapter_Plug *>(getPeer());
      }
  };
}
