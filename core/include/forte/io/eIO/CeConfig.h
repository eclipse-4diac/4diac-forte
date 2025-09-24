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

#include "forte/io/eIO/CeSpec.h"

#include "forte/util/devlog.h"
#include "forte/adapter.h"

namespace forte::io {
  class FORTE_eGenAdapter;
  /**
   * This class handles the registration of event-triggers and
   * maintains a record of all registered event-triggers from this instance, within this instance.
   */
  class CeConfig {
    private:
      CProcessInterfaceFB *mPeer;

    protected:
      std::vector<CeSpecBase *> mEventGenList;

      void registerEventTrigger(CeSpecBase *paEvent) {
        mEventGenList.push_back(paEvent);
      }

      void deregisterEventTrigger(CeSpecBase *paEvent) {
        auto it = std::find(mEventGenList.begin(), mEventGenList.end(), paEvent);
        if (it != mEventGenList.end()) {
          mEventGenList.erase(it);
          delete paEvent;
        }
      }

      void deregisterFBsEventTrigger() {
        for (CeSpecBase *eventSpec : mEventGenList) {
          deregisterEventTrigger(eventSpec);
        }
      }

      bool setIOPeer(CAdapter *paAdapter) {
        /* connecting to adapter peer */
        if (auto *peerAdapter = paAdapter->getPeer(); peerAdapter != nullptr) {
          mPeer = &static_cast<CProcessInterfaceFB &>(peerAdapter->getParent());
        } else {
          DEVLOG_ERROR("[getIOPeer] Could not reach IO FB via adapter!\r\n");
          return false;
        }
        if (mPeer == nullptr) {
          DEVLOG_ERROR("[getIOPeer] IO instance is nullptr.\r\n");
          return false;
        }
        return true;
      }

      CProcessInterfaceFB *getIOPeer(CAdapter *paAdapter) {
        if (mPeer == nullptr) {
          setIOPeer(paAdapter);
        }
        return mPeer;
      }

    public:
      virtual bool eventGen() = 0;
  };
} // namespace forte::io
