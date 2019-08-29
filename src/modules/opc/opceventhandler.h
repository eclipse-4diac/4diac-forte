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

    void sendCommand(ICmd *pa_pCmd);

    TCallbackDescriptor addComCallback(forte::com_infra::CComLayer* pa_pComCallback);
    void removeComCallback(TCallbackDescriptor pa_nCallbackDesc);

    void executeComCallback(TCallbackDescriptor pa_nCallbackDesc);

    /* functions needed for the external event handler interface */
    void enableHandler(void){
      start();
    }

    void disableHandler(void){
      end();
    }

    void setPriority(int){
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority(void) const{
      //the same as for setPriority
      return 0;
    }

  protected:
    virtual void run(void);

  private:
    ICmd* getNextCommand();

    struct TComContainer{
        TCallbackDescriptor m_nCallbackDesc;
        forte::com_infra::CComLayer* m_pCallback;
    };

    typedef CSinglyLinkedList<TComContainer> TCallbackList;
    TCallbackList m_lstComCallbacks;

    static TCallbackDescriptor m_nCallbackDescCount;

    CSyncObject m_oSync;

    typedef CSinglyLinkedList<ICmd*> TCommandQueue;
    TCommandQueue m_lCommandQueue;
};

#endif // OPCEVENTHANDLER_H_
