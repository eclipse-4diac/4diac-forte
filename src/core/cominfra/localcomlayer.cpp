/*******************************************************************************
  * Copyright (c) 2011-2014 fortiss and TU Wien ACIN.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation and bug fixes
  *    Patrik Smejkal - rename interrupt in interruptCCommFB
  *******************************************************************************/
#include "localcomlayer.h"
#include "../resource.h"
#include "../device.h"
#include "../utils/criticalregion.h"

using namespace forte::com_infra;

CLocalComLayer::CLocalCommGroupsManager CLocalComLayer::sm_oLocalCommGroupsManager;

CLocalComLayer::CLocalComLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poFB) :
  CComLayer(pa_poUpperLayer, pa_poFB), m_poLocalCommGroup(0){
}

CLocalComLayer::~CLocalComLayer(){
  closeConnection();
}

EComResponse CLocalComLayer::sendData(void *, unsigned int){
  CCriticalRegion criticalRegion(m_poFb->getResource().m_oResDataConSync);
  CIEC_ANY *aSDs = m_poFb->getSDs();
  unsigned int unNumSDs = m_poFb->getNumSD();

  // go through GroupList and trigger all Subscribers
  for(CSinglyLinkedList<CLocalComLayer*>::Iterator listiter(m_poLocalCommGroup->m_lSublList.begin()); listiter != m_poLocalCommGroup->m_lSublList.end(); ++listiter){
    setRDs((*listiter), aSDs, unNumSDs);
  }
  return e_ProcessDataOk;
}

void CLocalComLayer::setRDs(CLocalComLayer *pa_poSublLayer, CIEC_ANY *pa_aSDs, unsigned int pa_unNumSDs){
  CSyncObject *poTargetResDataConSync = 0;
  if(m_poFb->getResourcePtr() != pa_poSublLayer->m_poFb->getResourcePtr()){
    poTargetResDataConSync = &(pa_poSublLayer->m_poFb->getResourcePtr()->m_oResDataConSync);
    poTargetResDataConSync->lock();
  }

  CIEC_ANY *aRDs = pa_poSublLayer->m_poFb->getRDs();

  for(unsigned int i = 0; (i < pa_unNumSDs) && (i < pa_poSublLayer->m_poFb->getNumRD()); ++i){
    if(aRDs[i].getDataTypeID() == pa_aSDs[i].getDataTypeID()){
      aRDs[i].setValue(pa_aSDs[i]);
    }
  }

  pa_poSublLayer->m_poFb->interruptCommFB(pa_poSublLayer);
  m_poFb->getResource().getDevice().getDeviceExecution().startNewEventChain(pa_poSublLayer->m_poFb);

  if(0 != poTargetResDataConSync){
    poTargetResDataConSync->unlock();
  }
}

EComResponse CLocalComLayer::openConnection(char *pa_acLayerParameter){
  CStringDictionary::TStringId nId = CStringDictionary::getInstance().insert(pa_acLayerParameter);

  switch (m_poFb->getComServiceType()){
    case e_Server:
    case e_Client:
      break;
    case e_Publisher:
      m_poLocalCommGroup = sm_oLocalCommGroupsManager.registerPubl(nId, this);
      break;
    case e_Subscriber:
      m_poLocalCommGroup = sm_oLocalCommGroupsManager.registerSubl(nId, this);
      break;
  }
  return (0 != m_poLocalCommGroup) ? e_InitOk : e_InitInvalidId;
}

void CLocalComLayer::closeConnection(){
  if(0 != m_poLocalCommGroup){
    if(e_Publisher == m_poFb->getComServiceType()){
      sm_oLocalCommGroupsManager.unregisterPubl(m_poLocalCommGroup, this);
    }
    else{
      sm_oLocalCommGroupsManager.unregisterSubl(m_poLocalCommGroup, this);
    }
    m_poLocalCommGroup = 0;
  }
}

/********************** CLocalCommGroupsManager *************************************/
CLocalComLayer::SLocalCommGroup* CLocalComLayer::CLocalCommGroupsManager::registerPubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer){
  CCriticalRegion criticalRegion(m_oSync);
  SLocalCommGroup *poGroup = findLocalCommGroup(pa_nID);
  if(0 == poGroup){
    poGroup = createLocalCommGroup(pa_nID);
  }
  poGroup->m_lPublList.push_back(pa_poLayer);

  return poGroup;
}

