/*******************************************************************************
 * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add string functions accepting a size parameter
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
  mStringBufAddr = scmConstStringBuf;
  mStringIdBufAddr = scmIdList;

  mStringBufSize = cgStringDictInitialStringBufSize;
  mMaxNrOfStrings = cgStringDictInitialMaxNrOfStrings;

  mNrOfStrings = cgNumOfConstStrings;
  mNextString = g_nStringIdNextFreeId;
#else
  mStringIdBufAddr = nullptr;
  mStringBufSize = 0;
  mMaxNrOfStrings = 0;
  mNrOfStrings = 0;
  mNextString = CStringDictionary::scmInvalidStringId;
  size_t nStringBufSize = cgStringDictInitialStringBufSize;
  if(nStringBufSize < g_nStringIdNextFreeId){
    nStringBufSize = (g_nStringIdNextFreeId * 3) >> 1;
  }

  unsigned int nMaxNrOfStrings = cgStringDictInitialMaxNrOfStrings;
  if(nMaxNrOfStrings < cgNumOfConstStrings){
    nMaxNrOfStrings = (cgNumOfConstStrings * 3) >> 1;
  }

  mStringBufAddr = (char *) forte_malloc(nStringBufSize * sizeof(char));
  if(nullptr != mStringBufAddr){
    mStringIdBufAddr = (TStringId *) forte_malloc(nMaxNrOfStrings * sizeof(TStringId));
    if(nullptr != mStringIdBufAddr){
      memcpy(mStringBufAddr, scmConstStringBuf, g_nStringIdNextFreeId);
      memcpy(mStringIdBufAddr, scmIdList, (cgNumOfConstStrings * sizeof(TStringId)));

      mStringBufSize = nStringBufSize;
      mMaxNrOfStrings = nMaxNrOfStrings;

      mNrOfStrings = cgNumOfConstStrings;
      mNextString = g_nStringIdNextFreeId;
    }
    else{
      forte_free(mStringBufAddr);
      mStringBufAddr = nullptr;
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
  forte_free(mStringBufAddr);
  forte_free(mStringIdBufAddr);
#endif
  mStringIdBufAddr = nullptr;
  mStringBufAddr = nullptr;
  mStringBufSize = 0;
  mMaxNrOfStrings = 0;
  mNrOfStrings = 0;
  mNextString = 0;
}

// get a string (0 if not found)
const char *CStringDictionary::get(TStringId paId){
  if(paId >= mNextString) {
    return nullptr;
  }

  const char *adr = getStringAddress(paId);
  if(paId > 0 && adr[-1] != '\0') {
    return nullptr;
  }

  return adr;
}

CStringDictionary::TStringId CStringDictionary::insert(const char *paStr) {
  return insert(paStr, strlen(paStr));
}

// insert a string and return a string id (InvalidTStringId for no memory or other error)
CStringDictionary::TStringId CStringDictionary::insert(const char *paStr, size_t paStrSize){
  TStringId nRetVal = scmInvalidStringId;

  if(nullptr != paStr){
    unsigned int idx;
    nRetVal = findEntry(paStr, paStrSize, idx);
    if(scmInvalidStringId == nRetVal){
      size_t nRequiredSize = mNextString + paStrSize + 1;

      if(mNrOfStrings >= mMaxNrOfStrings){
#ifdef FORTE_STRING_DICT_FIXED_MEMORY
        return scmInvalidStringId;
#else
        //grow exponentially by 1.5 according to Herb Sutter best strategy
        if(!reallocateStringIdBuf((mMaxNrOfStrings * 3) >> 1)){
          return scmInvalidStringId;
        }
#endif

      }
      if(nRequiredSize > mStringBufSize){
#ifdef FORTE_STRING_DICT_FIXED_MEMORY
        return scmInvalidStringId;
#else
        //grow exponentially by 1.5 according to Herb Sutter best strategy
        if(!reallocateStringBuf((nRequiredSize * 3) >> 1)){
          return scmInvalidStringId;
        }
#endif
      }
      nRetVal = insertAt(paStr, idx, paStrSize);
    }
  }
  return nRetVal;
}

CStringDictionary::TStringId CStringDictionary::findEntry(const char *paStr, unsigned int &paIdx) const{
  paIdx = 0;
  if(mNrOfStrings == 0) {
    return scmInvalidStringId;
  }

  int r = 0;

  unsigned int low = 0, high = mNrOfStrings;

  while(low < high){
    paIdx = (low + high) / 2;

    r = strcmp(paStr, getStringAddress(mStringIdBufAddr[paIdx]));

    if(!r){
      return mStringIdBufAddr[paIdx];
    }

    if(r > 0) {
      low = paIdx + 1;
    } else {
      high = paIdx;
    }
  }

  if(r > 0) {
    paIdx++;
  }

  return scmInvalidStringId;
}

// Find an exact match or place to be the new index
CStringDictionary::TStringId CStringDictionary::findEntry(const char *paStr, size_t paStrSize, unsigned int &paIdx) const{
  paIdx = 0;
  if(mNrOfStrings == 0) {
    return scmInvalidStringId;
  }

  int r = 0;

  unsigned int low = 0, high = mNrOfStrings;

  while(low < high){
    paIdx = (low + high) / 2;

    r = strncmp(paStr, getStringAddress(mStringIdBufAddr[paIdx]), paStrSize);
    if(!r) {
      r = -static_cast<int>(getStringAddress(mStringIdBufAddr[paIdx])[paStrSize]);
    }

    if(!r){
      return mStringIdBufAddr[paIdx];
    }

    if(r > 0) {
      low = paIdx + 1;
    } else {
      high = paIdx;
    }
  }

  if(r > 0) {
    paIdx++;
  }

  return scmInvalidStringId;
}

// Reallocate the Id buffer
bool CStringDictionary::reallocateStringIdBuf(unsigned int paNewMaxNrOfStrings){
  bool bRetval = true;
  if(paNewMaxNrOfStrings > mMaxNrOfStrings){
    TStringId *adr = (TStringId *) forte_malloc(paNewMaxNrOfStrings * sizeof(TStringId));
    if(nullptr != adr){
      memcpy(adr, mStringIdBufAddr, mMaxNrOfStrings * sizeof(TStringId));
      TStringId *oldData = mStringIdBufAddr;
      mStringIdBufAddr = adr;
      mMaxNrOfStrings = paNewMaxNrOfStrings;
      forte_free(oldData);
    }
    else{
      bRetval = false;
    }
  }
  return bRetval;
}

// Reallocate the string buffer
bool CStringDictionary::reallocateStringBuf(size_t paNewBufSize){
  bool bRetval = true;
  if(paNewBufSize > mStringBufSize){
    char *adr = (char *) forte_malloc(paNewBufSize * sizeof(char));
    if(nullptr != adr){
      memcpy(adr, mStringBufAddr, mStringBufSize * sizeof(char));
      char *oldData = mStringBufAddr;
      mStringBufAddr = adr;
      mStringBufSize = paNewBufSize;
      forte_free(oldData);
    }
    else{
      bRetval = false;
    }
  }
  return bRetval;
}

// Insert the string at the specified position
CStringDictionary::TStringId CStringDictionary::insertAt(const char *paStr, unsigned int paIdx, size_t paLen){
  TStringId id = mNextString;
  char *p = getStringAddress(mNextString);

  memcpy(p, paStr, paLen);
  p[paLen] = '\0';
  mNextString += paLen + 1;

  if(paIdx < mNrOfStrings) {
    memmove(mStringIdBufAddr + paIdx + 1, mStringIdBufAddr + paIdx, (mNrOfStrings - paIdx) * sizeof(TStringId));
  }

  mStringIdBufAddr[paIdx] = id;
  mNrOfStrings++;

  return id;
}
