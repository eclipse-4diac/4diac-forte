/*******************************************************************************
 * Copyright (c) 2008 - 2015 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - adds typifyAnyAdapter
 *******************************************************************************/
#include "adapterconn.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "adapterconn_gen.cpp"
#endif
#include "funcbloc.h"
#include "adapter.h"
#include "anyadapter.h"

CAdapterConnection::CAdapterConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CAdapter *paPlug) :
    CConnection(paSrcFB, paSrcPortId), mPlug(paPlug), mSocket(0){
}

CAdapterConnection::~CAdapterConnection(){
  performDisconnect();
}

void CAdapterConnection::typifyAnyAdapter(CAdapter *paSocket, CAdapter *paPlug){
  if(g_nStringIdANY_ADAPTER == paSocket->getFBTypeId()){
    static_cast<CAnyAdapter*>(paSocket)->typifyAnyAdapter(paPlug);
  }

  if(g_nStringIdANY_ADAPTER == paPlug->getFBTypeId()){
    static_cast<CAnyAdapter*>(paPlug)->typifyAnyAdapter(paSocket);
  }
}

EMGMResponse CAdapterConnection::connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  TPortId portId = paDstFB->getAdapterPortId(paDstPortNameId);
  if(cg_unInvalidPortId != portId){
    if(!isConnected()){
      CAdapter *socket = paDstFB->getAdapter(paDstPortNameId);
      typifyAnyAdapter(socket, mPlug);

      if((socket->isSocket()) && (socket->isCompatible(mPlug))){
        if(mPlug->connect(socket, this) && socket->connect(mPlug, this)) {
          mSocket = socket;
          addDestination(CConnectionPoint(paDstFB, portId));
          retVal = e_RDY;
        }
        else{
          mPlug->disconnect();
          socket->disconnect();
          retVal = e_INVALID_OBJECT;
        }
      }
      else{
        retVal = e_INVALID_OBJECT;
      }
    }
    else{
      retVal = e_INVALID_STATE;
    }
  }
  return retVal;
}

EMGMResponse CAdapterConnection::connectToCFBInterface(CFunctionBlock *, CStringDictionary::TStringId){
  return e_INVALID_OPERATION; //currently we are not supporting adapter connections accross interface boundaries
}

EMGMResponse CAdapterConnection::disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  TPortId portId = paDstFB->getAdapterPortId(paDstPortNameId);
  if(cg_unInvalidPortId != portId){
    retVal = CConnection::removeDestination(CConnectionPoint(paDstFB, portId));
    if(e_RDY == retVal){
      performDisconnect();
    }
  }
  return retVal;
}

void CAdapterConnection::performDisconnect(){
  if(mPlug != 0){
    mPlug->disconnect(this);
  }

  if(mSocket != 0){
    mSocket->disconnect(this);
    mSocket = 0;
  }
}
