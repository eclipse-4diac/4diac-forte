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

#include <string>

#include "../../../core/src/lua/luatypehash.h"
#include "../../../core/src/typelib_internal.h"
#include "../../../stdfblib/system/src/CommandParser.h"
#include "forte/mgmcmdstruct.h"
#include "forte/util/string_utils.h"

#include "forte_boost_output_support.h"
#include "../fbtests/fbtesterglobalfixture.h"

using namespace forte::literals;
using forte::test::CFBTestDataGlobalFixture;

namespace forte::lua::test {

  namespace {
    constexpr char cAdapter[] = R"lua(
return {interfaceSpec = {
  numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
  numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
  numDIs = 1, DINames = {"IN"}, DIDataTypeNames = {"INT"},
  numDOs = 1, DONames = {"OUT"}, DODataTypeNames = {"INT"}
}}
)lua";

    // one INT input, optionally a plug or socket named A of the given adapter type
    std::string basicFB(std::string_view paAdapter = "", bool paIsPlug = false) {
      std::string adapters = "numAdapters = 0, adapterInstanceDefinition = {}";
      if (!paAdapter.empty()) {
        adapters = "numAdapters = 1, adapterInstanceDefinition = {{adapterNameID = \"A\", adapterTypeNameID = \"" +
                   std::string(paAdapter) + "\", isPlug = " + (paIsPlug ? "true" : "false") + "}}";
      }
      return "return {ECC = function(fb, id) end, interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, "
             "EIWithIndexes = {}, numEOs = 0, EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 1, DINames = "
             "{\"IN\"}, DIDataTypeNames = {\"INT\"}, numDOs = 0, DONames = {}, DODataTypeNames = {}, " +
             adapters + "}}";
    }

    // one INT input that goes to two internal FBs of the given type
    std::string compositeFB(std::string_view paInternalType) {
      const std::string type(paInternalType);
      return "return {interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, numEOs = 0, "
             "EONames = {}, EOWith = {}, EOWithIndexes = {}, numDIs = 1, DINames = {\"IN\"}, DIDataTypeNames = "
             "{\"INT\"}, numDOs = 0, DONames = {}, DODataTypeNames = {}, numAdapters = 0, adapterInstanceDefinition = "
             "{}}, fbnSpec = {internalFBs = {{fbNameID = \"A\", fbTypeID = \"" +
             type + "\"}, {fbNameID = \"B\", fbTypeID = \"" + type +
             "\"}}, parameters = {}, eventConnections = {}, dataConnections = {{srcID = \"IN\", srcFBNum = -1, "
             "dstID = \"IN\", dstFBNum = 0}, {srcID = \"IN\", srcFBNum = -1, dstID = \"IN\", dstFBNum = 1}}, "
             "adapterConnections = {}, numFBs = 2, numECons = 0, numDCons = 2, numAdpCons = 0, numParams = 0}}";
    }

    //! the same module with another source, so another hash
    std::string changed(std::string_view paSource) {
      return std::string(paSource) + "\n-- changed\n";
    }

    std::string createTypeRequest(std::string_view paElement, std::string_view paName, std::string_view paSource) {
      std::string escaped(paSource);
      util::transformNonEscapedToEscapedXMLText(escaped, 0);
      return "<Request ID=\"1\" Action=\"CREATE\"><" + std::string(paElement) + " Name=\"" + std::string(paName) +
             "\">" + escaped + "</" + std::string(paElement) + "></Request>";
    }

    std::string createFBRequest(std::string_view paName, std::string_view paType) {
      return "<Request ID=\"2\" Action=\"CREATE\"><FB Name=\"" + std::string(paName) + "\" Type=\"" +
             std::string(paType) + "\" /></Request>";
    }

    std::string stateRequest(std::string_view paAction, std::string_view paName) {
      return "<Request ID=\"3\" Action=\"" + std::string(paAction) + "\"><FB Name=\"" + std::string(paName) +
             "\" Type=\"\" /></Request>";
    }

    /*!\brief Runs a request on the test device like the device manager does: parse, execute, respond
     *
     * An empty destination addresses the device, otherwise the resource of that name.
     */
    EMGMResponse execute(std::string_view paDestination, const std::string &paRequest, std::string &paResponseText) {
      SManagementCMD command;
      iec61499::system::CommandParser parser(command);
      EMGMResponse response = parser.parseMGMCommand(paDestination, paRequest);
      if (response == EMGMResponse::Ready) {
        response = CFBTestDataGlobalFixture::getDevice().executeMGMCommand(command);
      }
      CIEC_STRING text;
      parser.generateResponse(text, response);
      paResponseText = text.getStorage();
      return response;
    }

