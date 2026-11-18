/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>

#include "forte/mgmcmdstruct.h"
#include "forte/iec61499/events/E_SR_fbt.h"
#include "../../core/src/typelib_internal.h"

#include "forte_boost_output_support.h"
#include "fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;

namespace forte::test {

  namespace {
    class CTestRuntimeFBTypeEntry final : public CRuntimeFBTypeEntry {
      public:
        CTestRuntimeFBTypeEntry(StringId paTypeNameId, std::string paDefinition) :
            CRuntimeFBTypeEntry(paTypeNameId, {}),
            mDefinition(std::move(paDefinition)) {
        }

        std::string_view getDefinition() const override {
          return mDefinition;
        }

        CFunctionBlock *createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const override {
          return new iec61499::events::FORTE_E_SR(paInstanceNameId, paContainer);
        }

      private:
        std::string mDefinition;
    };

    class CTestRuntimeAdapterTypeEntry final : public CRuntimeAdapterTypeEntry {
      public:
        explicit CTestRuntimeAdapterTypeEntry(StringId paTypeNameId) : CRuntimeAdapterTypeEntry(paTypeNameId, {}) {
        }

        std::string_view getDefinition() const override {
          return "adapter";
        }

        CAdapter *createAdapterInstance(StringId, CFBContainer &, bool, TForteUInt8) const override {
          return nullptr;
        }
    };

    EMGMResponse executeMGMCommand(CResource &paResource, SManagementCMD paCommand, std::string *paResult = nullptr) {
      const EMGMResponse response = paResource.executeMGMCommand(paCommand);
      if (paResult != nullptr) {
        *paResult = paCommand.mAdditionalParams;
      }
      return response;
    }

    bool contains(const std::vector<StringId> &paNames, StringId paName) {
      return std::find(paNames.begin(), paNames.end(), paName) != paNames.end();
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(RuntimeTypeLibTests)

  BOOST_AUTO_TEST_CASE(registerAndCreate) {
    const StringId typeName = "runtimetest::Created"_STRID;
    auto entry = std::make_shared<CTestRuntimeFBTypeEntry>(typeName, "first");
    BOOST_TEST(registerRuntimeFBType(entry) == EMGMResponse::Ready);
    BOOST_TEST(getRuntimeFBTypeEntry(typeName) == entry);
    BOOST_TEST(contains(getRuntimeFBTypeNames(), typeName));
    BOOST_TEST(getFBTypeEntry(typeName) == nullptr);

    CInternalFB<iec61499::system::EMB_RES> resource("RuntimeTypeLibTests_create"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::CreateFBInstance,
                                                .mFirstParam = {"Instance"_STRID},
                                                .mSecondParam = {typeName},
                                            }) == EMGMResponse::Ready);
    BOOST_TEST(executeMGMCommand(*resource, {
                                                .mCMD = EMGMCommandType::DeleteFBInstance,
                                                .mFirstParam = {"Instance"_STRID},
                                            }) == EMGMResponse::Ready);
    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(queriesListRuntimeTypes) {
    const StringId fbTypeName = "runtimetest::Queried"_STRID;
    const StringId adapterTypeName = "runtimetest::QueriedAdapter"_STRID;
    BOOST_REQUIRE(registerRuntimeFBType(std::make_shared<CTestRuntimeFBTypeEntry>(fbTypeName, "q")) ==
                  EMGMResponse::Ready);
    BOOST_REQUIRE(registerRuntimeAdapterType(std::make_shared<CTestRuntimeAdapterTypeEntry>(adapterTypeName)) ==
                  EMGMResponse::Ready);
    BOOST_TEST(contains(getRuntimeAdapterTypeNames(), adapterTypeName));

    CInternalFB<iec61499::system::EMB_RES> resource("RuntimeTypeLibTests_query"_STRID,
                                                    CFBTestDataGlobalFixture::getDevice());
    BOOST_REQUIRE(resource->initialize());
    std::string result;
    BOOST_TEST(executeMGMCommand(*resource, {.mCMD = EMGMCommandType::QueryFBTypes}, &result) == EMGMResponse::Ready);
    BOOST_TEST(result.find("iec61499::events::E_SR, ") != std::string::npos);
    BOOST_TEST(result.find(", runtimetest::Queried") != std::string::npos);
    BOOST_TEST(result.find(", , ") == std::string::npos);

    BOOST_TEST(executeMGMCommand(*resource, {.mCMD = EMGMCommandType::QueryAdapterTypes}, &result) ==
               EMGMResponse::Ready);
    BOOST_TEST(result.find("runtimetest::QueriedAdapter") != std::string::npos);

    BOOST_TEST(executeMGMCommand(*resource, {.mCMD = EMGMCommandType::QueryFBType, .mFirstParam = {fbTypeName}},
                                 &result) == EMGMResponse::Ready);
    BOOST_TEST(result == "<FBType Name=\"runtimetest::Queried#\"/>");
    BOOST_TEST(executeMGMCommand(*resource,
                                 {.mCMD = EMGMCommandType::QueryAdapterType, .mFirstParam = {adapterTypeName}},
                                 &result) == EMGMResponse::Ready);
    BOOST_TEST(executeMGMCommand(*resource,
                                 {.mCMD = EMGMCommandType::QueryFBType, .mFirstParam = {"runtimetest::Unknown"_STRID}},
                                 &result) == EMGMResponse::UnsupportedType);
    resource->deinitialize();
  }

  BOOST_AUTO_TEST_CASE(firmwareTypesCannotBeRedefined) {
    BOOST_TEST(registerRuntimeFBType(std::make_shared<CTestRuntimeFBTypeEntry>("iec61499::events::E_SR"_STRID, "x")) ==
               EMGMResponse::InvalidOperation);
    BOOST_TEST(getRuntimeFBTypeEntry("iec61499::events::E_SR"_STRID) == nullptr);
  }

  BOOST_AUTO_TEST_CASE(redefinitionPolicy) {
    const StringId typeName = "runtimetest::Redefined"_STRID;
    auto first = std::make_shared<CTestRuntimeFBTypeEntry>(typeName, "one");
    BOOST_REQUIRE(registerRuntimeFBType(first) == EMGMResponse::Ready);

    // the same definition again keeps the registered entry
    BOOST_TEST(registerRuntimeFBType(std::make_shared<CTestRuntimeFBTypeEntry>(typeName, "one")) ==
               EMGMResponse::Ready);
    BOOST_TEST(getRuntimeFBTypeEntry(typeName) == first);

    // a different definition is refused while instances of the old one exist
    first->addInstance();
    auto second = std::make_shared<CTestRuntimeFBTypeEntry>(typeName, "two");
    BOOST_TEST(registerRuntimeFBType(second) == EMGMResponse::InvalidState);
    BOOST_TEST(getRuntimeFBTypeEntry(typeName) == first);

    // and replaces it once they are gone
    first->removeInstance();
    BOOST_TEST(registerRuntimeFBType(second) == EMGMResponse::Ready);
    BOOST_TEST(getRuntimeFBTypeEntry(typeName) == second);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
