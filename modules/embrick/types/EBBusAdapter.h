/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Alois Zoitl - Upgraded to new adapter architecture
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::embrick {

  class FORTE_EBBusAdapter : public forte::io::IOConfigFBMultiAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_EBBusAdapter)

    public:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINITID = 0;

      CIEC_UINT var_UpdateInterval;

      TEventID evt_INITO() {
        return getParentAdapterListEventID() + scmEventINITOID;
      }

      TEventID evt_INIT() {
        return getParentAdapterListEventID() + scmEventINITID;
      }

      ~FORTE_EBBusAdapter() override = default;

      void setInitialValues() override;

      CIEC_ANY *getDeviceConfigPin(int paIndex) override {
        return (paIndex == 3) ? &var_UpdateInterval : nullptr;
      }

    protected:
      FORTE_EBBusAdapter(CFBContainer &paContainer,
                         const SFBInterfaceSpec &paInterfaceSpec,
                         const forte::StringId paInstanceNameId,
                         TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_EBBusAdapter_Socket;

  class FORTE_EBBusAdapter_Plug final : public FORTE_EBBusAdapter {
    public:
      FORTE_EBBusAdapter_Plug(forte::StringId paInstanceNameId,
                              CFBContainer &paContainer,
                              TForteUInt8 paParentAdapterlistID);
      ~FORTE_EBBusAdapter_Plug() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_MasterId;
      CDataConnection *conn_MasterIndex;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_EBBusAdapter_Socket *getSocket();
  };

  class FORTE_EBBusAdapter_Socket final : public FORTE_EBBusAdapter {
    public:
      FORTE_EBBusAdapter_Socket(forte::StringId paInstanceNameId,
                                CFBContainer &paContainer,
                                TForteUInt8 paParentAdapterlistID);
      ~FORTE_EBBusAdapter_Socket() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CDataConnection *conn_QO;

      CEventConnection conn_INIT;

      COutDataConnection<CIEC_BOOL> conn_QI;
      COutDataConnection<CIEC_UINT> conn_MasterId;
      COutDataConnection<CIEC_UINT> conn_MasterIndex;
      COutDataConnection<CIEC_UINT> conn_UpdateInterval;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_EBBusAdapter_Plug *getPlug() {
        return static_cast<FORTE_EBBusAdapter_Plug *>(getPeer());
      }
  };

} // namespace forte::eclipse4diac::io::embrick
