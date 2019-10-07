/*******************************************************************************
 * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "stringdict.h"
#include <fortenew.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "stringdict_gen.cpp"
#endif
#include <stringlist.h>
#include <string.h>
#include <stdlib.h>
#include "devlog.h"

DEFINE_SINGLETON(CStringDictionary)

CStringDictionary::CStringDictionary(){
#ifdef FORTE_STRING_DICT_FIXED_MEMORY
  m_paStringBufAddr = scm_acConstStringBuf;
  m_pnStringIdBufAddr = scm_aunIdList;

  m_nStringBufSize = cg_unStringDictInitialStringBufSize;
  m_nMaxNrOfStrings = cg_unStringDictInitialMaxNrOfStrings;

  m_nNrOfStrings = cg_nNumOfConstStrings;
  m_nNextString = g_nStringIdNextFreeId;
#else
  m_pnStringIdBufAddr = 0;
  m_nStringBufSize = 0;
  m_nMaxNrOfStrings = 0;
  m_nNrOfStrings = 0;
  m_nNextString = CStringDictionary::scm_nInvalidStringId;
  unsigned int nStringBufSize = cg_unStringDictInitialStringBufSize;
  if(nStringBufSize < g_nStringIdNextFreeId){
    nStringBufSize = (g_nStringIdNextFreeId * 3) >> 1;
  }

  unsigned int nMaxNrOfStrings = cg_unStringDictInitialMaxNrOfStrings;
  if(nMaxNrOfStrings < cg_nNumOfConstStrings){
    nMaxNrOfStrings = (cg_nNumOfConstStrings * 3) >> 1;
  }

  m_paStringBufAddr = (char *) forte_malloc(nStringBufSize * sizeof(char));
  if(0 != m_paStringBufAddr){
    m_pnStringIdBufAddr = (TStringId *) forte_malloc(nMaxNrOfStrings * sizeof(TStringId));
    if(0 != m_pnStringIdBufAddr){
      memcpy(m_paStringBufAddr, scm_acConstStringBuf, g_nStringIdNextFreeId);
      memcpy(m_pnStringIdBufAddr, scm_aunIdList, (cg_nNumOfConstStrings * sizeof(TStringId)));

      m_nStringBufSize = nStringBufSize;
      m_nMaxNrOfStrings = nMaxNrOfStrings;

      m_nNrOfStrings = cg_nNumOfConstStrings;
      m_nNextString = g_nStringIdNextFreeId;
    }
    else{
      forte_free(m_paStringBufAddr);
      m_paStringBufAddr = 0;
    }
  }
#endif
}

CStringDictionary::~CStringDictionary(){
  clear();
}

// clear
void CStringDictionary::clear(){
#ifndef FORTE_STRING_DICT_FIXED_MEMORY
  forte_free(m_paStringBufAddr);
  forte_free(m_pnStringIdBufAddr);
#endif
  m_pnStringIdBufAddr = 0;
  m_paStringBufAddr = 0;
  m_nStringBufSize = 0;
  m_nMaxNrOfStrings = 0;
  m_nNrOfStrings = 0;
  m_nNextString = 0;
}

// get a string (0 if not found)
const char *CStringDictionary::get(TStringId pa_nId){
  if(pa_nId >= m_nNextString) {
    return 0;
  }

  const char *adr = getStringAddress(pa_nId);
  if(pa_nId > 0 && adr[-1] != '\0') {
    return 0;
  }

  return adr;
}

// insert a string and return a string id (InvalidTStringId for no memory or other error)
CStringDictionary::TStringId CStringDictionary::insert(const char *pa_sStr){
  TStringId nRetVal = scm_nInvalidStringId;

  if(0 != pa_sStr){
    if('\0' != *pa_sStr){
      unsigned int idx;
      nRetVal = findEntry(pa_sStr, idx);
      if(scm_nInvalidStringId == nRetVal){
        TStringId len = static_cast<TStringId>(strlen(pa_sStr));
        TStringId nRequiredSize = m_nNextString + len + 1;

        if(m_nNrOfStrings >= m_nMaxNrOfStrings){
#ifdef FORTE_STRING_DICT_FIXED_MEMORY
          return scm_nInvalidStringId;
#else
          //grow exponentially by 1.5 according to Herb Sutter best strategy
          if(!reallocateStringIdBuf((m_nMaxNrOfStrings * 3) >> 1)){
            return scm_nInvalidStringId;
          }
#endif

        }
        if(nRequiredSize > m_nStringBufSize){
#ifdef FORTE_STRING_DICT_FIXED_MEMORY
          return scm_nInvalidStringId;
#else
          //grow exponentially by 1.5 according to Herb Sutter best strategy
          if(!reallocateStringBuf((nRequiredSize * 3) >> 1)){
            return scm_nInvalidStringId;
          }
#endif
        }
        nRetVal = insertAt(pa_sStr, idx, len);
      }
    }
    else{
      DEVLOG_WARNING("[CStringDictionary::insert] String to insert was empty\n");
    }
  }
  return nRetVal;
}

// Find an exact match or place to be the new index
CStringDictionary::TStringId CStringDictionary::findEntry(const char *pa_sStr, unsigned int &pa_rnIdx) const{
  pa_rnIdx = 0;
  if(m_nNrOfStrings == 0) {
    return scm_nInvalidStringId;
  }

  int r = 0;

  unsigned int low = 0, high = m_nNrOfStrings;

  while(low < high){
    pa_rnIdx = (low + high) / 2;

    r = strcmp(pa_sStr, getStringAddress(m_pnStringIdBufAddr[pa_rnIdx]));

    if(!r){
      return m_pnStringIdBufAddr[pa_rnIdx];
    }

    if(r > 0) {
      low = pa_rnIdx + 1;
    } else {
      high = pa_rnIdx;
    }
  }

  if(r > 0) {
    pa_rnIdx++;
  }

  return scm_nInvalidStringId;
}

// Reallocate the Id buffer
bool CStringDictionary::reallocateStringIdBuf(unsigned int pa_nNewMaxNrOfStrings){
  bool bRetval = true;
  if(pa_nNewMaxNrOfStrings > m_nMaxNrOfStrings){
    TStringId *adr = (TStringId *) forte_malloc(pa_nNewMaxNrOfStrings * sizeof(TStringId));
    if(0 != adr){
      memcpy(adr, m_pnStringIdBufAddr, m_nMaxNrOfStrings * sizeof(TStringId));
      TStringId *oldData = m_pnStringIdBufAddr;
      m_pnStringIdBufAddr = adr;
      m_nMaxNrOfStrings = pa_nNewMaxNrOfStrings;
      forte_free(oldData);
    }
    else{
      bRetval = false;
    }
  }
  return bRetval;
}

// Reallocate the string buffer
bool CStringDictionary::reallocateStringBuf(TForteUInt32 pa_nNewBufSize){
  bool bRetval = true;
  if(pa_nNewBufSize > m_nStringBufSize){
    char *adr = (char *) forte_malloc(pa_nNewBufSize * sizeof(char));
    if(0 != adr){
      memcpy(adr, m_paStringBufAddr, m_nStringBufSize * sizeof(char));
      char *oldData = m_paStringBufAddr;
      m_paStringBufAddr = adr;
      m_nStringBufSize = pa_nNewBufSize;
      forte_free(oldData);
    }
    else{
      bRetval = false;
    }
  }
  return bRetval;
}

// Insert the string at the specified position
CStringDictionary::TStringId CStringDictionary::insertAt(const char *pa_sStr, unsigned int pa_nIdx, unsigned int pa_nLen){
  TStringId id = m_nNextString;
  char *p = getStringAddress(m_nNextString);

  memcpy(p, pa_sStr, pa_nLen);
  p[pa_nLen] = '\0';
  m_nNextString += pa_nLen + 1;

  if(pa_nIdx < m_nNrOfStrings) {
    memmove(m_pnStringIdBufAddr + pa_nIdx + 1, m_pnStringIdBufAddr + pa_nIdx, (m_nNrOfStrings - pa_nIdx) * sizeof(TStringId));
  }

  m_pnStringIdBufAddr[pa_nIdx] = id;
  m_nNrOfStrings++;

  return id;
}
