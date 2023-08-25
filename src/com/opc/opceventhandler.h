/*******************************************************************************
 * Copyright (c) 2012, 2015 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef OPCEVENTHANDLER_H_
#define OPCEVENTHANDLER_H_

#include "extevhan.h"
#include <forte_thread.h>
#include "fortelist.h"
#include <forte_sync.h>
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
      start();
    }

    void disableHandler() override {
      end();
    }

    void setPriority(int) override {
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority() const override {
      //the same as for setPriority
      return 0;
    }

  protected:
    void run() override;

  private:
    ICmd* getNextCommand();

    struct TComContainer{
        TCallbackDescriptor mCallbackDesc;
        forte::com_infra::CComLayer* mCallback;
    };

    typedef CSinglyLinkedList<TComContainer> TCallbackList;
    TCallbackList mComCallbacks;

    static TCallbackDescriptor mCallbackDescCount;

    CSyncObject mSync;

    typedef CSinglyLinkedList<ICmd*> TCommandQueue;
    TCommandQueue mCommandQueue;
};

#endif // OPCEVENTHANDLER_H_
