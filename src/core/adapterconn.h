/*******************************************************************************
  * Copyright (c) 2008 - 2015 ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Ingo Hegny, Alois Zoitl, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ADAPTERCONN_H_
#define _ADAPTERCONN_H_

#include "conn.h"

class CAdapter;

/*! \ingroup CORE\brief Class for handling a adapter connection.
 */
class CAdapterConnection : public CConnection{
  public:
    CAdapterConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CAdapter *paPlug);
    virtual ~CAdapterConnection();

    virtual EMGMResponse connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId);
    virtual EMGMResponse connectToCFBInterface(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId);

#ifndef FORTE_CLASS_0
    virtual EMGMResponse disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId);
#endif

    CAdapter *getPlug(){
      return mPlug;
    }

    void setSocket(CAdapter *pa_poSocket){
      mSocket = pa_poSocket;
    }

    CAdapter *getSocket(){
      return mSocket;
    }

  private:
    void performDisconnect();

    CAdapter *mPlug;
    CAdapter *mSocket;

};

typedef CAdapterConnection *TAdapterConnectionPtr;

#endif /*_ADAPTERCONN_H_*/
