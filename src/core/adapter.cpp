/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes connect, prepares for working AnyAdapter
 *******************************************************************************/
#include <string.h>
#include "adapter.h"

USE_STRING_ID(ANY_ADAPTER);

#include "adapterconn.h"
#include "ecet.h"

CAdapter::CAdapter(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpecSocket, const CStringDictionary::TStringId paInstanceNameId, const SFBInterfaceSpec& paInterfaceSpecPlug, bool paIsPlug) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paIsPlug ? paInterfaceSpecPlug : paInterfaceSpecSocket, paInstanceNameId),
    mOutputEventIds(getFBInterfaceSpec().mNumEOs, cgInvalidEventID),
    mParentAdapterListEventID(0),
    mIsPlug(paIsPlug),
    mPeer(nullptr),
    mLocalDIs(mDIs),
    mAdapterConn(nullptr),
    mParentFB(nullptr) {
}

bool CAdapter::initialize() {
  if(!CGenFunctionBlock<CFunctionBlock>::initialize()) {
    return false;
  }
  mLocalDIs = mDIs;
  return true;
}

CAdapter::~CAdapter(){
  mDIs = mLocalDIs;
  if (mIsPlug) {
    if (mAdapterConn != nullptr) {
      delete mAdapterConn;
    }
  } else {
    if (mAdapterConn != nullptr) {
      mAdapterConn->setSocket(nullptr);
    }
  }
}

bool CAdapter::createInterfaceSpec(const char *, SFBInterfaceSpec &) {
  return true;
}

void CAdapter::fillEventEntryList(){
  for (TEventID i = 0; i < getFBInterfaceSpec().mNumEOs; ++i) {
    mOutputEventIds[i] = static_cast<TPortId>(mParentAdapterListEventID + i);
  }
}

void CAdapter::setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID){
  mParentAdapterListEventID = static_cast<TForteUInt16>((paParentAdapterlistID + 1) << 8);

  mParentFB = paParentFB;
  fillEventEntryList();

  if (isPlug()) {
    //the plug is in charge of managing the adapter connection
    mAdapterConn = new CAdapterConnection(*paParentFB, paParentAdapterlistID, *this);
  }
}

bool CAdapter::connect(CAdapter *paPeer, CAdapterConnection *paAdConn){
  bool bRetVal = false;
  if (mPeer == nullptr) {
    mPeer = paPeer;
    mDIs = paPeer->mDOs; //TODO: change is adapters of subtypes may be connected
    if (isSocket()) {
      mAdapterConn = paAdConn;
    }
    bRetVal = true;
  }

  return bRetVal;
}

bool CAdapter::disconnect(CAdapterConnection *paAdConn){
  if ((paAdConn != nullptr) && (paAdConn != mAdapterConn)) {
    return false; //connection requesting disconnect is not equal to established connection
  }
  mPeer = nullptr;
  mDIs = mLocalDIs;
  if (isSocket()) {
    mAdapterConn = nullptr;
  }
  return true;
}

bool CAdapter::isCompatible(const CAdapter &paPeer) const {
  //Need to check any adapter here as we don't know which adapter side is used for checking compatibility
  return ((getFBTypeId() == paPeer.getFBTypeId()) || ((getFBTypeId() == STRID(ANY_ADAPTER)) && (STRID(ANY_ADAPTER) != paPeer.getFBTypeId())) || ((getFBTypeId() != STRID(ANY_ADAPTER)) && (STRID(ANY_ADAPTER) == paPeer.getFBTypeId())));
}

void CAdapter::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET){
  if (mPeer != nullptr) {
    if (mPeer->mParentFB != nullptr) {
      paECET->addEventEntry(TEventEntry(*mPeer->mParentFB, mPeer->mOutputEventIds[paEIID]));
    } else {
      mPeer->sendOutputEvent(paEIID, paECET);
    }
  }
}

