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

#include <forward_list>
#include "extevhandlerhelper.h"
#include "ESpec.h"

#include "devlog.h"

class FORTE_eGenAdapter;
/**
 * This class handles the registration of event-triggers on the controller
 * and maintains a record of all registered event-triggers from this instance, within this instance.
 */
class EConfig {
  private:
    CProcessInterfaceFB *peer;

  protected:
    std::forward_list<ESpecBase*> eventGenList;

   void registerEventTrigger(ESpecBase* event) {
    eventGenList.push_front(event);
   }

   void deregisterEventTrigger(ESpecBase* event) {
    eventGenList.remove(event);
    delete event;
   }

   void deregisterFBsEventTrigger() {
    for (ESpecBase* eventSpec : eventGenList) {
     deregisterEventTrigger(eventSpec);
    }
   }

    bool setIOPeer(CAdapter *adapter) {
      /* connecting to adapter peer */
      CAdapter* peerAdapter = nullptr;
      FORTE_eGenAdapter* eGenAdapter = nullptr;

      if ((peerAdapter = adapter->getPeer()) != nullptr){
        eGenAdapter = static_cast<FORTE_eGenAdapter *>(peerAdapter);
        peer = static_cast<CProcessInterfaceFB *>(eGenAdapter->parentFB);
      } else {
        DEVLOG_ERROR("[getIOPeer] Could not reach IO FB via adapter!\r\n");
        return false;
      }
      if (peer == nullptr) {
        DEVLOG_ERROR("[getIOPeer] IO instance is nullptr.\r\n");
        return false;
      }
    return true;
    }

    CProcessInterfaceFB *getIOPeer(CAdapter *adapter) {
     if (peer == nullptr)
       setIOPeer(adapter);
     return peer;
   }

  public:
    virtual std::forward_list<ESpecBase*> *eventGen() = 0;
};
