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
#include "../forte_thread.h"
#include "../forte_sync.h"
#include "../../../core/extevhan.h"
#include "../../../core/fortelist.h"
#include "../forte_sem.h"

#ifndef CWIN32SERCOMHANDLER_H_
#define CWIN32SERCOMHANDLER_H_

class CWin32SerComLayer;

class CWin32SerComHandler : public CExternalEventHandler, public CThread{
  DECLARE_HANDLER(CWin32SerComHandler)
  public:
    void registerSerComLayer(CWin32SerComLayer *pa_poComLayer);
    void unregisterSerComLayer(CWin32SerComLayer *pa_poComLayer);

    /* functions needed for the external event handler interface */
    void enableHandler(void){
      start();
    }

    void disableHandler(void){
      setAlive(false);
      mSem.inc();
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

  private:
    void run();

    typedef CSinglyLinkedList<CWin32SerComLayer*> TCWin32SerComLayerContainer;

    TCWin32SerComLayerContainer mComLayerList;
    CSyncObject mSync;

    //!Sempahore for implementing a suspend feature similar to what we are doing in CEventChainExecutionThread
    forte::arch::CSemaphore mSem;
};

#endif /* CWIN32SERCOMLAYER_H_ */
