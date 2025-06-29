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
#include <cstring>
#include "generated/stringdict_generated.h"
#include "generated/config/FORTE_STRINGDICT_FIXEDMEMORY.h"

using TStringId = CStringDictionary::TStringId;

#ifdef FORTE_STRINGDICT_FIXEDMEMORY
#include "generated/config/FORTE_STRINGDICT_MAXNROFSTRINGS.h"
#include "generated/config/FORTE_STRINGDICT_BUFFERSIZE.h"
#include "core/util/inplace_vector.h"

#define ENTRY(name, len) , len
static forte::core::util::inplace_vector<TStringId, FORTE_STRINGDICT_MAXNROFSTRINGS> sIdList = { 0 ENTRY_LIST };
#undef ENTRY

#define ENTRY(name, len) #name "\0"
static constexpr const auto initbuf{ "\0" ENTRY_LIST};
static_assert(sizeof( "\0" ENTRY_LIST) < FORTE_STRINGDICT_BUFFERSIZE);
static forte::core::util::inplace_vector<char, FORTE_STRINGDICT_BUFFERSIZE> sStringBuf{initbuf, initbuf+sizeof( "\0" ENTRY_LIST)};
#undef ENTRY

#else
#include <vector>

#define ENTRY(name, len) , len
static std::vector<TStringId> sIdList = { 0 ENTRY_LIST };
#undef ENTRY

#define ENTRY(name, len) #name "\0"
static constexpr const auto initbuf{ "\0" ENTRY_LIST};
static std::vector<char> sStringBuf{initbuf, initbuf+sizeof( "\0" ENTRY_LIST)};
#undef ENTRY

#endif

// Find an exact match or place to be the new index
static TStringId findEntry(const char *paStr, size_t paStrSize, unsigned int &paIdx) {
  paIdx = 0;
  if (sIdList.empty()) {
    return CStringDictionary::scmInvalidStringId;
  }

  int r = 0;

  unsigned int low = 0, high = unsigned(sIdList.size());

  while (low < high) {
    paIdx = (low + high) / 2;

    r = strncmp(paStr, &sStringBuf[sIdList[paIdx]], paStrSize);
    if (!r) {
      r = -!!sStringBuf[sIdList[paIdx] + paStrSize];
    }

    if (!r) {
      return sIdList[paIdx];
    }

    if (r > 0) {
      low = paIdx + 1;
    } else {
      high = paIdx;
    }
  }

  if (r > 0) {
    paIdx++;
  }

  return CStringDictionary::scmInvalidStringId;
}

TStringId CStringDictionary::getId(const char *paStr) {
  return getId(paStr, strlen(paStr));
}

TStringId CStringDictionary::getId(const char *paStr, size_t paStrSize) {
  unsigned int nIdx;
  return findEntry(paStr, paStrSize, nIdx);
}

const char *CStringDictionary::get(TStringId paId) {
  if (paId >= sStringBuf.size() || paId < 1) {
    return nullptr;
  }

  if (sStringBuf[paId - 1] != '\0') {
    return nullptr;
  }

  return &sStringBuf[paId];
}

TStringId CStringDictionary::insert(const char *paStr) {
  return insert(paStr, strlen(paStr));
}

// insert a string and return a string id (InvalidTStringId for no memory or other error)
TStringId CStringDictionary::insert(const char *paStr, size_t paStrSize) {
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
  if (sIdList.capacity() < sIdList.size() + 1 || sStringBuf.capacity() < sStringBuf.size() + paStrSize + 1) {
    return scmInvalidStringId;
  }
#endif

  nRetVal = sStringBuf.size();
  sStringBuf.resize(nRetVal + paStrSize + 1, 0);
  memcpy(&sStringBuf[nRetVal], paStr, paStrSize);

  sIdList.insert(sIdList.begin() + idx, nRetVal);

  return nRetVal;
}
