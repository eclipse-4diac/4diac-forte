/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - adds typifyAnyAdapter
 *******************************************************************************/
#ifndef _ADAPTERCONN_H_
#define _ADAPTERCONN_H_

#include "conn.h"

class CAdapter;

/*! \ingroup CORE\brief Class for handling a adapter connection.
 */
class CAdapterConnection : public CConnection{
  public:
    CAdapterConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CAdapter &paPlug);
    ~CAdapterConnection() override;

    EMGMResponse connect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) override;
    EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse disconnect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    void getSourcePortName(forte::core::TNameIdentifier &paResult) const override;

    bool isConnected() const {
      return mSocket != nullptr;
    }

    CAdapter &getPlug(){
      return mPlug;
    }
    
    const CAdapter &getPlug() const {
      return mPlug;
    }

    void setSocket(CAdapter *paSocket){
      mSocket = paSocket;
    }

    CAdapter *getSocket(){
      return mSocket;
    }

  private:

    void typifyAnyAdapter(CAdapter *paSocket);
    void performDisconnect();

    CAdapter &mPlug;
    CAdapter *mSocket;

};

typedef CAdapterConnection *TAdapterConnectionPtr;

#endif /*_ADAPTERCONN_H_*/
