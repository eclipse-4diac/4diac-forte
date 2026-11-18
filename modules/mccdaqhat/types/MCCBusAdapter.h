/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  class FORTE_MCCBusAdapter : public forte::io::IOConfigFBMultiAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_MCCBusAdapter)

    public:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINITID = 0;

      ~FORTE_MCCBusAdapter() override = default;

      void setInitialValues() override;

      //! The MCC bus forwards no per-device configuration; every device carries its own parameters.
      CIEC_ANY *getDeviceConfigPin(int) override {
        return nullptr;
      }

    protected:
      FORTE_MCCBusAdapter(CFBContainer &paContainer,
                          const SFBInterfaceSpec &paInterfaceSpec,
                          const forte::StringId paInstanceNameId,
                          TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_MCCBusAdapter_Socket;

  class FORTE_MCCBusAdapter_Plug final : public FORTE_MCCBusAdapter {
    public:
      FORTE_MCCBusAdapter_Plug(forte::StringId paInstanceNameId,
                               CFBContainer &paContainer,
                               TForteUInt8 paParentAdapterlistID);
      ~FORTE_MCCBusAdapter_Plug() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CEventConnection conn_INITO;
      CDataConnection *conn_QI;
      CDataConnection *conn_MasterId;
      CDataConnection *conn_Index;
      COutDataConnection<CIEC_BOOL> conn_QO;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_MCCBusAdapter_Socket *getSocket();
  };

  class FORTE_MCCBusAdapter_Socket final : public FORTE_MCCBusAdapter {
    public:
      FORTE_MCCBusAdapter_Socket(forte::StringId paInstanceNameId,
                                 CFBContainer &paContainer,
                                 TForteUInt8 paParentAdapterlistID);
      ~FORTE_MCCBusAdapter_Socket() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      CDataConnection *conn_QO;
      CEventConnection conn_INIT;
      COutDataConnection<CIEC_BOOL> conn_QI;
      COutDataConnection<CIEC_UINT> conn_MasterId;
      COutDataConnection<CIEC_UINT> conn_Index;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_MCCBusAdapter_Plug *getPlug() {
        return static_cast<FORTE_MCCBusAdapter_Plug *>(getPeer());
      }
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