void CLocalComLayer::CLocalCommGroupsManager::unregisterPubl(SLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer){
  CCriticalRegion criticalRegion(m_oSync);
  removeListEntry(pa_poGroup->m_lPublList, pa_poLayer);

  if((pa_poGroup->m_lPublList.isEmpty()) && (pa_poGroup->m_lSublList.isEmpty())){
    removeCommGroup(pa_poGroup);
  }

}

CLocalComLayer::SLocalCommGroup* CLocalComLayer::CLocalCommGroupsManager::registerSubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer){
  CCriticalRegion criticalRegion(m_oSync);
  SLocalCommGroup *poGroup = findLocalCommGroup(pa_nID);
  if(0 == poGroup){
    poGroup = createLocalCommGroup(pa_nID);
  }
  poGroup->m_lSublList.push_back(pa_poLayer);

  return poGroup;
}

void CLocalComLayer::CLocalCommGroupsManager::unregisterSubl(SLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer){
  CCriticalRegion criticalRegion(m_oSync);
  removeListEntry(pa_poGroup->m_lSublList, pa_poLayer);

  if((pa_poGroup->m_lPublList.isEmpty()) && (pa_poGroup->m_lSublList.isEmpty())){
    removeCommGroup(pa_poGroup);
  }
}

CLocalComLayer::SLocalCommGroup* CLocalComLayer::CLocalCommGroupsManager::findLocalCommGroup(CStringDictionary::TStringId pa_nID){
  SLocalCommGroup *poGroup = 0;

  if(!m_lstLocalCommGroups.isEmpty()){
    CSinglyLinkedList<SLocalCommGroup>::Iterator it = m_lstLocalCommGroups.begin();
    while(it != m_lstLocalCommGroups.end()){
      if((*it).m_nGroupName == pa_nID){
        poGroup = &(*it);
        break;
      }
      ++it;
    }
  }

  return poGroup;
}

CLocalComLayer::SLocalCommGroup* CLocalComLayer::CLocalCommGroupsManager::createLocalCommGroup(CStringDictionary::TStringId pa_nID){
  m_lstLocalCommGroups.push_front(SLocalCommGroup(pa_nID));
  CSinglyLinkedList<SLocalCommGroup>::Iterator it = m_lstLocalCommGroups.begin();
  return &(*it);
}

void CLocalComLayer::CLocalCommGroupsManager::removeListEntry(CSinglyLinkedList<CLocalComLayer*> &pa_rlstList, CLocalComLayer *pa_poLayer){
  CSinglyLinkedList<CLocalComLayer*>::Iterator itRunner = pa_rlstList.begin();
  CSinglyLinkedList<CLocalComLayer*>::Iterator itRevNode = pa_rlstList.end();

  while(itRunner != pa_rlstList.end()){
    if((*itRunner) == pa_poLayer){
      if(itRevNode == pa_rlstList.end()){
        pa_rlstList.pop_front();
      }
      else{
        pa_rlstList.eraseAfter(itRevNode);
      }
      break;
    }
    itRevNode = itRunner;
    ++itRunner;
  }
}

void CLocalComLayer::CLocalCommGroupsManager::removeCommGroup(SLocalCommGroup *pa_poGroup){
  CSinglyLinkedList<SLocalCommGroup>::Iterator itRunner = m_lstLocalCommGroups.begin();
  CSinglyLinkedList<SLocalCommGroup>::Iterator itRevNode = m_lstLocalCommGroups.end();

  while(itRunner != m_lstLocalCommGroups.end()){
    if((*itRunner).m_nGroupName == pa_poGroup->m_nGroupName){
      if(itRevNode == m_lstLocalCommGroups.end()){
        m_lstLocalCommGroups.pop_front();
      }
      else{
        m_lstLocalCommGroups.eraseAfter(itRevNode);
      }
      break;
    }
    itRevNode = itRunner;
    ++itRunner;
  }
}
