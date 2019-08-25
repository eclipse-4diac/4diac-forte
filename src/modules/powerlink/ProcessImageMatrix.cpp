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

  m_nBitSize = 0;
  m_nNumberOfEntries = 0;

}

CProcessImageMatrix::~CProcessImageMatrix(){
  clearAll();
}

void CProcessImageMatrix::addEntry(unsigned int pa_nCN, unsigned int pa_nModule, unsigned int pa_nIoId, unsigned int pa_nDataSize, unsigned int pa_nPiOffset, unsigned int pa_nBitOffset){

  m_lMatrix.pushBack(new SChannelEntry(pa_nCN, pa_nModule, pa_nIoId, pa_nDataSize, pa_nPiOffset, pa_nBitOffset));

  // Check if 8, 16, 32 bit aligned
  if((pa_nDataSize % 32 == 0) || (pa_nDataSize % 16 == 0) || (pa_nDataSize % 8 == 0)){
    if(m_nBitSize % pa_nDataSize != 0){
      unsigned long fillBits = pa_nDataSize - (m_nBitSize % pa_nDataSize);
      m_nBitSize += fillBits;
    }
  }
  m_nBitSize += pa_nDataSize;

  m_nNumberOfEntries++;
}

// getEntry: Returns an array with [dataSize, PIOffset, BitOffset]
unsigned int* CProcessImageMatrix::getEntry(unsigned int pa_nCN, unsigned int pa_nModule, unsigned int pa_nIoId){
  static unsigned int data[3] = { 0, 0, 0 };

  TChannelList::Iterator itEnd = m_lMatrix.end();
  for(TChannelList::Iterator it = m_lMatrix.begin(); it != itEnd; ++it){
    if(it->m_nCN == pa_nCN && it->m_nModuleId == pa_nModule && it->m_nIOid == pa_nIoId){
      data[0] = it->m_nDataSize;
      data[1] = it->m_nPIOffset;
      data[2] = it->m_nBitOffset;
      return data;
    }
  }

  return NULL;
}

// getEntry: same as above but returns the values on row "index"
unsigned int* CProcessImageMatrix::getEntry(unsigned int pa_nIndex){
  if(pa_nIndex < m_nNumberOfEntries){
    static unsigned int data[3] = { 0, 0, 0 };
    TChannelList::Iterator it = m_lMatrix.begin();
    for(unsigned int i = 0; i < pa_nIndex; i++){
      ++it;
    }
    data[0] = it->m_nDataSize;
    data[1] = it->m_nPIOffset;
    data[2] = it->m_nBitOffset;
    return data;
  }

  return NULL;
}

unsigned int CProcessImageMatrix::getNrOfEntries() const {
  return m_nNumberOfEntries;
}

CProcessImageMatrix* CProcessImageMatrix::getModuleEntries(unsigned int pa_nCN, unsigned int pa_nModule){
  CProcessImageMatrix* newMatrix = NULL;

  TChannelList::Iterator itEnd = m_lMatrix.end();
  for(TChannelList::Iterator it(m_lMatrix.begin()); it != itEnd; ++it){
    if(it->m_nCN == pa_nCN && it->m_nModuleId == pa_nModule){
      if(newMatrix == NULL) {
        newMatrix = new CProcessImageMatrix();
      }

      newMatrix->addEntry(it->m_nCN, it->m_nModuleId, it->m_nIOid, it->m_nDataSize, it->m_nPIOffset, it->m_nBitOffset);
    }
  }

  return newMatrix;
}

unsigned long CProcessImageMatrix::getProcessImageSize() const {
  // Check if the whole matrix is 32 bit aligned
  if(m_nBitSize % 32 != 0){
    int fillBits = 32 - (m_nBitSize % 32);
    return (unsigned long) (m_nBitSize + fillBits) / 8;
  }
  return (unsigned long) m_nBitSize / 8;
}

void CProcessImageMatrix::clearAll(){
  while(!m_lMatrix.isEmpty()){
    delete *(TChannelList::Iterator) m_lMatrix.begin();
    m_lMatrix.popFront();
  }
}
