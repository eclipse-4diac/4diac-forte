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

#include "forte/io/configFB/io_slave_multi.h"
#include "forte/datatypes/forte_string.h"
#include "MCCBusAdapter.h"
#include "../MCCDaqHatController.h"
#include "../mccdaqhat_api.h"

#include <cstddef>

namespace forte::eclipse4diac::io::mccdaqhat {

  /*! @brief Common part of the MCC device FBs.
   *
   * DI 0 is QI and DI 1 is Address in every MCC device. The device registers its jumper address with the controller
   * before the framework runs isSlaveAvailable()/checkSlaveType(), so both checks look at the configured address
   * instead of the position in the adapter chain.
   */
  class MCCIODevice : public forte::io::IOConfigFBMultiSlave {
    public:
      MCCIODevice(HatType paType,
               CFBContainer &paContainer,
               const SFBInterfaceSpec &paInterfaceSpec,
               const forte::StringId paInstanceNameId);

      forte::CSocketPin<FORTE_MCCBusAdapter_Socket> var_BusAdapterIn;
      forte::CPlugPin<FORTE_MCCBusAdapter_Plug> var_BusAdapterOut;

      forte::IPlugPin *getPlugPinUnchecked(size_t) override;
      forte::ISocketPin *getSocketPinUnchecked(size_t) override;

    protected:
      virtual unsigned int getConfiguredAddress() = 0;

      MCCDaqHatController &getMCCController() {
        return static_cast<MCCDaqHatController &>(getController());
      }

      //! Registers one channel as IO handle if its id is not empty
      void addChannel(CIEC_STRING &paId,
                      forte::io::IOMapper::Direction paDirection,
                      MCCDaqHatController::HandleKind paKind,
                      uint8_t paChannel);

      //! Storage for the text returned by init(); the framework copies it into STATUS
      std::string mInitError;

    private:
      bool createSlaveHandler() override;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
