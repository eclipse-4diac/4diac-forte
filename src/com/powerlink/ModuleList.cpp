/*******************************************************************************
 * Copyright (c) 2012, 2014 AIT, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "ModuleList.h"
#include <cstring>

CModuleList::IoModule::IoModule(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset, unsigned int paModuleNr) :
    mPiOffset(paPiOffset), mBitOffset(paBitOffset), mModuleNr(paModuleNr){

  mName = new char[strlen(paName) + 1];
  strcpy(mName, paName);
}

CModuleList::IoModule::~IoModule(){
  delete[] mName;
}

CModuleList::CModuleList() :
    mNumberOfModules(0){

}

CModuleList::~CModuleList(){
  // Delete modules in list
  mModules.clearAll();

}

void CModuleList::addEntry(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset, unsigned int paModuleNr){
  mModules.pushBack(new IoModule(paName, paPiOffset, paBitOffset, paModuleNr));

  mNumberOfModules++;
}

int CModuleList::getModuleNr(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset){

  TModuleList::Iterator itEnd(mModules.end());
  for(TModuleList::Iterator it(mModules.begin()); it != itEnd; ++it){
    if(strcmp(it->mName, paName) == 0 && it->mPiOffset == paPiOffset && it->mBitOffset == paBitOffset){
      return it->mModuleNr;
    }
  }

  return -1;
}

int CModuleList::getModuleNr(const char* paName, unsigned int paOccurence){
  unsigned int nrOcc = 0;

  TModuleList::Iterator itEnd(mModules.end());
  for(TModuleList::Iterator it(mModules.begin()); it != itEnd; ++it){
    char* pch = strstr(it->mName, paName);
    if(pch != nullptr){
      nrOcc++;
      if(nrOcc == paOccurence){
        return it->mModuleNr;
      }
    }
  }

  return -1;
}

int CModuleList::getNrOfModules(const char* paName){
  int nrMods = 0;

  TModuleList::Iterator itEnd(mModules.end());
  for(TModuleList::Iterator it(mModules.begin()); it != itEnd; ++it){
    char* pch = strstr(it->mName, paName);
    if(pch != nullptr){
      nrMods++;
    }
  }

  return nrMods;
}

bool CModuleList::moduleNameExist(const char* paName){

  TModuleList::Iterator itEnd(mModules.end());
  for(TModuleList::Iterator it(mModules.begin()); it != itEnd; ++it){
    if(strcmp(it->mName, paName) == 0){
      return true;
    }
  }

  return false;
}
