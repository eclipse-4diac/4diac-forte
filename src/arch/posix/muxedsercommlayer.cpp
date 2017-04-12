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

CMuxedSerCommLayer::CMuxedSerPortsManager CMuxedSerCommLayer::sm_oMuxedSerPortsManager;

CMuxedSerCommLayer::CMuxedSerCommLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poFB) :
    CComLayer(pa_poUpperLayer, pa_poFB), m_unBufFillSize(0), m_nFD(scm_nInvalidFileDescriptor),
	m_eInterruptResp(forte::com_infra::EComResponse::e_Nothing), m_unSerMuxId(0){

}

CMuxedSerCommLayer::~CMuxedSerCommLayer(){
  closeConnection();
}

EComResponse CMuxedSerCommLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
    //first send id

    m_acRecvBuffer[0] = m_unSerMuxId;
    memcpy(&(m_acRecvBuffer[1]), pa_pvData, pa_unSize);
    pa_unSize++;

    ssize_t nToSend = pa_unSize;
    while(0 < nToSend){
      ssize_t nSentBytes = write(m_nFD, &(m_acRecvBuffer[0]), nToSend);
      if(nSentBytes <= 0){
        DEVLOG_ERROR("CMuxedSerCommLayer: Send failed: %s\n", strerror(errno));
        return e_ProcessDataSendFailed;
      }
      nToSend -= nSentBytes;
      pa_pvData = static_cast<char*>(pa_pvData) + nSentBytes;
    }
  }

  return e_ProcessDataOk;
}

EComResponse CMuxedSerCommLayer::recvData(const void *, unsigned int){
  ssize_t nReadCount = read(m_nFD, &m_acRecvBuffer[m_unBufFillSize], cg_unIPLayerRecvBufferSize - m_unBufFillSize);

  switch (nReadCount){
    case 0:
      DEVLOG_INFO("CMuxedSerCommLayer: Connection closed by peer\n");
      m_eInterruptResp = e_InitTerminated;
      closeConnection();
      break;
    case -1:
      DEVLOG_ERROR("CMuxedSerCommLayer: read failed: %s\n", strerror(errno));
      m_eInterruptResp = e_ProcessDataRecvFaild;
      break;
    default:
      //we successfully received data
      m_unBufFillSize += nReadCount;
      m_eInterruptResp = e_ProcessDataOk;
      break;
  }

  m_poFb->interruptCommFB(this);
  return m_eInterruptResp;
}

EComResponse CMuxedSerCommLayer::processInterrupt(){
  if(e_ProcessDataOk == m_eInterruptResp){
    m_eInterruptResp = m_poTopLayer->recvData(m_acRecvBuffer, m_unBufFillSize);
    m_unBufFillSize = 0;
  }
  return m_eInterruptResp;
}

EComResponse CMuxedSerCommLayer::openConnection(char *pa_acLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;

  char *acPort = strchr(pa_acLayerParameter, ':');
  if(0 != acPort){
    *acPort = '\0';
    ++acPort;
    m_unSerMuxId = static_cast<TForteUInt8>(forte::core::util::strtoul(acPort, 0, 10));
    m_nFD = sm_oMuxedSerPortsManager.addMuxedSerLayer(pa_acLayerParameter, this);
    if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
      eRetVal = e_InitOk;
    }
  }

  return eRetVal;
}

void CMuxedSerCommLayer::closeConnection(){
  if(CFDSelectHandler::scm_nInvalidFileDescriptor != m_nFD){
    sm_oMuxedSerPortsManager.removeMuxedSerLayer(m_nFD, this);
    m_nFD = CFDSelectHandler::scm_nInvalidFileDescriptor;
  }
}

//**************************************************************************************************************************
CMuxedSerCommLayer::CMuxedSerPortsManager::CMuxedSerPortsManager(){

}

