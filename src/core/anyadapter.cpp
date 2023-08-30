/*******************************************************************************
 * Copyright (c) 2013, 2014 fortiss GmbH, 2018 TU Vienna/ACIN
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - fixes event chain initialisation, adds typifyAnyAdapter
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include "anyadapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "anyadapter_gen.cpp"
#endif

DEFINE_GENERIC_ADAPTER_TYPE(CAnyAdapter, g_nStringIdANY_ADAPTER)

const SFBInterfaceSpec CAnyAdapter::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

CAnyAdapter::CAnyAdapter(CStringDictionary::TStringId paAdapterInstanceName, CResource *paSrcRes, bool paIsPlug) :
    CAdapter(paSrcRes, &scmFBInterfaceSpec, paAdapterInstanceName, &scmFBInterfaceSpec, paIsPlug), m_ParentFB(nullptr),
    mParentAdapterlistID(0){
  memset(&mCurrentFBInterfaceSpec, 0, sizeof(SFBInterfaceSpec));
}

CAnyAdapter::~CAnyAdapter() = default;

void CAnyAdapter::typifyAnyAdapter(CAdapter *paPeer){
  mCurrentFBInterfaceSpec.mNumEIs = paPeer->getAdapterInterfaceSpec()->mNumEOs;
  mCurrentFBInterfaceSpec.mEINames = paPeer->getAdapterInterfaceSpec()->mEONames;
  mCurrentFBInterfaceSpec.mEIWith = paPeer->getAdapterInterfaceSpec()->mEOWith;
  mCurrentFBInterfaceSpec.mEIWithIndexes = paPeer->getAdapterInterfaceSpec()->mEOWithIndexes;
  mCurrentFBInterfaceSpec.mNumEOs = paPeer->getAdapterInterfaceSpec()->mNumEIs;
  mCurrentFBInterfaceSpec.mEONames = paPeer->getAdapterInterfaceSpec()->mEINames;
  mCurrentFBInterfaceSpec.mEOWith = paPeer->getAdapterInterfaceSpec()->mEIWith;
  mCurrentFBInterfaceSpec.mEOWithIndexes = paPeer->getAdapterInterfaceSpec()->mEIWithIndexes;
  mCurrentFBInterfaceSpec.mNumDIs = paPeer->getAdapterInterfaceSpec()->mNumDOs;
  mCurrentFBInterfaceSpec.mDINames = paPeer->getAdapterInterfaceSpec()->mDONames;
  mCurrentFBInterfaceSpec.mDIDataTypeNames = paPeer->getAdapterInterfaceSpec()->mDODataTypeNames;
  mCurrentFBInterfaceSpec.mNumDOs = paPeer->getAdapterInterfaceSpec()->mNumDIs;
  mCurrentFBInterfaceSpec.mDONames = paPeer->getAdapterInterfaceSpec()->mDINames;
  mCurrentFBInterfaceSpec.mDODataTypeNames = paPeer->getAdapterInterfaceSpec()->mDIDataTypeNames;
  mCurrentFBInterfaceSpec.mNumDIOs = paPeer->getAdapterInterfaceSpec()->mNumDIOs;
  mCurrentFBInterfaceSpec.mDIONames = paPeer->getAdapterInterfaceSpec()->mDIONames;

  setupFBInterface(&mCurrentFBInterfaceSpec);
  fillEventEntryList(m_ParentFB);
}

bool CAnyAdapter::disconnect(CAdapterConnection *paAdConn){
  bool bRetVal = CAdapter::disconnect(paAdConn);

  //clean interface data and reset to empty interface
  freeAllData();
  setupFBInterface(&scmFBInterfaceSpec);

  return bRetVal;
}

// Saves parentFB for later use
void CAnyAdapter::setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID){
  if(nullptr == m_ParentFB){
    m_ParentFB = paParentFB;
  }
  if(0 == mParentAdapterlistID){
    mParentAdapterlistID = paParentAdapterlistID;
  }

  CAdapter::setParentFB(m_ParentFB, mParentAdapterlistID);
}
