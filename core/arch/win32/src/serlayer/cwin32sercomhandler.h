/*******************************************************************************
 * Copyright (c) 2013 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/arch/forte_thread.h"
#include "forte/arch/forte_sync.h"
#include "forte/extevhan.h"
#include "forte/arch/forte_sem.h"

namespace forte::arch {
  class CWin32SerComLayer;

  class CWin32SerComHandler : public CExternalEventHandler,
                              public RegisterExternalEventHandler<CWin32SerComHandler>,
                              public CThread {
    public:
      explicit CWin32SerComHandler(CDeviceExecution &paDeviceExecution);
      ~CWin32SerComHandler() override;

      void registerSerComLayer(CWin32SerComLayer *paComLayer);
      void unregisterSerComLayer(CWin32SerComLayer *paComLayer);

      /* functions needed for the external event handler interface */
      void enableHandler() override {
        start();
      }

      void disableHandler() override {
        setAlive(false);
        mSem.inc();
        end();
      }

    private:
      void run();

      std::vector<CWin32SerComLayer *> mComLayerList;
      CSyncObject mSync;

      //! Sempahore for implementing a suspend feature similar to what we are doing in CEventChainExecutionThread
      CSemaphore mSem;
  };
} // namespace forte::arch
