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
#ifndef _FDHAND_H_
#define _FDHAND_H_

#include "../core/fortelist.h"
#include "../core/extevhan.h"
#include <forte_thread.h>
#include <forte_sync.h>
#include <sockhand.h>

namespace forte{
  namespace com_infra{
    class CComCallback;
  }
}

/*!\brief An external event handler for file descriptor based external events.
 *
 * This handler uses a thread and the select function to monitor given file descriptors on
 * data reception. Examples for possible file descriptors are sockets or com re
 */

class CFDSelectHandler : public CExternalEventHandler, private CThread {
  DECLARE_HANDLER(CFDSelectHandler)
  public:
    typedef FORTE_SOCKET_TYPE TFileDescriptor; //!< General type definition for a file descriptor. To be used by the callback classes.
    static const TFileDescriptor scmInvalidFileDescriptor = FORTE_INVALID_SOCKET;

    void addComCallback(TFileDescriptor paFD, forte::com_infra::CComCallback *paComLayer);
    void removeComCallback(TFileDescriptor paFD);

    /* functions needed for the external event handler interface */
    void enableHandler(void){
      start();
    }

    void disableHandler(void){
      end();
    }

    void setPriority(int ){
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority(void) const {
      //the same as for setPriority
      return 0;
    }

  protected:
    virtual void run(void);

  private:
    struct TConnContType{
        TFileDescriptor mSockDes;
        forte::com_infra::CComCallback * mCallee;
    };

    typedef CSinglyLinkedList<TConnContType> TConnectionContainer;

    TFileDescriptor createFDSet(fd_set *m_panFDSet);

    TConnectionContainer mConnectionsList;
    CSyncObject mSync;
    bool mConnectionListChanged;
};

#endif
