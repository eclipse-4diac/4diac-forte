/*******************************************************************************
 * Copyright (c) 2010 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <sockhand.h>      //needs to be first pulls in the platform specific includes
#include "fdselecthand.h"
#include "devlog.h"
#include "../core/devexec.h"
#include "../core/cominfra/commfb.h"
#include "../core/cominfra/comCallback.h"
#include "../core/utils/criticalregion.h"

DEFINE_HANDLER(CFDSelectHandler)
CFDSelectHandler::CFDSelectHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
  mConnectionListChanged = false;
#ifdef WIN32
  // Windows Socket Startupcode
  WORD wVersionRequested;
  WSADATA wsaData;

  /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
  wVersionRequested = MAKEWORD(2, 2);

  WSAStartup(wVersionRequested, &wsaData);
#endif
}

CFDSelectHandler::~CFDSelectHandler(){
  this->end();
#ifdef WIN32
  WSACleanup();
#endif
}

// single-threaded-network-code
void CFDSelectHandler::run(void){

  struct timeval tv;
  fd_set anFDSet;
  fd_set anFDSetMaster;

  TFileDescriptor nHighestFDID = scmInvalidFileDescriptor;
  int retval = 0;

  FD_ZERO(&anFDSetMaster);

  while(isAlive()){
    // TODO: create method to prevent 100ms timeout on reconnection
    mSync.lock();
    if(true == mConnectionListChanged){
      nHighestFDID = createFDSet(&anFDSetMaster);
    }
    anFDSet = anFDSetMaster;
    mSync.unlock();

    tv.tv_sec = 1; //TODO : To be set!
    tv.tv_usec = 0;

    if(scmInvalidFileDescriptor != nHighestFDID){
      retval = select(static_cast<int>(nHighestFDID + 1), &anFDSet, NULL, NULL, &tv);
      if(!isAlive()){
        //the thread has been closed in the meantime do not process any messages anymore
        return;
      }
    }
    else{
      retval = 0;
    }

    if(retval > 0){
      mSync.lock();
      TConnectionContainer::Iterator itEnd(mConnectionsList.end());
      for(TConnectionContainer::Iterator itRunner = mConnectionsList.begin(); itRunner != itEnd;){
        // need to retrieve the callee as the iterator may get invalid in the recvDat function below in case of connection closing
        forte::com_infra::CComCallback *callee = itRunner->mCallee;
        TFileDescriptor sockDes = itRunner->mSockDes;
        ++itRunner;

        if((0 != FD_ISSET(sockDes, &anFDSet)) && (0 != callee)){
          mSync.unlock();
          if(forte::com_infra::e_Nothing != callee->recvData(&sockDes,0)){
            startNewEventChain(callee->getCommFB());
          }
          mSync.lock();
        }
      }
      mSync.unlock();
    }
    else{
      if(retval != 0) {
#ifdef WIN32
        DEVLOG_ERROR("Select failed: %d", WSAGetLastError());
#else
        DEVLOG_ERROR("Select failed: %s", strerror(errno));
#endif
      }
    }
  }
}

void CFDSelectHandler::addComCallback(TFileDescriptor paFD, forte::com_infra::CComCallback *paComCallback){
  {
    CCriticalRegion criticalRegion(mSync);
    TConnContType stNewNode = { paFD, paComCallback };
    mConnectionsList.pushBack(stNewNode);
    mConnectionListChanged = true;
  }
  if(!isAlive()){
    this->start();
  }
}

void CFDSelectHandler::removeComCallback(TFileDescriptor paFD){
  CCriticalRegion criticalRegion(mSync);

  TConnectionContainer::Iterator itRunner(mConnectionsList.begin());
  TConnectionContainer::Iterator itRefNode(mConnectionsList.end());
  TConnectionContainer::Iterator itEnd(mConnectionsList.end());

  while(itRunner != itEnd){
    if(itRunner->mSockDes == paFD){
      if(itRefNode ==itEnd){
        mConnectionsList.popFront();
      }
      else{
        mConnectionsList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }

  mConnectionListChanged = true;
}

CFDSelectHandler::TFileDescriptor CFDSelectHandler::createFDSet(fd_set *m_panFDSet){
  TFileDescriptor nRetVal = scmInvalidFileDescriptor;
  FD_ZERO(m_panFDSet);
  TConnectionContainer::Iterator itEnd(mConnectionsList.end());
  for(TConnectionContainer::Iterator itRunner = mConnectionsList.begin(); itRunner != itEnd; ++itRunner){
    FD_SET(itRunner->mSockDes, m_panFDSet);
    if(itRunner->mSockDes > nRetVal || scmInvalidFileDescriptor == nRetVal){
      nRetVal = itRunner->mSockDes;
    }
  }
  mConnectionListChanged = false;
  return nRetVal;
}
