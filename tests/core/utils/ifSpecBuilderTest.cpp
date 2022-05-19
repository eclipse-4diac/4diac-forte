/*******************************************************************************
 * Copyright (c) 2022 Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <cstring>
#include <boost/test/unit_test.hpp>
#include "../../../src/core/utils/mixedStorage.h"
#include "../../../src/core/utils/ifSpecBuilder.h"

#if BOOST_VERSION < 106400
    BOOST_TEST_DONT_PRINT_LOG_VALUE(std::nullptr_t)
#endif

using namespace forte::core::util;


BOOST_AUTO_TEST_SUITE(IfSpecBuilder_Test)
  static constexpr const CStringDictionary::TStringId constStringIdList1[] = {1, 2, 3};
  static constexpr const CStringDictionary::TStringId constStringIdList2[] = {4, 5, 6};
  static constexpr std::array<TDataIOID, 3> staticBindings = {0, 1, 255};
  static constexpr std::array<TForteInt16, 1> staticIndexes = {0};
  SFBInterfaceSpec ifspec;
  CMixedStorage storage;

  void build(CIfSpecBuilder &uut) {
    memset(&ifspec, 0xCC, sizeof(ifspec));
    storage.clear();
    BOOST_REQUIRE(uut.build(storage, ifspec));
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_EmptyBuild) {
    CIfSpecBuilder uut;
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_nNumEIs, 0);
    BOOST_CHECK_EQUAL(ifspec.m_nNumEOs, 0);
    BOOST_CHECK_EQUAL(ifspec.m_nNumDIs, 0);
    BOOST_CHECK_EQUAL(ifspec.m_nNumDOs, 0);
    BOOST_CHECK_EQUAL(ifspec.m_nNumAdapters, 0);
  }

  static auto strid(const char *s) {
    auto strId = CStringDictionary::getInstance().getId(s);
    BOOST_REQUIRE(strId != CStringDictionary::scm_nInvalidStringId);
    return strId;
  }

  template<class DirTag>
  void add_events(CEventSpecBuilder<DirTag> &e, int n) {
    for (int i = 0; i < n; ++i) {
      char name[16];
      snprintf(name, sizeof(name), "E%d", i + 1);
      BOOST_REQUIRE(e.addEvent(name));
    }
  }

  template<class DirTag>
  void add_data(CDataSpecBuilder<DirTag> &e, int n) {
    for (int i = 0; i < n; ++i) {
      char name[16], typeName[16];
      snprintf(name, sizeof(name), "D%d", i + 1);
      snprintf(typeName, sizeof(typeName), "T%d", i);
      BOOST_REQUIRE(e.addData(name, typeName));
    }
  }

  void test_events(CIfSpecBuilder &uut, TForteUInt8 &n, const CStringDictionary::TStringId *&f) {
    build(uut);
    BOOST_REQUIRE_EQUAL(n, 2);
    BOOST_REQUIRE_NE(f, nullptr);
    BOOST_CHECK_EQUAL(f[0], strid("E1"));
    BOOST_CHECK_EQUAL(f[1], strid("E2"));
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_SetStaticInputEvents) {
    CIfSpecBuilder uut;
    uut.m_oEI.setStaticEvents(constStringIdList1, 3);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_nNumEIs, 3);
    BOOST_CHECK_EQUAL(ifspec.m_aunEINames, constStringIdList1);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_SetStaticOutputEvents) {
    CIfSpecBuilder uut;
    uut.m_oEO.setStaticEvents(constStringIdList2, 3);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_nNumEOs, 3);
    BOOST_CHECK_EQUAL(ifspec.m_aunEONames, constStringIdList2);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddInputEvents) {
    CIfSpecBuilder uut;
    add_events(uut.m_oEI, 2);
    test_events(uut, ifspec.m_nNumEIs, ifspec.m_aunEINames);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddOutputEvents) {
    CIfSpecBuilder uut;
    add_events(uut.m_oEO, 2);
    test_events(uut, ifspec.m_nNumEOs, ifspec.m_aunEONames);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddEventsRange) {
    CIfSpecBuilder uut;
    uut.m_oEI.addEventRange("E", 2);
    test_events(uut, ifspec.m_nNumEIs, ifspec.m_aunEINames);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_DefaultWith) {
    CIfSpecBuilder uut;
    add_events(uut.m_oEI, 2);
    add_events(uut.m_oEO, 1);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_anEIWithIndexes[0], -1);
    BOOST_CHECK_EQUAL(ifspec.m_anEIWithIndexes[1], -1);
    BOOST_CHECK_EQUAL(ifspec.m_anEOWithIndexes[0], -1);
  }

  void test_data(CIfSpecBuilder &uut, TForteUInt8 &n, const CStringDictionary::TStringId *&f, const CStringDictionary::TStringId *&t, CStringDictionary::TStringId dataTypeId = -1u) {
    build(uut);
    BOOST_REQUIRE_EQUAL(n, 2);
    BOOST_REQUIRE_NE(f, nullptr);
    BOOST_REQUIRE_NE(t, nullptr);
    BOOST_CHECK_EQUAL(f[0], strid("D1"));
    BOOST_CHECK_EQUAL(f[1], strid("D2"));
    if (dataTypeId == -1u) {
      BOOST_CHECK_EQUAL(t[0], strid("T0"));
      BOOST_CHECK_EQUAL(t[1], strid("T1"));
    } else {
      BOOST_CHECK_EQUAL(t[0], strid("ANY"));
      BOOST_CHECK_EQUAL(t[1], strid("ANY"));
    }
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddInputData) {
    CIfSpecBuilder uut;
    add_data(uut.m_oDI, 2);
    test_data(uut, ifspec.m_nNumDIs, ifspec.m_aunDINames, ifspec.m_aunDIDataTypeNames);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddOutputData) {
    CIfSpecBuilder uut;
    add_data(uut.m_oDO, 2);
    test_data(uut, ifspec.m_nNumDOs, ifspec.m_aunDONames, ifspec.m_aunDODataTypeNames);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_AddDataRange) {
    CIfSpecBuilder uut;
    uut.m_oDI.addDataRange("D", 2);
    test_data(uut, ifspec.m_nNumDIs, ifspec.m_aunDINames, ifspec.m_aunDIDataTypeNames, strid("ANY"));
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_SetStaticInputData) {
    CIfSpecBuilder uut;
    uut.m_oDI.setStaticData(constStringIdList1, constStringIdList2, 3);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_nNumDIs, 3);
    BOOST_CHECK_EQUAL(ifspec.m_aunDINames, constStringIdList1);
    BOOST_CHECK_EQUAL(ifspec.m_aunDIDataTypeNames, constStringIdList2);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_SetStaticOuputData) {
    CIfSpecBuilder uut;
    uut.m_oDO.setStaticData(constStringIdList1, constStringIdList2, 3);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_nNumDOs, 3);
    BOOST_CHECK_EQUAL(ifspec.m_aunDONames, constStringIdList1);
    BOOST_CHECK_EQUAL(ifspec.m_aunDODataTypeNames, constStringIdList2);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_FindValidEventPort) {
    CEventSpecBuilder<CInputSpecTag> uut;
    uut.addEventRange("E", 2);
    auto portRef = uut["E1"];
    BOOST_CHECK(portRef.isValid());
    BOOST_CHECK_EQUAL(*portRef, 0);
    portRef = uut["E2"];
    BOOST_CHECK(portRef.isValid());
    BOOST_CHECK_EQUAL(*portRef, 1);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_FindInvalidEventPort) {
    CEventSpecBuilder<CInputSpecTag> uut;
    uut.addEventRange("E", 2);
    auto portRef = uut["E3"];
    BOOST_CHECK(!portRef.isValid());
    portRef = uut["foo"];
    BOOST_CHECK(!portRef.isValid());
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_FindValidDataPort) {
    CDataSpecBuilder<CInputSpecTag> uut;
    uut.addDataRange("D", 2);
    auto portRef = uut["D1"];
    BOOST_CHECK(portRef.isValid());
    BOOST_CHECK_EQUAL(*portRef, 0);
    portRef = uut["D2"];
    BOOST_CHECK(portRef.isValid());
    BOOST_CHECK_EQUAL(*portRef, 1);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_FindInvalidDataPort) {
    CDataSpecBuilder<CInputSpecTag> uut;
    uut.addDataRange("D", 2);
    auto portRef = uut["D3"];
    BOOST_CHECK(!portRef.isValid());
    portRef = uut["bar"];
    BOOST_CHECK(!portRef.isValid());
  }

  void test_bind(const TDataIOID *&w, const TForteInt16 *&i) {
    static constexpr std::array<TDataIOID, 5>tw = {0, 2, 255, 1, 255};
    BOOST_CHECK_EQUAL_COLLECTIONS(w, w + tw.size(), tw.begin(), tw.end());
    static constexpr std::array<TForteInt16, 3>ti = {0, 3, -1};
    BOOST_CHECK_EQUAL_COLLECTIONS(i, i + ti.size(), ti.begin(), ti.end());
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_InputWith) {
    CIfSpecBuilder uut;
    uut.m_oEI.addEventRange("E", 3);
    uut.m_oDI.addDataRange("D", 3);
    uut.bind(uut.m_oEI["E1"], {uut.m_oDI["D1"], uut.m_oDI["D3"]});
    uut.bind(uut.m_oEI["E2"], uut.m_oDI["D2"]);
    build(uut);
    test_bind(ifspec.m_anEIWith, ifspec.m_anEIWithIndexes);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_OutputWith) {
    CIfSpecBuilder uut;
    uut.m_oEO.addEventRange("E", 3);
    uut.m_oDO.addDataRange("D", 3);
    uut.bind(uut.m_oEO["E1"], {uut.m_oDO["D1"], uut.m_oDO["D3"]});
    uut.bind(uut.m_oEO["E2"], uut.m_oDO["D2"]);
    build(uut);
    test_bind(ifspec.m_anEOWith, ifspec.m_anEOWithIndexes);
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_RangeWith) {
    CIfSpecBuilder uut;
    uut.m_oEO.addEvent("E");
    uut.m_oDO.addDataRange("D", 3);
    uut.bindRange(uut.m_oEO["E"], uut.m_oDO["D1"], uut.m_oDO["D3"]);
    build(uut);
    static constexpr std::array<TDataIOID, 4> tw = {0, 1, 2, 255};
    BOOST_CHECK_EQUAL_COLLECTIONS(ifspec.m_anEOWith, ifspec.m_anEOWith + tw.size(), tw.begin(), tw.end());
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_StaticWith) {
    CIfSpecBuilder uut;
    uut.m_oOWith.setStaticBindings(staticBindings, staticIndexes);
    build(uut);
    BOOST_CHECK_EQUAL(ifspec.m_anEOWith, staticBindings.data());
    BOOST_CHECK_EQUAL(ifspec.m_anEOWithIndexes, staticIndexes.data());
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_CombinedStaticAndDynamicWith) {
    CWithSpecBuilderBase uut;
    BOOST_CHECK(uut.setStaticBindings(staticBindings, staticIndexes));
    BOOST_CHECK(!uut.bind(0, 0));
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_CombinedDynamicAndStaticWith) {
    CWithSpecBuilderBase uut;
    BOOST_CHECK(uut.bind(0, 0));
    BOOST_CHECK(!uut.setStaticBindings(staticBindings, staticIndexes));
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_CombinedStaticAndDynamicString) {
    CStringIdListSpecBuilder uut{16};
    uut.setStaticList(constStringIdList1, 3);
    uut.addString(4);
    auto out = uut.build(storage);
    BOOST_CHECK_NE(std::get<0>(out), constStringIdList1);
    BOOST_CHECK_EQUAL(std::get<1>(out), 4);
    static constexpr std::array<CStringDictionary::TStringId, 4> expected = {1, 2, 3, 4};
    BOOST_CHECK_EQUAL_COLLECTIONS(std::get<0>(out), std::get<0>(out) + 4, expected.begin(), expected.end());
  }

  BOOST_AUTO_TEST_CASE(IfSpecBuilder_StringListOverflow) {
    CStringIdListSpecBuilder uut{2};
    BOOST_CHECK(uut.addString(1));
    BOOST_CHECK(uut.addString(1));
    BOOST_CHECK(!uut.addString(1));
  }

  BOOST_AUTO_TEST_SUITE_END()
