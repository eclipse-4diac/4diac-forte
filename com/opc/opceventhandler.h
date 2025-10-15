/*******************************************************************************
 * Copyright (c) 2012, 2015, 2022 AIT, fortiss GmbH, HIT robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao, use stl vector
 *   Ketut Kumajaya - Clear command in queue on exit
 *                  - Fix disconnection issue on exit
 *******************************************************************************/
#pragma once

#include "forte/extevhan.h"
#include "forte/arch/forte_thread.h"
#include <vector>
#include "forte/arch/forte_sync.h"
#include "forte/cominfra/comlayer.h"

#include "ICmd.h"

class COpcEventHandler : public CExternalEventHandler,
                         public RegisterExternalEventHandler<COpcEventHandler>,
                         private CThread {
  public:
    explicit COpcEventHandler(CDeviceExecution &paDeviceExecution);
    ~COpcEventHandler() override;

    typedef int TCallbackDescriptor;

    void sendCommand(ICmd *paCmd);

    TCallbackDescriptor addComCallback(forte::com_infra::CComLayer *paComCallback);
    void removeComCallback(TCallbackDescriptor paCallbackDesc);

    void executeComCallback(TCallbackDescriptor paCallbackDesc);

    /* functions needed for the external event handler interface */
    void enableHandler() override {
      if (!isAlive()) {
        start();
        DEVLOG_INFO("COpcEventHandler: handler enabled\n");
      }
    }

    void disableHandler() override {
      if (!mComCallbacks.empty()) {
        DEVLOG_INFO("COpcEventHandler: command callback not empty\n");
        resumeSelfSuspend();
        return; // refuse, wait the second trigger
      }

      if (!mCommandQueue.isEmpty()) {
        DEVLOG_INFO("COpcEventHandler: command queue not empty\n");
        resumeSelfSuspend(); // wake-up, execute all commands in queue and continue
      }

      mStateSemaphore.timedWait(100000000); // wait 100ms to back wake-up
      if (isAlive()) {
        resumeSelfSuspend();
        setAlive(false);
        end();
        DEVLOG_INFO("COpcEventHandler: handler disabled\n");
      }
    }

    void resumeSelfSuspend();

    void selfSuspend();

  protected:
    void run() override;

  private:
    ICmd *getNextCommand();
    void clearCommandQueue();
    void executeCommandQueue();

    struct TComContainer {
        TCallbackDescriptor mCallbackDesc;
        forte::com_infra::CComLayer *mCallback;
    };

    typedef std::vector<TComContainer> TCallbackList;
    TCallbackList mComCallbacks;

    static TCallbackDescriptor mCallbackDescCount;

    static CSyncObject mSync;
    static CSemaphore mStateSemaphore;
    static bool mIsSemaphoreEmpty;

    typedef CSinglyLinkedList<ICmd *> TCommandQueue;
    TCommandQueue mCommandQueue;
};
