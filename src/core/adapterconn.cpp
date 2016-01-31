/*******************************************************************************
  * Copyright (c) 2008 - 2015 ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Ingo Hegny, Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "adapterconn.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "adapterconn_gen.cpp"
#endif
#include "funcbloc.h"
#include "adapter.h"

CAdapterConnection::CAdapterConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId,
    CAdapter *paPlug) :
    CConnection(paSrcFB, paSrcPortId),
        mPlug(paPlug),
        mSocket(0){
}

CAdapterConnection::~CAdapterConnection(){
  performDisconnect();
}

EMGMResponse CAdapterConnection::connect(CFunctionBlock *paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  TPortId portId = paDstFB->getAddapterPortId(paDstPortNameId);
  if(cg_unInvalidPortId != portId){
    if(!isConnected()){
      CAdapter *socket = paDstFB->getAdapter(paDstPortNameId);
      if((socket->isSocket()) && (socket->isCompatible(mPlug))){
        if(((mPlug->connect(socket, this)) && (socket->connect(mPlug, this)))){
          mSocket = socket;
          addDestination(SConnectionPoint(paDstFB, portId));
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

EMGMResponse CAdapterConnection::connectToCFBInterface(CFunctionBlock *, CStringDictionary::TStringId ){
  return e_INVALID_OPERATION; //currently we are not supporting adapter connections accross interface boundaries
}

#ifndef FORTE_CLASS_0
EMGMResponse CAdapterConnection::disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = e_NO_SUCH_OBJECT;

  TPortId portId = paDstFB->getAddapterPortId(paDstPortNameId);
  if(cg_unInvalidPortId != portId){
    retVal = CConnection::removeDestination(SConnectionPoint(paDstFB, portId));
    if(e_RDY == retVal){
      performDisconnect();
    }
  }
  return retVal;
}

#endif

void CAdapterConnection::performDisconnect(){
  if(mPlug != 0){
    mPlug->disconnect(this);
  }

  if(mSocket != 0){
    mSocket->disconnect(this);
    mSocket = 0;
  }
}