CFDSelectHandler::TFileDescriptor CMuxedSerCommLayer::CMuxedSerPortsManager::addMuxedSerLayer(char* pa_acSerPort, CMuxedSerCommLayer *pa_poComCallBack){
  CCriticalRegion criticalRegion(m_oSync);
  CFDSelectHandler::TFileDescriptor nRetVal = CFDSelectHandler::scm_nInvalidFileDescriptor;
  SSerPortEntry *m_pstSerPortEntry = getSerPortEntry(pa_acSerPort);
  if(0 != m_pstSerPortEntry){
    m_pstSerPortEntry->m_lstConnectionsList.push_back(pa_poComCallBack);
    nRetVal = m_pstSerPortEntry->m_nFD;
  }
  return nRetVal;
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::removeMuxedSerLayer(CFDSelectHandler::TFileDescriptor m_nFD, CMuxedSerCommLayer *pa_poComCallBack){
  CCriticalRegion criticalRegion(m_oSync);
  SSerPortEntry *m_pstSerPortEntry = getOpendSerPortEntry(m_nFD);
  if(0 != m_pstSerPortEntry){
    TConnectionContainer::Iterator itRunner(m_pstSerPortEntry->m_lstConnectionsList.begin());
    TConnectionContainer::Iterator itRevNode(m_pstSerPortEntry->m_lstConnectionsList.end());
    TConnectionContainer::Iterator itEnd(m_pstSerPortEntry->m_lstConnectionsList.end());

    while(itRunner != itEnd){
      if((*itRunner) == pa_poComCallBack){
        if(itRevNode == itEnd){
          m_pstSerPortEntry->m_lstConnectionsList.pop_front();
        }else{
           m_pstSerPortEntry->m_lstConnectionsList.eraseAfter(itRevNode);
        }
        break;
      }

      itRevNode = itRunner;
      ++itRunner;
    }

    if(m_pstSerPortEntry->m_lstConnectionsList.isEmpty()){
      closePort(m_pstSerPortEntry);
    }
  }
}

EComResponse CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry::recvData(const void *, unsigned int){
  EComResponse eRetVal = e_Nothing;
  char nID;

  ssize_t nReadCount = read(m_nFD, &nID, 1);

  if(1 == nReadCount){

    TConnectionContainer::Iterator itEnd(m_lstConnectionsList.end());
    for(TConnectionContainer::Iterator itRunner = m_lstConnectionsList.begin(), itCurrent = m_lstConnectionsList.begin(); itRunner != itEnd;){
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

CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry *CMuxedSerCommLayer::CMuxedSerPortsManager::getSerPortEntry(char* pa_acSerPort){
  for(TSerPortList::Iterator itRunner = m_lstPortList.begin(); itRunner != m_lstPortList.end(); ++itRunner){
    if(0 == strcmp(itRunner->m_acSerPort, pa_acSerPort)){
      return &(*itRunner);
    }
  }

  TSerPortList::Iterator itRef(m_lstPortList.back());
//this port is not yet managed by us
  m_lstPortList.push_back(SSerPortEntry());
  TSerPortList::Iterator itCurrent(m_lstPortList.back());
  SSerPortEntry *pstRetVal = &(*itCurrent);
  openPort(pa_acSerPort, pstRetVal);
  if(CFDSelectHandler::scm_nInvalidFileDescriptor == pstRetVal->m_nFD){
    if(itCurrent == m_lstPortList.begin()){
      m_lstPortList.pop_front();
    }
    else{
      m_lstPortList.eraseAfter(itRef);
    }
    pstRetVal = 0;
  }
  else{
    pstRetVal->m_acSerPort = new char[strlen(pa_acSerPort) + 1];
    strcpy(pstRetVal->m_acSerPort, pa_acSerPort);
  }
  return pstRetVal;
}

CMuxedSerCommLayer::CMuxedSerPortsManager::SSerPortEntry *CMuxedSerCommLayer::CMuxedSerPortsManager::getOpendSerPortEntry(CFDSelectHandler::TFileDescriptor pa_nFD){
  SSerPortEntry *pstRetVal = 0;
  for(TSerPortList::Iterator itRunner = m_lstPortList.begin(); itRunner != m_lstPortList.end(); ++itRunner){
    if(pa_nFD == itRunner->m_nFD){
      pstRetVal = &(*itRunner);
      break;
    }
  }
  return pstRetVal;
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::openPort(char* pa_acSerPort, SSerPortEntry *pa_pstPortEntry){
  pa_pstPortEntry->m_nFD = open(pa_acSerPort, O_RDWR | O_NOCTTY);

  if(CFDSelectHandler::scm_nInvalidFileDescriptor != pa_pstPortEntry->m_nFD){
    CFDSelectHandler::getInstance().addComCallback(pa_pstPortEntry->m_nFD, pa_pstPortEntry);
  }
  else{
    DEVLOG_ERROR("CSerCommLayer: open failed: %s\n", strerror(errno));
  }
}

void CMuxedSerCommLayer::CMuxedSerPortsManager::closePort(SSerPortEntry *pa_pstSerPortEntry){
  CFDSelectHandler::getInstance().removeComCallback(pa_pstSerPortEntry->m_nFD);
  close(pa_pstSerPortEntry->m_nFD);

  delete[] pa_pstSerPortEntry->m_acSerPort;

  TSerPortList::Iterator itRunner(m_lstPortList.begin());
  TSerPortList::Iterator itRefNode(m_lstPortList.end());

  while(itRunner != m_lstPortList.end()){
    if(itRunner->m_nFD == pa_pstSerPortEntry->m_nFD){
      if(itRefNode == m_lstPortList.end()){
        m_lstPortList.pop_front();
      }
      else{
        m_lstPortList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}
