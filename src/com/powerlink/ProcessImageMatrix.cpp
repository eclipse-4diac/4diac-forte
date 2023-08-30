/*******************************************************************************
 * Copyright (c) 2012 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "ProcessImageMatrix.h"

CProcessImageMatrix::CProcessImageMatrix(){

  mBitSize = 0;
  mNumberOfEntries = 0;

}

CProcessImageMatrix::~CProcessImageMatrix(){
  clearAll();
}

void CProcessImageMatrix::addEntry(unsigned int paCN, unsigned int paModule, unsigned int paIoId, unsigned int paDataSize, unsigned int paPiOffset, unsigned int paBitOffset){

  mMatrix.pushBack(new SChannelEntry(paCN, paModule, paIoId, paDataSize, paPiOffset, paBitOffset));

  // Check if 8, 16, 32 bit aligned
  if((paDataSize % 32 == 0) || (paDataSize % 16 == 0) || (paDataSize % 8 == 0)){
    if(mBitSize % paDataSize != 0){
      unsigned long fillBits = paDataSize - (mBitSize % paDataSize);
      mBitSize += fillBits;
    }
  }
  mBitSize += paDataSize;

  mNumberOfEntries++;
}

// getEntry: Returns an array with [dataSize, PIOffset, BitOffset]
unsigned int* CProcessImageMatrix::getEntry(unsigned int paCN, unsigned int paModule, unsigned int paIoId){
  static unsigned int data[3] = { 0, 0, 0 };

  TChannelList::Iterator itEnd = mMatrix.end();
  for(TChannelList::Iterator it = mMatrix.begin(); it != itEnd; ++it){
    if(it->mCN == paCN && it->mModuleId == paModule && it->mIOid == paIoId){
      data[0] = it->mDataSize;
      data[1] = it->mPIOffset;
      data[2] = it->mBitOffset;
      return data;
    }
  }

  return nullptr;
}

// getEntry: same as above but returns the values on row "index"
unsigned int* CProcessImageMatrix::getEntry(unsigned int paIndex){
  if(paIndex < mNumberOfEntries){
    static unsigned int data[3] = { 0, 0, 0 };
    TChannelList::Iterator it = mMatrix.begin();
    for(unsigned int i = 0; i < paIndex; i++){
      ++it;
    }
    data[0] = it->mDataSize;
    data[1] = it->mPIOffset;
    data[2] = it->mBitOffset;
    return data;
  }

  return nullptr;
}

unsigned int CProcessImageMatrix::getNrOfEntries() const {
  return mNumberOfEntries;
}

CProcessImageMatrix* CProcessImageMatrix::getModuleEntries(unsigned int paCN, unsigned int paModule){
  CProcessImageMatrix* newMatrix = nullptr;

  TChannelList::Iterator itEnd = mMatrix.end();
  for(TChannelList::Iterator it(mMatrix.begin()); it != itEnd; ++it){
    if(it->mCN == paCN && it->mModuleId == paModule){
      if(newMatrix == nullptr) {
        newMatrix = new CProcessImageMatrix();
      }

      newMatrix->addEntry(it->mCN, it->mModuleId, it->mIOid, it->mDataSize, it->mPIOffset, it->mBitOffset);
    }
  }

  return newMatrix;
}

unsigned long CProcessImageMatrix::getProcessImageSize() const {
  // Check if the whole matrix is 32 bit aligned
  if(mBitSize % 32 != 0){
    int fillBits = 32 - (mBitSize % 32);
    return (unsigned long) (mBitSize + fillBits) / 8;
  }
  return (unsigned long) mBitSize / 8;
}

void CProcessImageMatrix::clearAll(){
  while(!mMatrix.isEmpty()){
    delete *(TChannelList::Iterator) mMatrix.begin();
    mMatrix.popFront();
  }
}
