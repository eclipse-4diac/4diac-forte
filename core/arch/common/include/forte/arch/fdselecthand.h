/*******************************************************************************
 * Copyright (c) 2010 - 2014 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Patrick Smejkal
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/extevhan.h"
#include "forte/arch/forte_thread.h"
#include "forte/arch/forte_sync.h"
#include "forte/arch/sockhand.h"

#include <forward_list>

namespace forte {
  namespace com_infra {
    class CComCallback;
  }

  namespace arch {
    /*!\brief An external event handler for file descriptor based external events.
     *
     * This handler uses a thread and the select function to monitor given file descriptors on
     * data reception. Examples for possible file descriptors are sockets or com re
     */
    class CFDSelectHandler : public CExternalEventHandler,
                             public RegisterExternalEventHandler<CFDSelectHandler>,
                             private CThread {
      public:
        explicit CFDSelectHandler(CDeviceExecution &paDeviceExecution);
        ~CFDSelectHandler() override;

        typedef FORTE_SOCKET_TYPE
            TFileDescriptor; //!< General type definition for a file descriptor. To be used by the callback classes.
        static const TFileDescriptor scmInvalidFileDescriptor = FORTE_INVALID_SOCKET;

        void addComCallback(TFileDescriptor paFD, com_infra::CComCallback *paComLayer);
        void removeComCallback(TFileDescriptor paFD);

        /* functions needed for the external event handler interface */
        void enableHandler() override {
          start();
        }

        void disableHandler() override {
          end();
        }

      protected:
        void run() override;

      private:
        struct TConnContType {
            TFileDescriptor mSockDes;
            com_infra::CComCallback *mCallee;
        };

        TFileDescriptor createFDSet(fd_set *mFDSet);

        std::forward_list<TConnContType> mConnectionsList;
        CSyncObject mSync;
        bool mConnectionListChanged;
    };
  } // namespace arch
} // namespace forte