    EMGMResponse execute(std::string_view paDestination, const std::string &paRequest) {
      std::string responseText;
      return execute(paDestination, paRequest, responseText);
    }

    //! deletes a resource the way the IDE does: KILL and DELETE sent to the device
    void deleteResource(std::string_view paName) {
      BOOST_TEST(execute("", stateRequest("KILL", paName)) == EMGMResponse::Ready);
      BOOST_TEST(execute("", stateRequest("DELETE", paName)) == EMGMResponse::Ready);
    }

    void createResource(std::string_view paName) {
      BOOST_REQUIRE(execute("", createFBRequest(paName, "iec61499::system::EMB_RES")) == EMGMResponse::Ready);
    }

    std::size_t fbInstances(StringId paTypeName) {
      const auto entry = getRuntimeFBTypeEntry(paTypeName);
      BOOST_REQUIRE(entry != nullptr);
      return entry->getInstanceCount();
    }

    std::size_t adapterInstances(StringId paTypeName) {
      const auto entry = getRuntimeAdapterTypeEntry(paTypeName);
      BOOST_REQUIRE(entry != nullptr);
      return entry->getInstanceCount();
    }

    std::string queryHash(std::string_view paElement, std::string_view paName) {
      std::string responseText;
      BOOST_TEST(execute("",
                         "<Request ID=\"4\" Action=\"QUERY\"><" + std::string(paElement) + " Name=\"" +
                             std::string(paName) + "\" /></Request>",
                         responseText) == EMGMResponse::Ready);
      const std::string prefix = std::string(paName) + "#";
      const auto start = responseText.find(prefix);
      if (start == std::string::npos) {
        return {};
      }
      return responseText.substr(start + prefix.size(), 16);
    }
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaResourceTests)

  BOOST_AUTO_TEST_CASE(deleteResourceAndRedefineItsTypes) {
    const std::string adapter = cAdapter;
    const std::string basic = basicFB();
    const std::string plugFB = basicFB("luarestest::Adapter", true);
    const std::string socketFB = basicFB("luarestest::Adapter", false);
    const std::string composite = compositeFB("luarestest::Basic");

    BOOST_REQUIRE(execute("", createTypeRequest("AdapterType", "luarestest::Adapter", adapter)) == EMGMResponse::Ready);
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::Basic", basic)) == EMGMResponse::Ready);
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::PlugFB", plugFB)) == EMGMResponse::Ready);
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::SocketFB", socketFB)) == EMGMResponse::Ready);
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::Composite", composite)) == EMGMResponse::Ready);

    createResource("LuaResTest_R");
    BOOST_TEST(execute("LuaResTest_R", createFBRequest("B", "luarestest::Basic")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R", createFBRequest("C", "luarestest::Composite")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R", createFBRequest("P", "luarestest::PlugFB")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R", createFBRequest("S", "luarestest::SocketFB")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R",
                       R"(<Request ID="5" Action="CREATE"><Connection Source="P.A" Destination="S.A" /></Request>)") ==
               EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R", R"(<Request ID="6" Action="START"/>)") == EMGMResponse::Ready);

    BOOST_TEST(fbInstances("luarestest::Basic"_STRID) == 3U); // B and the two in C
    BOOST_TEST(fbInstances("luarestest::Composite"_STRID) == 1U);
    BOOST_TEST(fbInstances("luarestest::PlugFB"_STRID) == 1U);
    BOOST_TEST(fbInstances("luarestest::SocketFB"_STRID) == 1U);
    BOOST_TEST(adapterInstances("luarestest::Adapter"_STRID) == 2U);
    // the definitions are held by the instances
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Basic", changed(basic))) ==
               EMGMResponse::InvalidState);
    BOOST_TEST(execute("", createTypeRequest("AdapterType", "luarestest::Adapter", changed(adapter))) ==
               EMGMResponse::InvalidState);

    deleteResource("LuaResTest_R");
    std::string responseText;
    BOOST_TEST(execute("", R"(<Request ID="7" Action="QUERY"><FB Name="*" Type="*" /></Request>)", responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("LuaResTest_R") == std::string::npos);

    BOOST_TEST(fbInstances("luarestest::Basic"_STRID) == 0U);
    BOOST_TEST(fbInstances("luarestest::Composite"_STRID) == 0U);
    BOOST_TEST(fbInstances("luarestest::PlugFB"_STRID) == 0U);
    BOOST_TEST(fbInstances("luarestest::SocketFB"_STRID) == 0U);
    BOOST_TEST(adapterInstances("luarestest::Adapter"_STRID) == 0U);

    // every type takes a changed definition now, and reports its hash
    BOOST_TEST(execute("", createTypeRequest("AdapterType", "luarestest::Adapter", changed(adapter))) ==
               EMGMResponse::Ready);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Basic", changed(basic))) == EMGMResponse::Ready);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::PlugFB", changed(plugFB))) == EMGMResponse::Ready);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::SocketFB", changed(socketFB))) ==
               EMGMResponse::Ready);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Composite", changed(composite))) ==
               EMGMResponse::Ready);
    BOOST_TEST(queryHash("AdapterType", "luarestest::Adapter") == computeTypeHash(changed(adapter)));
    BOOST_TEST(queryHash("FBType", "luarestest::Basic") == computeTypeHash(changed(basic)));
    BOOST_TEST(queryHash("FBType", "luarestest::PlugFB") == computeTypeHash(changed(plugFB)));
    BOOST_TEST(queryHash("FBType", "luarestest::SocketFB") == computeTypeHash(changed(socketFB)));
    BOOST_TEST(queryHash("FBType", "luarestest::Composite") == computeTypeHash(changed(composite)));

    // a new resource of the same name uses the new definitions
    createResource("LuaResTest_R");
    BOOST_TEST(
        execute("LuaResTest_R", createFBRequest("C", "luarestest::Composite#" + computeTypeHash(changed(composite)))) ==
        EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_R", createFBRequest("P", "luarestest::PlugFB")) == EMGMResponse::Ready);
    BOOST_TEST(fbInstances("luarestest::Basic"_STRID) == 2U);
    BOOST_TEST(adapterInstances("luarestest::Adapter"_STRID) == 1U);
    BOOST_TEST(execute("LuaResTest_R", R"(<Request ID="6" Action="START"/>)") == EMGMResponse::Ready);
    deleteResource("LuaResTest_R");
    BOOST_TEST(fbInstances("luarestest::Basic"_STRID) == 0U);
    BOOST_TEST(fbInstances("luarestest::Composite"_STRID) == 0U);
    BOOST_TEST(adapterInstances("luarestest::Adapter"_STRID) == 0U);
  }

  BOOST_AUTO_TEST_CASE(deleteResourceThatNeverRan) {
    const std::string basic = basicFB();
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::Idle", basic)) == EMGMResponse::Ready);
    createResource("LuaResTest_Idle");
    BOOST_TEST(execute("LuaResTest_Idle", createFBRequest("I", "luarestest::Idle")) == EMGMResponse::Ready);
    BOOST_TEST(fbInstances("luarestest::Idle"_STRID) == 1U);

    std::string responseText;
    BOOST_TEST(execute("", stateRequest("KILL", "LuaResTest_Idle"), responseText) == EMGMResponse::InvalidState);
    BOOST_TEST(responseText == "<Response ID=\"3\" Reason=\"INVALID_STATE\" />");
    BOOST_TEST(execute("", stateRequest("DELETE", "LuaResTest_Idle"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"3\" />");

    BOOST_TEST(fbInstances("luarestest::Idle"_STRID) == 0U);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Idle", changed(basic))) == EMGMResponse::Ready);
    BOOST_TEST(queryHash("FBType", "luarestest::Idle") == computeTypeHash(changed(basic)));
  }

  BOOST_AUTO_TEST_CASE(instanceInAnotherResourceHoldsTheType) {
    const std::string basic = basicFB();
    BOOST_REQUIRE(execute("", createTypeRequest("FBType", "luarestest::Shared", basic)) == EMGMResponse::Ready);
    createResource("LuaResTest_First");
    createResource("LuaResTest_Second");
    BOOST_TEST(execute("LuaResTest_First", createFBRequest("F", "luarestest::Shared")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_Second", createFBRequest("F", "luarestest::Shared")) == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_First", R"(<Request ID="6" Action="START"/>)") == EMGMResponse::Ready);
    BOOST_TEST(execute("LuaResTest_Second", R"(<Request ID="6" Action="START"/>)") == EMGMResponse::Ready);
    BOOST_TEST(fbInstances("luarestest::Shared"_STRID) == 2U);

    deleteResource("LuaResTest_First");
    BOOST_TEST(fbInstances("luarestest::Shared"_STRID) == 1U);
    std::string responseText;
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Shared", changed(basic)), responseText) ==
               EMGMResponse::InvalidState);
    BOOST_TEST(responseText == "<Response ID=\"1\" Reason=\"INVALID_STATE\" />");
    BOOST_TEST(queryHash("FBType", "luarestest::Shared") == computeTypeHash(basic));

    deleteResource("LuaResTest_Second");
    BOOST_TEST(fbInstances("luarestest::Shared"_STRID) == 0U);
    BOOST_TEST(execute("", createTypeRequest("FBType", "luarestest::Shared", changed(basic))) == EMGMResponse::Ready);
    BOOST_TEST(queryHash("FBType", "luarestest::Shared") == computeTypeHash(changed(basic)));
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
