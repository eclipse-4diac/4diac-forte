/*******************************************************************************
 * Copyright (c) 2012 - 2015 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "muxedsercommlayer.h"
#include "commfb.h"
#include "../devlog.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <criticalregion.h>

using namespace forte::com_infra;

CMuxedSerCommLayer::CMuxedSerPortsManager CMuxedSerCommLayer::smMuxedSerPortsManager;

CMuxedSerCommLayer::CMuxedSerCommLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB) :
    CComLayer(paUpperLayer, paFB), mBufFillSize(0), mFD(scmInvalidFileDescriptor),
  mInterruptResp(forte::com_infra::EComResponse::e_Nothing), mSerMuxId(0){

}

CMuxedSerCommLayer::~CMuxedSerCommLayer(){
  closeConnection();
}

EComResponse CMuxedSerCommLayer::sendData(void *paData, unsigned int paSize){
  if(CFDSelectHandler::scmInvalidFileDescriptor != mFD){
    //first send id

    mRecvBuffer[0] = mSerMuxId;
    memcpy(&(mRecvBuffer[1]), paData, paSize);
    paSize++;

    ssize_t nToSend = paSize;
    while(0 < nToSend){
      ssize_t nSentBytes = write(mFD, &(mRecvBuffer[0]), nToSend);
      if(nSentBytes <= 0){
        DEVLOG_ERROR("CMuxedSerCommLayer: Send failed: %s\n", strerror(errno));
        return e_ProcessDataSendFailed;
      }
      nToSend -= nSentBytes;
      paData = static_cast<char*>(paData) + nSentBytes;
    }
  }

  return e_ProcessDataOk;
}

EComResponse CMuxedSerCommLayer::recvData(const void *, unsigned int){
  ssize_t nReadCount = read(mFD, &mRecvBuffer[mBufFillSize], cg_unIPLayerRecvBufferSize - mBufFillSize);

  switch (nReadCount){
    case 0:
      DEVLOG_INFO("CMuxedSerCommLayer: Connection closed by peer\n");
      mInterruptResp = e_InitTerminated;
      closeConnection();
      break;
    case -1:
      DEVLOG_ERROR("CMuxedSerCommLayer: read failed: %s\n", strerror(errno));
      mInterruptResp = e_ProcessDataRecvFaild;
      break;
    default:
      //we successfully received data
      mBufFillSize += nReadCount;
      mInterruptResp = e_ProcessDataOk;
      break;
  }

  m_poFb->interruptCommFB(this);
  return mInterruptResp;
}

EComResponse CMuxedSerCommLayer::processInterrupt(){
  if(e_ProcessDataOk == mInterruptResp){
    mInterruptResp = m_poTopLayer->recvData(mRecvBuffer, mBufFillSize);
    mBufFillSize = 0;
  }
  return mInterruptResp;
}

EComResponse CMuxedSerCommLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;

  char *acPort = strchr(paLayerParameter, ':');
  if(0 != acPort){
    *acPort = '\0';
    ++acPort;
    mSerMuxId = static_cast<TForteUInt8>(forte::core::util::strtoul(acPort, 0, 10));
    mFD = smMuxedSerPortsManager.addMuxedSerLayer(paLayerParameter, this);
    if(CFDSelectHandler::scmInvalidFileDescriptor != mFD){
      eRetVal = e_InitOk;
    }
  }

  return eRetVal;
}

void CMuxedSerCommLayer::closeConnection(){
  if(CFDSelectHandler::scmInvalidFileDescriptor != mFD){
    smMuxedSerPortsManager.removeMuxedSerLayer(mFD, this);
    mFD = CFDSelectHandler::scmInvalidFileDescriptor;
  }
}

//**************************************************************************************************************************
CMuxedSerCommLayer::CMuxedSerPortsManager::CMuxedSerPortsManager(){

}

CFDSelectHandler::TFileDescriptor CMuxedSerCommLayer::CMuxedSerPortsManager::addMuxedSerLayer(char* paSerPort, CMuxedSerCommLayer *paComCallBack){
  CCriticalRegion criticalRegion(mSync);
  CFDSelectHandler::TFileDescriptor nRetVal = CFDSelectHandler::scmInvalidFileDescriptor;
  SSerPortEntry *mSerPortEntry = getSerPortEntry(paSerPort);
  if(0 != mSerPortEntry){
    mSerPortEntry->mConnectionsList.push_back(paComCallBack);
    nRetVal = mSerPortEntry->mFD;
  }
  return nRetVal;
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::removeMuxedSerLayer(CFDSelectHandler::TFileDescriptor paFD, CMuxedSerCommLayer *paComCallBack){
  CCriticalRegion criticalRegion(mSync);
  SSerPortEntry *mSerPortEntry = getOpendSerPortEntry(paFD);
  if(0 != mSerPortEntry){
    TConnectionContainer::Iterator itRunner(mSerPortEntry->mConnectionsList.begin());
    TConnectionContainer::Iterator itRevNode(mSerPortEntry->mConnectionsList.end());
    TConnectionContainer::Iterator itEnd(mSerPortEntry->mConnectionsList.end());

    while(itRunner != itEnd){
      if((*itRunner) == paComCallBack){
        if(itRevNode == itEnd){
          mSerPortEntry->mConnectionsList.pop_front();
        }else{
           mSerPortEntry->mConnectionsList.eraseAfter(itRevNode);
        }
        break;
      }

      itRevNode = itRunner;
      ++itRunner;
    }

    if(mSerPortEntry->mConnectionsList.isEmpty()){
      closePort(mSerPortEntry);
    }
  }
}

EComResponse CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry::recvData(const void *, unsigned int){
  EComResponse eRetVal = e_Nothing;
  char nID;

  ssize_t nReadCount = read(mFD, &nID, 1);

  if(1 == nReadCount){

    TConnectionContainer::Iterator itEnd(mConnectionsList.end());
    for(TConnectionContainer::Iterator itRunner = mConnectionsList.begin(), itCurrent = mConnectionsList.begin(); itRunner != itEnd;){
      itCurrent = itRunner;
      ++itRunner;
      if(nID == itCurrent->getSerMuxId()){
        eRetVal = itCurrent->recvData(0,0);
        if(e_Nothing != eRetVal){
          m_poFb = itCurrent->getCommFB();
        }
        break;
      }
    }
  }
  else{
    //FIXME handle error close connections and inform FBs
  }

  return eRetVal;
}

EComResponse CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry::processInterrupt(){
  return e_Nothing;
}

CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry *CMuxedSerCommLayer::CMuxedSerPortsManager::getSerPortEntry(char* paSerPort){
  for(TSerPortList::Iterator itRunner = mPortList.begin(); itRunner != mPortList.end(); ++itRunner){
    if(0 == strcmp(itRunner->mSerPort, paSerPort)){
      return &(*itRunner);
    }
  }

  TSerPortList::Iterator itRef(mPortList.back());
//this port is not yet managed by us
  mPortList.push_back(SSerPortEntry());
  TSerPortList::Iterator itCurrent(mPortList.back());
  SSerPortEntry *pstRetVal = &(*itCurrent);
  openPort(paSerPort, pstRetVal);
  if(CFDSelectHandler::scmInvalidFileDescriptor == pstRetVal->mFD){
    if(itCurrent == mPortList.begin()){
      mPortList.pop_front();
    }
    else{
      mPortList.eraseAfter(itRef);
    }
    pstRetVal = 0;
  }
  else{
    pstRetVal->mSerPort = new char[strlen(paSerPort) + 1];
    strcpy(pstRetVal->mSerPort, paSerPort);
  }
  return pstRetVal;
}

CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry *CMuxedSerCommLayer::CMuxedSerPortsManager::getOpendSerPortEntry(CFDSelectHandler::TFileDescriptor paFD){
  SSerPortEntry *pstRetVal = 0;
  for(TSerPortList::Iterator itRunner = mPortList.begin(); itRunner != mPortList.end(); ++itRunner){
    if(paFD == itRunner->mFD){
      pstRetVal = &(*itRunner);
      break;
    }
  }
  return pstRetVal;
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::openPort(char* paSerPort, SSerPortEntry *paPortEntry){
  paPortEntry->mFD = open(paSerPort, O_RDWR | O_NOCTTY);

  if(CFDSelectHandler::scmInvalidFileDescriptor != paPortEntry->mFD){
    GET_HANDLER_FROM_COMM_LAYER(CFDSelectHandler)->addComCallback(paPortEntry->mFD, paPortEntry);
  }
  else{
    DEVLOG_ERROR("CSerCommLayer: open failed: %s\n", strerror(errno));
  }
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::closePort(SSerPortEntry *paSerPortEntry){
  GET_HANDLER_FROM_COMM_LAYER(CFDSelectHandler)->removeComCallback(paSerPortEntry->mFD);
  close(paSerPortEntry->mFD);

  delete[] paSerPortEntry->mSerPort;

  TSerPortList::Iterator itRunner(mPortList.begin());
  TSerPortList::Iterator itRefNode(mPortList.end());

  while(itRunner != mPortList.end()){
    if(itRunner->mFD == paSerPortEntry->mFD){
      if(itRefNode == mPortList.end()){
        mPortList.pop_front();
      }
      else{
        mPortList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}
