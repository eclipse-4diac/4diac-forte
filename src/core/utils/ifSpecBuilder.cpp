/* Copyright (c) 2022 Davor Cihlar
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar
 *     - initial implementation
 **********************************************************************/

#include <algorithm>
#include <cstdio>

#include "forte_printer.h"
#include "ifSpecBuilder.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ifSpecBuilder_gen.cpp"
#endif
#include "mixedStorage.h"

using namespace forte::core::util;


bool CStringIdListSpecBuilder::setStaticList(const CStringDictionary::TStringId *pa_aunStaticList, std::size_t pa_nItemsCount) {
  if (pa_nItemsCount > m_unMaxItems)
    return false;
  m_vDynamicList.clear();
  m_aunStaticList = pa_aunStaticList;
  m_unStaticListSize = (TForteUInt8)pa_nItemsCount;
  return true;
}

bool CStringIdListSpecBuilder::addString(CStringDictionary::TStringId pa_unString) {
  if (m_aunStaticList) {
    m_vDynamicList.insert(m_vDynamicList.end(), m_aunStaticList, m_aunStaticList + m_unStaticListSize);
    m_aunStaticList = nullptr;
  }
  m_unStaticListSize = 0;
  if (m_vDynamicList.size() >= m_unMaxItems)
    return false;
  m_vDynamicList.push_back(pa_unString);
  return true;
}

bool CStringIdListSpecBuilder::addString(const char *pa_sString) {
  return addString(CStringDictionary::getInstance().insert(pa_sString));
}

int CStringIdListSpecBuilder::findString(const char *pa_sString) const {
  auto nString = CStringDictionary::getInstance().getId(pa_sString);
  if (nString == CStringDictionary::scm_nInvalidStringId)
    return -1;
  return findString(nString);
}

int CStringIdListSpecBuilder::findString(CStringDictionary::TStringId pa_unString) const {
  int retVal = -1;
  if (m_aunStaticList) {
    const auto end = m_aunStaticList + m_unStaticListSize;
    auto pos = std::find(m_aunStaticList, end, pa_unString);
    if (pos != end) {
      retVal = (int)(pos - m_aunStaticList);
    }
  } else {
    auto pos = std::find(m_vDynamicList.begin(), m_vDynamicList.end(), pa_unString);
    if (pos != m_vDynamicList.end()) {
      retVal = (int)(pos - m_vDynamicList.begin());
    }
  }
  return retVal;
}

std::size_t CStringIdListSpecBuilder::calcStorageSize() const {
  return m_aunStaticList ? 0 : m_vDynamicList.size() * sizeof(CStringDictionary::TStringId);
}

std::tuple<const CStringDictionary::TStringId*, TForteUInt8> CStringIdListSpecBuilder::build(CMixedStorage &pa_oStorage) const {
  if (m_aunStaticList) {
    return {m_aunStaticList, m_unStaticListSize};
  }
  const CStringDictionary::TStringId* listPtr = pa_oStorage.write(m_vDynamicList.data(), m_vDynamicList.size());
  return {listPtr, (TForteUInt8)m_vDynamicList.size()};
}


static bool makeName(char *pa_sName, const char *pa_sPrefix, int pa_nNumber) {
  return forte_snprintf(pa_sName, sizeof(TIdentifier), "%s%d", pa_sPrefix, pa_nNumber) < (int)sizeof(TIdentifier);
}

bool CEventSpecBuilderBase::addEventRange(const char *pa_sPrefix, int pa_nRangeSize) {
  TIdentifier name;
  bool retVal = true;
  for (int i = 0; i < pa_nRangeSize; ++i) {
    if (!makeName(name, pa_sPrefix, i + 1))
      return false;
    retVal = retVal && addEvent(name);
  }
  return retVal;
}

bool CDataSpecBuilderBase::addDataRange(const char *pa_sPrefix, int pa_nRangeSize) {
  TIdentifier name;
  bool retVal = true;
  for (int i = 0; i < pa_nRangeSize; ++i) {
    if (!makeName(name, pa_sPrefix, i + 1))
      return false;
    retVal = retVal && addData(name, g_nStringIdANY);
  }
  return retVal;
}

std::tuple<const CStringDictionary::TStringId*, const CStringDictionary::TStringId*, TForteUInt8>
CDataSpecBuilderBase::build(CMixedStorage &pa_oStorage) const {
  auto nameData = m_oNamesListBuilder.build(pa_oStorage);
  auto typeData = m_oTypesListBuilder.build(pa_oStorage);
  return {std::get<0>(nameData), std::get<0>(typeData), std::get<1>(nameData) };
}


void CWithSpecBuilderBase::grow(std::size_t pa_unNumEvents) {
  if (pa_unNumEvents > m_vvDynamicList.size()) {
    m_vvDynamicList.resize(pa_unNumEvents);
  }
}

