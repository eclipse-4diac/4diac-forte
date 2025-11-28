/*******************************************************************************
 * Copyright (c) 2025 Alexander Trojnin github.com/trojninalex
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alexander Trojnin - initial API and implementation and/or initial documentation
 *  Malte Grave - Adapt the CAN handler to the new 4diac-FORTE structure
 *******************************************************************************/

#pragma once

#include <linux/can.h>
#include <linux/can/raw.h>

#include "forte/arch/forte_thread.h"
#include "forte/arch/sockhand.h"
#include "forte/extevhan.h"
#include "forte/cominfra/comCallback.h"

using namespace forte::arch;

namespace forte::com_infra {
  class CCANHandler : public CExternalEventHandler, public RegisterExternalEventHandler<CCANHandler>, private CThread {
    public:
      explicit CCANHandler(CDeviceExecution &paDeviceExecution);
      ~CCANHandler() override;
      void addComCallback(CFDSelectHandler::TFileDescriptor paFD, CComCallback *paCanLayer);
      void removeComCallback(CFDSelectHandler::TFileDescriptor paFD);

      void enableHandler() override {
        start();
      }

      void disableHandler() override {
        end();
      }

      void setPriority(int) {
      }

      int getPriority() const {
        return 0;
      }

    protected:
      void run() override;

    private:
      struct TConnContType {
          CFDSelectHandler::TFileDescriptor mSockDes;
          CComCallback *mCalled;
      };

      struct can_frame frame;

      typedef std::vector<TConnContType> TConnectionContainer;

      CFDSelectHandler::TFileDescriptor createFDSet(fd_set *m_panFDSet);

      TConnectionContainer mConnectionsList;
      CSyncObject mSync;
      bool mConnectionListChanged;
  };
} // namespace forte::com_infra
