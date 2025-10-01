/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
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
 *******************************************************************************/

#pragma once

#include "EBBusAdapter.h"
#include "forte/arch/forte_sync.h"
#include "../slave/slave.h"
#include "../slave/handle.h"
#include "forte/io/configFB/io_slave_multi.h"

namespace forte::eclipse4diac::io::embrick {

  class EmbrickSlave : public forte::io::IOConfigFBMultiSlave, public EmbrickSlaveHandler::Delegate {
    public:
      EmbrickSlave(const TForteUInt8 *const paSlaveConfigurationIO,
                   const TForteUInt8 paSlaveConfigurationIO_num,
                   int paType,
                   CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   const forte::StringId paInstanceNameId);
      ~EmbrickSlave() override;

      forte::CSocketPin<FORTE_EBBusAdapter_Socket> var_BusAdapterIn;
      forte::CPlugPin<FORTE_EBBusAdapter_Plug> var_BusAdapterOut;

      forte::IPlugPin *getPlugPinUnchecked(size_t) override;
      forte::ISocketPin *getSocketPinUnchecked(size_t) override;

    protected:
      virtual CIEC_UINT &UpdateInterval() {
        // TODO Remove
        return *static_cast<CIEC_UINT *>(getDI(0));
      }

      arch::CSyncObject mSlaveMutex;
      EmbrickSlaveHandler *mSlave;

    public:
      void onSlaveStatus(EmbrickSlaveHandler::SlaveStatus paStatus,
                         EmbrickSlaveHandler::SlaveStatus paOldStatus) override;
      void onSlaveDestroy() override;

    private:
      const char *init() override;
      void deInit() override;

      static const CIEC_WSTRING scmSlow;
      static const CIEC_WSTRING scmInterrupted;
      static const CIEC_WSTRING scmError;
      static const CIEC_WSTRING scmUnknown;
  };

} // namespace forte::eclipse4diac::io::embrick
