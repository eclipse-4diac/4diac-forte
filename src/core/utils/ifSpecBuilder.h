/*********************************************************************
 * Copyright (c) 2022 Davor Cihlar
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

#ifndef _IF_SPEC_BUILDER_H_
#define _IF_SPEC_BUILDER_H_

#include <vector>
#include <array>
#include <tuple>
#include <funcbloc.h>


namespace forte::core::util {

class CMixedStorage;

class CInputSpecTag;
class COutputSpecTag;
class CDataSpecTag;
class CEventSpecTag;

template<class TypeTag, class DirTag>
class CSpecReference {
  public:
    constexpr CSpecReference() = default;
    constexpr CSpecReference(int pa_nRef) {
      if (pa_nRef < 0 || pa_nRef >= maxVal) {
        ref = maxVal;
      } else {
        ref = (TDataIOID)pa_nRef;
      }
    }

    constexpr bool isValid() const {
      return ref != maxVal;
    }

    constexpr auto operator*() const {
      return ref;
    }

  private:
    static constexpr auto maxVal = std::numeric_limits<TDataIOID>::max();

    TDataIOID ref = maxVal;
};

template<class TypeTag, class DirTag>
class CSpecReferenceRange {
  public:
    using TCSpecReference = CSpecReference<TypeTag, DirTag>;

    TCSpecReference m_oFirst;
    TCSpecReference m_oLast;

    constexpr CSpecReferenceRange() = default;
    constexpr CSpecReferenceRange(TCSpecReference pa_oFirst, TCSpecReference pa_oLast)
      : m_oFirst(pa_oFirst), m_oLast(pa_oLast)
    {}
    constexpr CSpecReferenceRange(std::pair<int, int> pa_oRangePair)
      : CSpecReferenceRange(std::get<0>(pa_oRangePair), std::get<1>(pa_oRangePair))
    {}

    constexpr TCSpecReference operator[](int offset) const {
      int id = *m_oFirst + offset;
      return { isValid() && id >= *m_oFirst && id <= *m_oLast ? id : -1 };
    }

    constexpr bool isValid() const {
      return m_oFirst.isValid() && m_oLast.isValid();
    }
};

class CStringIdListSpecBuilder {
  public:
    CStringIdListSpecBuilder(std::size_t pa_unMaxItems) : m_unMaxItems(pa_unMaxItems)
    {}

    void setStaticList(const CStringDictionary::TStringId *pa_aunStaticList, std::size_t pa_nItemsCount);
    int addString(const CStringDictionary::TStringId pa_unString);
    int addString(const char *pa_sString);

    std::size_t getNumStrings() const {
      return m_aunStaticList ? m_unStaticListSize : m_vDynamicList.size();
    }

    int findString(const char *pa_sString) const;
    int findString(CStringDictionary::TStringId pa_unString) const;

    std::size_t calcStorageSize() const;
    std::tuple<const CStringDictionary::TStringId*, TForteUInt8> build(CMixedStorage &pa_oStorage) const;

    bool isGood() const { return getNumStrings() <= m_unMaxItems; }

  private:
    const std::size_t m_unMaxItems;
    std::vector<CStringDictionary::TStringId> m_vDynamicList;
    std::size_t m_unStaticListSize = 0;
    const CStringDictionary::TStringId *m_aunStaticList = nullptr;
};

class CEventSpecBuilderBase {
  public:
    void setStaticEvents(const CStringDictionary::TStringId *pa_aunStaticNames, std::size_t pa_nEventsCount) {
      m_oNamesListBuilder.setStaticList(pa_aunStaticNames, pa_nEventsCount);
    }
    template<std::size_t N>
    void setStaticEvents(const std::array<CStringDictionary::TStringId, N> &pa_aStaticNames) {
      setStaticEvents(pa_aStaticNames.data(), pa_aStaticNames.size());
    }

    auto getNumEvents() const {
      return m_oNamesListBuilder.getNumStrings();
    }

    template<typename T>
    int findEvent(T pa_tName) const {
      return m_oNamesListBuilder.findString(pa_tName);
    }

    auto calcStorageSize() const {
      return m_oNamesListBuilder.calcStorageSize();
    }
    auto build(CMixedStorage &pa_oStorage) const {
      return m_oNamesListBuilder.build(pa_oStorage);
    }

    bool isGood() const {
      return m_oNamesListBuilder.isGood() && m_bIsGood;
    }

  private:
    static constexpr auto scm_unMaxEvents = CFunctionBlock::scm_nMaxInterfaceEvents;
    bool m_bIsGood = true;
    CStringIdListSpecBuilder m_oNamesListBuilder{scm_unMaxEvents};

  protected:
    template<typename NameType>
    int addEvent(NameType pa_xName) {
      return m_oNamesListBuilder.addString(pa_xName);
    }

    std::pair<int, int> addEventRange(const char *pa_sPrefix, int pa_nRangeSize);
};

template<class DirTag>
class CEventSpecBuilder : public CEventSpecBuilderBase {
  public:
    using TCSpecReference = CSpecReference<CEventSpecTag, DirTag>;
    using TCSpecReferenceRange = CSpecReferenceRange<CEventSpecTag, DirTag>;

    template<typename TNameType>
    TCSpecReference addEvent(TNameType pa_xName) {
      return CEventSpecBuilderBase::addEvent(pa_xName);
    }

    TCSpecReferenceRange addEventRange(const char *pa_sPrefix, int pa_nRangeSize) {
      return CEventSpecBuilderBase::addEventRange(pa_sPrefix, pa_nRangeSize);
    }

    template<typename TNameType>
    TCSpecReference operator[] (TNameType pa_tName) const {
      return { findEvent(pa_tName) };
    }
};

class CDataSpecBuilderBase {
  public:
    void setStaticData(const CStringDictionary::TStringId *pa_aunStaticDataNames, const CStringDictionary::TStringId *pa_aunStaticTypeNames, std::size_t pa_nDataCount) {
      m_oNamesListBuilder.setStaticList(pa_aunStaticDataNames, pa_nDataCount);
      m_oTypesListBuilder.setStaticList(pa_aunStaticTypeNames, pa_nDataCount);
    }
    template<std::size_t N>
    void setStaticData(const std::array<CStringDictionary::TStringId, N> &pa_aunStaticDataNames, const std::array<CStringDictionary::TStringId, N> &pa_aunStaticTypeNames) {
      setStaticData(pa_aunStaticDataNames.data(), pa_aunStaticTypeNames.data(), N);
    }

    template<typename T>
    int findData(T pa_tName) const {
      return m_oNamesListBuilder.findString(pa_tName);
    }

    std::size_t calcStorageSize() const {
      return m_oNamesListBuilder.calcStorageSize() + m_oTypesListBuilder.calcStorageSize();
    }
    std::tuple<const CStringDictionary::TStringId*, const CStringDictionary::TStringId*, TForteUInt8> build(CMixedStorage &pa_oStorage) const;

    bool isGood() const {
      return m_oNamesListBuilder.isGood() && m_oTypesListBuilder.isGood() && m_bIsGood;
    }

  private:
    static constexpr auto scm_unMaxData = CFunctionBlock::scm_nMaxInterfaceEvents;
    bool m_bIsGood = true;
    CStringIdListSpecBuilder m_oNamesListBuilder{scm_unMaxData};
    CStringIdListSpecBuilder m_oTypesListBuilder{scm_unMaxData};

  protected:
    template<typename TNameType1, typename TNameType2>
    int addData(TNameType1 pa_xName, TNameType2 pa_xTypeName) {
      m_oNamesListBuilder.addString(pa_xName);
      return m_oTypesListBuilder.addString(pa_xTypeName);
    }
    std::pair<int, int> addDataRange(const char *pa_sPrefix, int pa_nRangeSize);
    std::pair<int, int> addDataRange(const char *pa_sPrefix, int pa_nRangeSize, CStringDictionary::TStringId pa_unTypeName);
    std::pair<int, int> addDataRange(const char *pa_sPrefix, int pa_nRangeSize, const char *pa_sTypeName);
};

template<class DirTag>
class CDataSpecBuilder : public CDataSpecBuilderBase {
  public:
    using TCSpecReference = CSpecReference<CDataSpecTag, DirTag>;
    using TCSpecReferenceRange = CSpecReferenceRange<CDataSpecTag, DirTag>;

    template<typename TNameType1, typename TNameType2>
    TCSpecReference addData(TNameType1 pa_xName, TNameType2 pa_xTypeName) {
      return CDataSpecBuilderBase::addData(pa_xName, pa_xTypeName);
    }
    TCSpecReferenceRange addDataRange(const char *pa_sPrefix, int pa_nRangeSize) {
      return CDataSpecBuilderBase::addDataRange(pa_sPrefix, pa_nRangeSize);
    }
    template<typename TNameType>
    TCSpecReferenceRange addDataRange(const char *pa_sPrefix, int pa_nRangeSize, TNameType pa_xTypeName) {
      return CDataSpecBuilderBase::addDataRange(pa_sPrefix, pa_nRangeSize, pa_xTypeName);
    }

    template<typename TNameType>
    TCSpecReference operator[] (TNameType pa_tName) const {
      return { findData(pa_tName) };
    }
};

class CWithSpecBuilderBase {
  public:
    void setStaticBindings(const TDataIOID *pa_aunStaticBindings, const TForteInt16 *pa_anStaticIndexes, std::size_t pa_unNumEvents);
    template<std::size_t NBindings, std::size_t NEvents>
    void setStaticBindings(const std::array<TDataIOID, NBindings> &pa_aunStaticBindings, const std::array<TForteInt16, NEvents> &pa_anStaticIndexes) {
      setStaticBindings(pa_aunStaticBindings.data(), pa_anStaticIndexes.data(), NEvents);
    }
    void bind(TDataIOID pa_unEventId, TDataIOID pa_unDataId);
    void bind(TDataIOID pa_unEventId, std::initializer_list<TDataIOID> pa_aunDataIds) {
      for (auto dataId : pa_aunDataIds) {
        bind(pa_unEventId, dataId);
      }
    }
    void bindRange(TDataIOID pa_unEventId, TDataIOID pa_unFirstDataId, TDataIOID pa_unLastDataId);
    
    std::size_t calcStorageSize(std::size_t pa_unNumEvents) const;
    std::tuple<const TDataIOID*, const TForteInt16*> build(CMixedStorage &pa_oStorage, std::size_t pa_unNumEvents);

    bool isGood() const { return m_bIsGood; }

  private:
    bool m_bIsGood = true;
    const TDataIOID *m_aunStaticBindings = nullptr;
    const TForteInt16 *m_anStaticIndexes = nullptr;
    std::size_t m_unNumStaticEvents = 0;
    std::vector<std::vector<TDataIOID>> m_vvDynamicList;

    void grow(std::size_t pa_unNumEvents);

  protected:
    bool check(bool pa_bState) {
      m_bIsGood = m_bIsGood && pa_bState;
      return isGood();
    }
};

template<class DirTag>
class CWithSpecBuilder : public CWithSpecBuilderBase {
  public:
    using TCSpecEventReference = CSpecReference<CEventSpecTag, DirTag>;
    using TCSpecDataReference = CSpecReference<CDataSpecTag, DirTag>;
    using TCSpecDataReferenceRange = CSpecReferenceRange<CDataSpecTag, DirTag>;

    void bind(TCSpecEventReference pa_oEventRef, TCSpecDataReference pa_oDataRef) {
      if (check(pa_oEventRef.isValid() && pa_oDataRef.isValid())) {
        CWithSpecBuilderBase::bind(*pa_oEventRef, *pa_oDataRef);
      }
    }
    void bind(TCSpecEventReference pa_oEventRef, std::initializer_list<TCSpecDataReference> pa_aoDataRefs) {
      check(pa_oEventRef.isValid());
      for (auto ref : pa_aoDataRefs) {
        if (!check(ref.isValid()))
            break;
        bind(pa_oEventRef, *ref);
      }
    }

    void bindRange(TCSpecEventReference pa_oEventRef, TCSpecDataReference pa_oFirstDataRef, TCSpecDataReference pa_oLastDataRef) {
      if (check(pa_oEventRef.isValid() && pa_oFirstDataRef.isValid() && pa_oLastDataRef.isValid())) {
        CWithSpecBuilderBase::bindRange(*pa_oEventRef, *pa_oFirstDataRef, *pa_oLastDataRef);
      }
    }
    void bindRange(TCSpecEventReference pa_oEventRef, TCSpecDataReferenceRange pa_oDataRefRange) {
      if (check(pa_oEventRef.isValid() && pa_oDataRefRange.isValid())) {
        CWithSpecBuilderBase::bindRange(*pa_oEventRef, *pa_oDataRefRange.m_oFirst, *pa_oDataRefRange.m_oLast);
      }
    }
};

class CIfSpecBuilder {
  public:
    CEventSpecBuilder<CInputSpecTag>  m_oEI;
    CEventSpecBuilder<COutputSpecTag> m_oEO;
    CDataSpecBuilder<CInputSpecTag>   m_oDI;
    CDataSpecBuilder<COutputSpecTag>  m_oDO;
    CWithSpecBuilder<CInputSpecTag>   m_oIWith;
    CWithSpecBuilder<COutputSpecTag>  m_oOWith;

    template<class DirTag>
    void bind(CSpecReference<CEventSpecTag, DirTag> pa_oEventRef, CSpecReference<CDataSpecTag, DirTag> pa_oDataRef) {
      getWithFromDir(pa_oEventRef).bind(pa_oEventRef, pa_oDataRef);
    }
    template<class DirTag>
    void bind(CSpecReference<CEventSpecTag, DirTag> pa_oEventRef, std::initializer_list<CSpecReference<CDataSpecTag, DirTag>> &&pa_loDataRef) {
      getWithFromDir(pa_oEventRef).bind(pa_oEventRef, std::move(pa_loDataRef));
    }
    template<class DirTag>
    void bindRange(CSpecReference<CEventSpecTag, DirTag> pa_oEventRef, CSpecReference<CDataSpecTag, DirTag> pa_oFirstDataRef, CSpecReference<CDataSpecTag, DirTag> pa_oLastDataRef) {
      getWithFromDir(pa_oEventRef).bindRange(pa_oEventRef, pa_oFirstDataRef, pa_oLastDataRef);
    }
    template<class DirTag>
    void bindRange(CSpecReference<CEventSpecTag, DirTag> pa_oEventRef, CSpecReferenceRange<CDataSpecTag, DirTag> pa_oDataRefRange) {
      getWithFromDir(pa_oEventRef).bindRange(pa_oEventRef, pa_oDataRefRange);
    }

    bool build(CMixedStorage &pa_oStorage, SFBInterfaceSpec &pa_oInterfaceSpec);

    bool isGood() const;

  private:
    bool m_bIsGood = true;

    constexpr CWithSpecBuilder<CInputSpecTag>& getWithFromDir(CSpecReference<CEventSpecTag, CInputSpecTag>) {
      return m_oIWith;
    }
    constexpr CWithSpecBuilder<COutputSpecTag>& getWithFromDir(CSpecReference<CEventSpecTag, COutputSpecTag>) {
      return m_oOWith;
    }
};

}

#endif /* _IF_SPEC_BUILDER_H_ */
