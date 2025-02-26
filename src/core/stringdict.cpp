/*******************************************************************************
 * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 *               2025 Jörg Walter
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
 *    Jörg Walter
 *      - rework to use std::vector/inplace_vecctor
 *******************************************************************************/
#include "stringdict.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "stringdict_gen.cpp"
#endif
#include <cstring>
#include <stringlist.cpp>

DEFINE_SINGLETON(CStringDictionary)

#ifdef FORTE_STRING_DICT_FIXED_MEMORY
#include "core/util/inplace_vector.h"
static forte::core::util::inplace_vector<CStringDictionary::TStringId, FORTE_STRINGDICT_MAXNROFSTRINGS> scmIdList = INDEX_LIST;
static forte::core::util::inplace_vector<char, FORTE_STRINGDICT_BUFFERSIZE> scmStringBuf = STRING_BUF;

#else
#include <vector>
static std::vector<CStringDictionary::TStringId> scmIdList = INDEX_LIST;
static std::vector<char> scmStringBuf = STRING_BUF;

#endif

CStringDictionary::CStringDictionary()
{
}

CStringDictionary::~CStringDictionary()
{
}

CStringDictionary::TStringId CStringDictionary::getId(const char *paStr) const
{
  return getId(paStr, strlen(paStr));
}

CStringDictionary::TStringId CStringDictionary::getId(const char *paStr, size_t paStrSize) const
{
  unsigned int nIdx;
  return findEntry(paStr, paStrSize, nIdx);
}

// get a string (0 if not found)
const char *CStringDictionary::get(TStringId paId)
{
  if (paId >= scmStringBuf.size() || paId < 1) {
    return nullptr;
  }

  if (scmStringBuf[paId-1] != '\0') {
    return nullptr;
  }

  return &scmStringBuf[paId];
}

CStringDictionary::TStringId CStringDictionary::insert(const char *paStr)
{
  return insert(paStr, strlen(paStr));
}

// insert a string and return a string id (InvalidTStringId for no memory or other error)
CStringDictionary::TStringId CStringDictionary::insert(const char *paStr, size_t paStrSize)
{
  TStringId nRetVal = scmInvalidStringId;

  if (!paStr) {
    return scmInvalidStringId;
  }

  unsigned int idx = 0;
  nRetVal = findEntry(paStr, paStrSize, idx);

  if (nRetVal != scmInvalidStringId) {
    return nRetVal;
  }

#ifdef FORTE_STRING_DICT_FIXED_MEMORY
  if (scmIdList.capacity() < scmIdList.size()+1 || scmStringBuf.capacity() < scmStringBuf.size()+paStrSize+1) {
    return scmInvalidStringId;
  }
#endif

  nRetVal = scmStringBuf.size();
  scmStringBuf.resize(nRetVal+paStrSize+1, 0);
  memcpy(&scmStringBuf[nRetVal], paStr, paStrSize);

  scmIdList.insert(scmIdList.begin()+idx, nRetVal);

  return nRetVal;
}

// Find an exact match or place to be the new index
CStringDictionary::TStringId CStringDictionary::findEntry(const char *paStr, size_t paStrSize, unsigned int &paIdx) const
{
  paIdx = 0;
  if (scmIdList.empty()) {
    return scmInvalidStringId;
  }

  int r = 0;

  unsigned int low = 0, high = unsigned(scmIdList.size());

  while(low < high){
    paIdx = (low + high) / 2;

    r = strncmp(paStr, &scmStringBuf[scmIdList[paIdx]], paStrSize);
    if(!r) {
      r = -!!scmStringBuf[scmIdList[paIdx]+paStrSize];
    }

    if(!r){
      return scmIdList[paIdx];
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
