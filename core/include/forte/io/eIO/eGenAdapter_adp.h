/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/adapter.h"
#include "forte/typelib.h"

namespace forte::io {
  class FORTE_eGenAdapter : public CAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_eGenAdapter)

    public:
      FORTE_eGenAdapter(CFBContainer &paContainer,
                        const SFBInterfaceSpec &paInterfaceSpec,
                        const StringId paInstanceNameId,
                        TForteUInt8 paParentAdapterlistID) :
          CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID) {};

      ~FORTE_eGenAdapter() override = default;

      void setInitialValues() override;
  };

  class FORTE_eGenAdapter_Socket;

  class FORTE_eGenAdapter_Plug final : public FORTE_eGenAdapter {
    public:
      FORTE_eGenAdapter_Plug(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_eGenAdapter_Plug() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_eGenAdapter_Socket *getSocket();
  };

  class FORTE_eGenAdapter_Socket final : public FORTE_eGenAdapter {
    public:
      FORTE_eGenAdapter_Socket(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID);
      ~FORTE_eGenAdapter_Socket() override = default;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    protected:
      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(TPortId paDINum) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;
      CIEC_ANY *getDO(TPortId paDONum) override;

    private:
      FORTE_eGenAdapter_Plug *getPlug() {
        return static_cast<FORTE_eGenAdapter_Plug *>(getPeer());
      }
  };
} // namespace forte::io
