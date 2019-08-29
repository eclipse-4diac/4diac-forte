/*******************************************************************************
 * Copyright (c) 2012 -2015 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _MODBUSHANDLER_H_
#define _MODBUSHANDLER_H_

#include <forte_config.h>
#include "extevhan.h"
#include <forte_sync.h>
#include <comlayer.h>
#include <fortelist.h>

class CModbusHandler : public CExternalEventHandler{
    DECLARE_HANDLER(CModbusHandler)
  public:
    typedef int TCallbackDescriptor;

    void enableHandler(void){
    }
    ;
    void disableHandler(void){
    }
    ;

    void setPriority(int){
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority(void) const{
      //the same as for setPriority
      return 0;
    }

    TCallbackDescriptor addComCallback(forte::com_infra::CComLayer* pa_pComCallback);
    void removeComCallback(TCallbackDescriptor pa_nCallbackDesc);

    void executeComCallback(TCallbackDescriptor pa_nCallbackDesc);

  private:
    struct TComContainer{
        TCallbackDescriptor m_nCallbackDesc;
        forte::com_infra::CComLayer* m_pCallback;
    };

    typedef CSinglyLinkedList<TComContainer> TCallbackList;
    TCallbackList m_lstComCallbacks;

    CSyncObject m_oSync;

    static TCallbackDescriptor m_nCallbackDescCount;
};

#endif // _MODBUSHANDLER_H_
