/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#pragma once

#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::revpi {

  class FORTE_IORevPiBusAdapter : public ::forte::io::IOConfigFBMultiAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_IORevPiBusAdapter)

    protected:
      void setInitialValues() override;

    public:
      CIEC_BOOL var_QO;

      CIEC_BOOL var_QI;
      CIEC_UINT var_MasterId;
      CIEC_UINT var_Index;

      TEventID evt_INITO() {
        return getParentAdapterListEventID() + scmEventINITOID;
      }

      TEventID evt_INIT() {
        return getParentAdapterListEventID() + scmEventINITID;
      }

      CIEC_ANY *getDeviceConfigPin(int) override {
        return nullptr;
      }
      ~FORTE_IORevPiBusAdapter() override = default;

    protected:
      FORTE_IORevPiBusAdapter(CFBContainer &paContainer,
                              const SFBInterfaceSpec &paInterfaceSpec,
                              const forte::StringId paInstanceNameId,
                              TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_IORevPiBusAdapter_Plug final : public FORTE_IORevPiBusAdapter {
    public:
      FORTE_IORevPiBusAdapter_Plug(forte::StringId paInstanceNameId,
                                   CFBContainer &paContainer,
                                   TForteUInt8 paParentAdapterlistID);
      ~FORTE_IORevPiBusAdapter_Plug() override = default;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_MasterId;
      CDataConnection *conn_Index;

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

  class FORTE_IORevPiBusAdapter_Socket final : public FORTE_IORevPiBusAdapter {
    public:
      FORTE_IORevPiBusAdapter_Socket(forte::StringId paInstanceNameId,
                                     CFBContainer &paContainer,
                                     TForteUInt8 paParentAdapterlistID);
      ~FORTE_IORevPiBusAdapter_Socket() override = default;

      CEventConnection conn_INIT;

      CDataConnection *conn_QO;

      COutDataConnection<CIEC_BOOL> conn_QI;
      COutDataConnection<CIEC_UINT> conn_MasterId;
      COutDataConnection<CIEC_UINT> conn_Index;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

} // namespace forte::eclipse4diac::io::revpi