bool CWithSpecBuilderBase::setStaticBindings(const TDataIOID *pa_aunStaticBindings, const TForteInt16 *pa_anStaticIndexes, std::size_t pa_unNumEvents) {
  if (!m_vvDynamicList.empty())
    return false;
  m_aunStaticBindings = pa_aunStaticBindings;
  m_anStaticIndexes = pa_anStaticIndexes;
  m_unNumStaticEvents = pa_unNumEvents;
  return true;
}

bool CWithSpecBuilderBase::bind(TDataIOID pa_unEventId, TDataIOID pa_unDataId) {
  if (m_aunStaticBindings)
    return false;
  grow(pa_unEventId + 1);
  m_vvDynamicList[pa_unEventId].push_back(pa_unDataId);
  return true;
}

bool CWithSpecBuilderBase::bindRange(TDataIOID pa_unEventId, TDataIOID pa_unFirstDataId, TDataIOID pa_unLastDataId) {
  bool retVal = true;
  for (TDataIOID runnerPort = pa_unFirstDataId; runnerPort <= pa_unLastDataId; ++runnerPort) {
    retVal = retVal && bind(pa_unEventId, runnerPort);
  }
  return retVal;
}

std::size_t CWithSpecBuilderBase::calcStorageSize(std::size_t pa_unNumEvents) const {
  if (m_aunStaticBindings) {
    return 0;
  }
  std::size_t sumSize = m_vvDynamicList.size();
  for (const auto &lst : m_vvDynamicList) {
    if (!lst.empty()) {
      sumSize += (lst.size() + 1) * sizeof(TDataIOID);
    }
    sumSize += sizeof(TForteInt16);
  }
  sumSize += sizeof(TForteInt16) * (pa_unNumEvents - m_vvDynamicList.size());
  return sumSize;
}

std::tuple<const TDataIOID*, const TForteInt16*> CWithSpecBuilderBase::build(CMixedStorage &pa_oStorage, std::size_t pa_unNumEvents) const {
  if (m_aunStaticBindings) {
    //TODO error when number of events differ
    return {m_aunStaticBindings, m_anStaticIndexes};
  }

  // build with reference list
  auto pWiths = pa_oStorage.end<TDataIOID>();
  for (const auto &lst : m_vvDynamicList) {
    if (!lst.empty()) {
      pa_oStorage.write(lst.data(), lst.size());
      pa_oStorage.write<TDataIOID>(CFunctionBlock::scmWithListDelimiter);
    }
  }

  // build indexes list
  auto pIndexes = pa_oStorage.end<TForteInt16>();
  TForteInt16 idx = 0;
  for (const auto &lst : m_vvDynamicList) {
    if (lst.empty()) {
      pa_oStorage.write<TForteInt16>(-1);
    } else {
      pa_oStorage.write(idx);
      idx += (TForteInt16)(lst.size() + 1);
    }
  }
  // handle unspecified events
  for (std::size_t ei = m_vvDynamicList.size(); ei < pa_unNumEvents; ++ei) {
    pa_oStorage.write<TForteInt16>(-1);
  }
  //TODO error when internal event count is greater than pa_unNumEvents

  return {pWiths, pIndexes};
}


bool CIfSpecBuilder::build(CMixedStorage &pa_oStorage, SFBInterfaceSpec &pa_oInterfaceSpec) const {
  std::size_t storageSize =
    m_oEI.calcStorageSize() + m_oEO.calcStorageSize() +
    m_oDI.calcStorageSize() + m_oDO.calcStorageSize() +
    m_oIWith.calcStorageSize(m_oEI.getNumEvents()) + m_oOWith.calcStorageSize(m_oEO.getNumEvents());
  pa_oStorage.reserve(storageSize);
  std::tie(pa_oInterfaceSpec.m_aunEINames, pa_oInterfaceSpec.m_nNumEIs) = m_oEI.build(pa_oStorage);
  std::tie(pa_oInterfaceSpec.m_aunEONames, pa_oInterfaceSpec.m_nNumEOs) = m_oEO.build(pa_oStorage);
  std::tie(
    pa_oInterfaceSpec.m_aunDINames, pa_oInterfaceSpec.m_aunDIDataTypeNames,
    pa_oInterfaceSpec.m_nNumDIs
  ) = m_oDI.build(pa_oStorage);
  std::tie(
    pa_oInterfaceSpec.m_aunDONames, pa_oInterfaceSpec.m_aunDODataTypeNames,
    pa_oInterfaceSpec.m_nNumDOs
  ) = m_oDO.build(pa_oStorage);
  std::tie(pa_oInterfaceSpec.m_anEIWith, pa_oInterfaceSpec.m_anEIWithIndexes) = m_oIWith.build(pa_oStorage, m_oEI.getNumEvents());
  std::tie(pa_oInterfaceSpec.m_anEOWith, pa_oInterfaceSpec.m_anEOWithIndexes) = m_oOWith.build(pa_oStorage, m_oEO.getNumEvents());

  // Adapters are not yet supported
  pa_oInterfaceSpec.m_nNumAdapters = 0;
  pa_oInterfaceSpec.m_pstAdapterInstanceDefinition = nullptr;

  return pa_oStorage.size() <= storageSize;
}
