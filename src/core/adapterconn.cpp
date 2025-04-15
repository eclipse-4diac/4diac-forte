/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
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

USE_STRING_ID(ANY_ADAPTER);

#include "funcbloc.h"
#include "adapter.h"
#include "anyadapter.h"

CAdapterConnection::CAdapterConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CAdapter &paPlug) :
    CConnection(paSrcFB, paSrcPortId), mPlug(paPlug), mSocket(nullptr){
}

CAdapterConnection::~CAdapterConnection(){
  performDisconnect();
}

void CAdapterConnection::typifyAnyAdapter(CAdapter *paSocket){
  if(STRID(ANY_ADAPTER) == paSocket->getFBTypeId()){
    static_cast<CAnyAdapter*>(paSocket)->typifyAnyAdapter(mPlug);
  }

  if(STRID(ANY_ADAPTER) == mPlug.getFBTypeId()){
    static_cast<CAnyAdapter&>(mPlug).typifyAnyAdapter(*paSocket);
  }
}

EMGMResponse CAdapterConnection::connect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) {
  const TPortId portId = paDstFB.getAdapterPortId(paDstPortNameId);
  if (portId == cgInvalidPortId) {
    return EMGMResponse::NoSuchObject;
  }

  if (isConnected()) {
    return EMGMResponse::InvalidState;
  }

  CAdapter *socket = paDstFB.getAdapter(paDstPortNameId);
  typifyAnyAdapter(socket);

  if (!socket->isSocket() || !socket->isCompatible(mPlug)) {
    return EMGMResponse::InvalidObject;
  }

  if (mPlug.connect(socket, this) && socket->connect(&mPlug, this)) {
    mSocket = socket;
    paDstFB.incConnRefCount();
    getSourceId().getFB().incConnRefCount();
    return EMGMResponse::Ready;
  }
  mPlug.disconnect();
  socket->disconnect();
  return EMGMResponse::InvalidObject;
}

EMGMResponse CAdapterConnection::connectToCFBInterface(CFunctionBlock &, CStringDictionary::TStringId){
  return EMGMResponse::InvalidOperation; //currently we are not supporting adapter connections accross interface boundaries
}

EMGMResponse CAdapterConnection::disconnect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) {
  if (isConnected()) {
    CAdapter *socket = paDstFB.getAdapter(paDstPortNameId);
    if (socket == mSocket) {
      performDisconnect();
      paDstFB.decConnRefCount();
      getSourceId().getFB().decConnRefCount();
      return EMGMResponse::Ready;
    }
  }
  return EMGMResponse::NoSuchObject;
}

void CAdapterConnection::performDisconnect(){
  mPlug.disconnect(this);

  if(mSocket != nullptr){
    mSocket->disconnect(this);
    mSocket = nullptr;
  }
}
