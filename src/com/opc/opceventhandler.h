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
#ifndef OPCEVENTHANDLER_H_
#define OPCEVENTHANDLER_H_

#include "extevhan.h"
#include "forte_thread.h"
#include <vector>
#include "forte_sync.h"
#include "comlayer.h"

#include "ICmd.h"

class COpcEventHandler : public CExternalEventHandler, private CThread{
    DECLARE_HANDLER(COpcEventHandler)
  public:
    typedef int TCallbackDescriptor;

    void sendCommand(ICmd *paCmd);

    TCallbackDescriptor addComCallback(forte::com_infra::CComLayer* paComCallback);
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
        resumeSelfSuspend(); //wake-up, execute all commands in queue and continue
      }

      mStateSemaphore.timedWait(100000000); // wait 100ms to back wake-up
      if (isAlive()) {
        resumeSelfSuspend();
        setAlive(false);
        end();
        DEVLOG_INFO("COpcEventHandler: handler disabled\n");
      }
    }

    void setPriority(int) override {
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority() const override {
      //the same as for setPriority
      return 0;
    }

    void resumeSelfSuspend();

    void selfSuspend();

  protected:
    void run() override;

  private:
    ICmd* getNextCommand();
    void clearCommandQueue();
    void executeCommandQueue();

    struct TComContainer{
        TCallbackDescriptor mCallbackDesc;
        forte::com_infra::CComLayer* mCallback;
    };

    typedef std::vector<TComContainer> TCallbackList;
    TCallbackList mComCallbacks;

    static TCallbackDescriptor mCallbackDescCount;

    static CSyncObject mSync;
    static forte::arch::CSemaphore mStateSemaphore;
    static bool mIsSemaphoreEmpty;

    typedef CSinglyLinkedList<ICmd*> TCommandQueue;
    TCommandQueue mCommandQueue;
};

#endif // OPCEVENTHANDLER_H_
