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

    // REQ copies IN to OUT plus one and fires CNF, the comparison needs '<' and '&' to be escaped
    constexpr char cIncrement[] = R"lua(
local text = "a < b & 'c'"
return {
  ECC = function(fb, id)
    if id == 0 and #text > 0 then
      fb[67108864] = fb[33554432] + 1
      fb(0)
    end
  end,
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"IN"}, DIDataTypeNames = {"INT"},
    numDOs = 1, DONames = {"OUT"}, DODataTypeNames = {"INT"},
    numAdapters = 1,
    adapterInstanceDefinition = {{adapterNameID = "P", adapterTypeNameID = "luamgmtest::AValue", isPlug = true}}
  }
}
)lua";

    constexpr char cWrapper[] = R"lua(
return {
  interfaceSpec = {
    numEIs = 1, EINames = {"REQ"}, EIWith = {0, 65535}, EIWithIndexes = {0},
    numEOs = 1, EONames = {"CNF"}, EOWith = {0, 65535}, EOWithIndexes = {0},
    numDIs = 1, DINames = {"IN"}, DIDataTypeNames = {"INT"},
    numDOs = 1, DONames = {"OUT"}, DODataTypeNames = {"INT"},
    numAdapters = 0, adapterInstanceDefinition = {}
  },
  fbnSpec = {
    internalFBs = {{fbNameID = "INC", fbTypeID = "luamgmtest::Increment"}}, parameters = {},
    eventConnections = {
      {srcID = "REQ", srcFBNum = -1, dstID = "REQ", dstFBNum = 0},
      {srcID = "CNF", srcFBNum = 0, dstID = "CNF", dstFBNum = -1}
    },
    dataConnections = {
      {srcID = "IN", srcFBNum = -1, dstID = "IN", dstFBNum = 0},
      {srcID = "OUT", srcFBNum = 0, dstID = "OUT", dstFBNum = -1}
    },
    adapterConnections = {},
    numFBs = 1, numECons = 2, numDCons = 2, numAdpCons = 0, numParams = 0
  }
}
)lua";

    const std::string cFBInterface =
        "interfaceSpec = {numEIs = 0, EINames = {}, EIWith = {}, EIWithIndexes = {}, numEOs = 0, EONames = {}, "
        "EOWith = {}, EOWithIndexes = {}, numDIs = 1, DINames = {\"IN\"}, DIDataTypeNames = {\"%s\"}, numDOs = 0, "
        "DONames = {}, DODataTypeNames = {}, numAdapters = 0, adapterInstanceDefinition = {}}";

    std::string basicFB(std::string_view paDataType, std::string_view paBody = "") {
      std::string interface = cFBInterface;
      interface.replace(interface.find("%s"), 2, paDataType);
      return "return {ECC = function(fb, id) " + std::string(paBody) + " end, " + interface + "}";
    }

    //! one INT input IN that goes to two internal FBs of the given type
    std::string compositeFB(std::string_view paInternalType) {
      const std::string type(paInternalType);
      std::string interface = cFBInterface;
      interface.replace(interface.find("%s"), 2, "INT");
      return "return {" + interface + ", fbnSpec = {internalFBs = {{fbNameID = \"A\", fbTypeID = \"" + type +
             "\"}, {fbNameID = \"B\", fbTypeID = \"" + type +
             "\"}}, parameters = {}, eventConnections = {}, dataConnections = {{srcID = \"IN\", srcFBNum = -1, "
             "dstID = \"IN\", dstFBNum = 0}, {srcID = \"IN\", srcFBNum = -1, dstID = \"IN\", dstFBNum = 1}}, "
             "adapterConnections = {}, numFBs = 2, numECons = 0, numDCons = 2, numAdpCons = 0, numParams = 0}}";
    }

    std::string escape(std::string_view paText) {
      std::string escaped(paText);
      util::transformNonEscapedToEscapedXMLText(escaped, 0);
      return escaped;
    }

    std::string createRequest(std::string_view paElement, std::string_view paName, std::string_view paSource) {
      return "<Request ID=\"3\" Action=\"CREATE\"><" + std::string(paElement) + " Name=\"" + std::string(paName) +
             "\">" + escape(paSource) + "</" + std::string(paElement) + "></Request>";
    }

    std::string queryRequest(std::string_view paElement, std::string_view paName) {
      return "<Request ID=\"5\" Action=\"QUERY\"><" + std::string(paElement) + " Name=\"" + std::string(paName) +
             "\" /></Request>";
    }

    //! the response to a type query that carries the registered name and hash
    std::string typeResponse(std::string_view paElement,
                             std::string_view paName,
                             std::string_view paHash,
                             std::string_view paReason = "") {
      std::string response = "<Response ID=\"5\"";
      if (!paReason.empty()) {
        response += " Reason=\"" + std::string(paReason) + "\"";
      }
      return response + ">\n  <" + std::string(paElement) + " Name=\"" + std::string(paName) + "#" +
             std::string(paHash) + "\"/>\n</Response>";
    }

    //! A resource of its own, with the request path of the device manager: parse, execute, respond
    class CManagedResource {
      public:
        explicit CManagedResource(StringId paName) : mResource(paName, CFBTestDataGlobalFixture::getDevice()) {
          BOOST_REQUIRE(mResource->initialize());
        }

        ~CManagedResource() {
          mResource->deinitialize();
        }

        EMGMResponse execute(const std::string &paRequest, std::string &paResponseText) {
          SManagementCMD command;
          iec61499::system::CommandParser parser(command);
          EMGMResponse response = parser.parseMGMCommand("", paRequest);
          if (response == EMGMResponse::Ready) {
            response = mResource->executeMGMCommand(command);
          }
          CIEC_STRING text;
          parser.generateResponse(text, response);
          paResponseText = text.getStorage();
          return response;
        }

        EMGMResponse execute(const std::string &paRequest) {
          std::string responseText;
          return execute(paRequest, responseText);
        }

        CResource &operator*() {
          return *mResource;
        }

      private:
        CInternalFB<iec61499::system::EMB_RES> mResource;
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(LuaMGMTests)

  BOOST_AUTO_TEST_CASE(createTypesAndUseThem) {
    CManagedResource resource("LuaMGMTest_create"_STRID);
    std::string responseText;

    BOOST_TEST(resource.execute(createRequest("AdapterType", "luamgmtest::AValue", cAdapter), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"3\" />");
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Increment", cIncrement), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"3\" />");
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Wrapper", cWrapper)) == EMGMResponse::Ready);

    BOOST_TEST(resource.execute(R"(<Request ID="4" Action="QUERY"><FBType Name="*" /></Request>)", responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("luamgmtest::Increment") != std::string::npos);
    BOOST_TEST(responseText.find("luamgmtest::Wrapper") != std::string::npos);
    BOOST_TEST(resource.execute(R"(<Request ID="5" Action="QUERY"><AdapterType Name="*" /></Request>)", responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("luamgmtest::AValue") != std::string::npos);

    BOOST_TEST(
        resource.execute(R"(<Request ID="6" Action="CREATE"><FB Name="W" Type="luamgmtest::Wrapper" /></Request>)") ==
        EMGMResponse::Ready);
    BOOST_TEST(
        resource.execute(R"(<Request ID="7" Action="WRITE"><Connection Source="41" Destination="W.IN" /></Request>)") ==
        EMGMResponse::Ready);
    BOOST_TEST(
        resource.execute(R"(<Request ID="8" Action="READ"><Connection Source="W.IN" Destination="*" /></Request>)",
                         responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText.find("Destination=\"41\"") != std::string::npos);

    // the same definition again changes nothing
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Wrapper", cWrapper)) == EMGMResponse::Ready);
    // another definition while W exists
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Wrapper", basicFB("INT")), responseText) ==
               EMGMResponse::InvalidState);
    BOOST_TEST(responseText == "<Response ID=\"3\" Reason=\"INVALID_STATE\" />");

    BOOST_TEST(resource.execute(R"(<Request ID="9" Action="DELETE"><FB Name="W" Type="*" /></Request>)") ==
               EMGMResponse::Ready);
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Wrapper", basicFB("INT"))) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(reasons) {
    CManagedResource resource("LuaMGMTest_reasons"_STRID);
    std::string responseText;
    const auto createFB = [&resource, &responseText](std::string_view paName, std::string_view paSource) {
      return resource.execute(createRequest("FBType", paName, paSource), responseText);
    };

    BOOST_TEST(createFB("luamgmtest::Syntax", "return {") == EMGMResponse::InvalidObject);
    BOOST_TEST(responseText == "<Response ID=\"3\" Reason=\"INVALID_OBJECT\" />");
    BOOST_TEST(createFB("luamgmtest::Runtime", "error('at load time')") == EMGMResponse::InvalidObject);
    BOOST_TEST(createFB("luamgmtest::NoTable", "return 1") == EMGMResponse::InvalidObject);
    BOOST_TEST(createFB("luamgmtest::Schema", "return {ECC = function() end}") == EMGMResponse::BadParams);
    BOOST_TEST(createFB("luamgmtest::Neither", "return {}") == EMGMResponse::BadParams);
    BOOST_TEST(createFB("luamgmtest::DataType", basicFB("luamgmtest::NoSuchStruct")) == EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText == "<Response ID=\"3\" Reason=\"UNSUPPORTED_TYPE\" />");
    BOOST_TEST(createFB("iec61499::events::E_CTU", basicFB("INT")) == EMGMResponse::InvalidOperation);
    BOOST_TEST(createFB("not a name", basicFB("INT")) == EMGMResponse::BadParams);
    BOOST_TEST(createFB("luamgmtest::Fine", basicFB("INT")) == EMGMResponse::Ready);
    BOOST_TEST(resource.execute(createRequest("AdapterType", "luamgmtest::BadAdapter", "return {}")) ==
               EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(runawayAndHostileModules) {
    CManagedResource resource("LuaMGMTest_hostile"_STRID);
    const auto create = [&resource](std::string_view paElement, std::string_view paName, std::string_view paSource) {
      return resource.execute(createRequest(paElement, paName, paSource));
    };

    // the instruction budget of CREATE stops modules that do not finish, even when they catch errors
    BOOST_TEST(create("FBType", "luamgmtest::Endless", "while true do end") == EMGMResponse::InvalidObject);
    BOOST_TEST(create("FBType", "luamgmtest::Swallow", "while true do pcall(function() while true do end end) end") ==
               EMGMResponse::InvalidObject);
    BOOST_TEST(create("AdapterType", "luamgmtest::EndlessAdapter", "while true do end") == EMGMResponse::InvalidObject);
    BOOST_TEST(create("FBType", "luamgmtest::Recursion", "local function f() return f() + 1 end return f()") ==
               EMGMResponse::InvalidObject);

    // the tables of a module are read raw, metamethods never run
    BOOST_TEST(
        create("FBType", "luamgmtest::Trap", "return setmetatable({}, {__index = function() while true do end end})") ==
        EMGMResponse::BadParams);
    std::string trappedInterface = basicFB("INT");
    trappedInterface.replace(trappedInterface.find("interfaceSpec = {"), 17,
                             "interfaceSpec = setmetatable({}, {__index = function() error('trap') end}), x = {");
    BOOST_TEST(create("FBType", "luamgmtest::TrappedInterface", trappedInterface) == EMGMResponse::BadParams);
    BOOST_TEST(create("FBType", "luamgmtest::TrappedNetwork",
                      "return {fbnSpec = setmetatable({}, {__index = function() error('trap') end}), " +
                          basicFB("INT").substr(basicFB("INT").find("interfaceSpec"))) == EMGMResponse::BadParams);

    // nothing of the refused modules was registered
    std::string responseText;
    BOOST_TEST(resource.execute(R"(<Request ID="4" Action="QUERY"><FBType Name="*" /></Request>)", responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("luamgmtest::Endless") == std::string::npos);
    BOOST_TEST(responseText.find("luamgmtest::Trap") == std::string::npos);

    // a module that loops in its ECC only is created, the budget applies to the module's top level
    BOOST_TEST(create("FBType", "luamgmtest::LoopsInECC", basicFB("INT", "while true do end")) == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(typeHashFunction) {
    BOOST_TEST(computeTypeHash("") == "cbf29ce484222325");
    BOOST_TEST(computeTypeHash("a") == "af63dc4c8601ec8c");
    BOOST_TEST(computeTypeHash("foobar") == "85944171f73967e8");
    // bytes above 0x7f count as unsigned, and the digits are padded with zeros
    BOOST_TEST(computeTypeHash("\xff") == "af64724c8602eb6e");
    BOOST_TEST(computeTypeHash("\xc3\xa9") == "0ac21707b7181e01");
    // interned, the view stays valid and equal for the same source
    BOOST_TEST(getInternedTypeHash("foobar") == "85944171f73967e8");
    BOOST_TEST(getInternedTypeHash("foobar").data() == getInternedTypeHash("foobar").data());
  }

  BOOST_AUTO_TEST_CASE(queryTypeHashes) {
    CManagedResource resource("LuaMGMTest_queryHashes"_STRID);
    std::string responseText;
    const std::string basicSource = basicFB("INT", "local unused = \"<&>\"");
    const std::string compositeSource = compositeFB("luamgmtest::HashBasic");
    const std::string basicHash = computeTypeHash(basicSource);
    const std::string compositeHash = computeTypeHash(compositeSource);
    const std::string adapterHash = computeTypeHash(cAdapter);

    BOOST_REQUIRE(resource.execute(createRequest("AdapterType", "luamgmtest::HashAdapter", cAdapter)) ==
                  EMGMResponse::Ready);
    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::HashBasic", basicSource)) ==
                  EMGMResponse::Ready);
    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::HashComposite", compositeSource)) ==
                  EMGMResponse::Ready);

    // without a hash the query answers the name with the hash of the unescaped source
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashBasic"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::HashBasic", basicHash));
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashComposite"), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::HashComposite", compositeHash));
    BOOST_TEST(resource.execute(queryRequest("AdapterType", "luamgmtest::HashAdapter"), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("AdapterType", "luamgmtest::HashAdapter", adapterHash));

    // the matching hash gives an empty Ready response
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashBasic#" + basicHash), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"5\" />");
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashComposite#" + compositeHash), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"5\" />");
    BOOST_TEST(resource.execute(queryRequest("AdapterType", "luamgmtest::HashAdapter#" + adapterHash), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == "<Response ID=\"5\" />");

    // another hash is refused with the registered one
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashBasic#0123456789abcdef"), responseText) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::HashBasic", basicHash, "UNSUPPORTED_TYPE"));
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::HashComposite#" + basicHash), responseText) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::HashComposite", compositeHash, "UNSUPPORTED_TYPE"));
    BOOST_TEST(resource.execute(queryRequest("AdapterType", "luamgmtest::HashAdapter#"), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("AdapterType", "luamgmtest::HashAdapter", adapterHash));
    BOOST_TEST(resource.execute(queryRequest("AdapterType", "luamgmtest::HashAdapter#" + basicHash), responseText) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText == typeResponse("AdapterType", "luamgmtest::HashAdapter", adapterHash, "UNSUPPORTED_TYPE"));

    // unknown types answer without a body
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::NoHashType#" + basicHash), responseText) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText == "<Response ID=\"5\" Reason=\"UNSUPPORTED_TYPE\" />");
  }

  BOOST_AUTO_TEST_CASE(typeHashFollowsRedefinition) {
    CManagedResource resource("LuaMGMTest_redefineHashes"_STRID);
    std::string responseText;
    const std::string first = basicFB("INT");
    const std::string second = basicFB("DINT");
    const std::string adapterSecond = std::string(cAdapter) + "-- second version\n";

    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::Rehash", first)) == EMGMResponse::Ready);
    BOOST_REQUIRE(resource.execute(createRequest("AdapterType", "luamgmtest::RehashAdapter", cAdapter)) ==
                  EMGMResponse::Ready);
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::Rehash"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::Rehash", computeTypeHash(first)));

    // without instances a new definition replaces the old one and its hash
    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::Rehash", second)) == EMGMResponse::Ready);
    BOOST_TEST(computeTypeHash(first) != computeTypeHash(second));
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::Rehash"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::Rehash", computeTypeHash(second)));
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::Rehash#" + computeTypeHash(first)), responseText) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText ==
               typeResponse("FBType", "luamgmtest::Rehash", computeTypeHash(second), "UNSUPPORTED_TYPE"));

    BOOST_REQUIRE(resource.execute(createRequest("AdapterType", "luamgmtest::RehashAdapter", adapterSecond)) ==
                  EMGMResponse::Ready);
    BOOST_TEST(resource.execute(queryRequest("AdapterType", "luamgmtest::RehashAdapter"), responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText ==
               typeResponse("AdapterType", "luamgmtest::RehashAdapter", computeTypeHash(adapterSecond)));

    // a definition refused because of an instance leaves the hash alone
    BOOST_REQUIRE(
        resource.execute(R"(<Request ID="6" Action="CREATE"><FB Name="R" Type="luamgmtest::Rehash" /></Request>)") ==
        EMGMResponse::Ready);
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Rehash", first)) == EMGMResponse::InvalidState);
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::Rehash"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::Rehash", computeTypeHash(second)));
    // the same definition again keeps it as well
    BOOST_TEST(resource.execute(createRequest("FBType", "luamgmtest::Rehash", second)) == EMGMResponse::Ready);
    BOOST_TEST(resource.execute(queryRequest("FBType", "luamgmtest::Rehash"), responseText) == EMGMResponse::Ready);
    BOOST_TEST(responseText == typeResponse("FBType", "luamgmtest::Rehash", computeTypeHash(second)));
  }

  BOOST_AUTO_TEST_CASE(createInstancesWithTypeHash) {
    CManagedResource resource("LuaMGMTest_createWithHash"_STRID);
    std::string responseText;
    const std::string basicSource = basicFB("INT");
    const std::string compositeSource = compositeFB("luamgmtest::HashedBasic");
    const auto createInstance = [&resource, &responseText](std::string_view paName, std::string_view paType) {
      return resource.execute("<Request ID=\"6\" Action=\"CREATE\"><FB Name=\"" + std::string(paName) + "\" Type=\"" +
                                  std::string(paType) + "\" /></Request>",
                              responseText);
    };

    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::HashedBasic", basicSource)) ==
                  EMGMResponse::Ready);
    BOOST_REQUIRE(resource.execute(createRequest("FBType", "luamgmtest::HashedComposite", compositeSource)) ==
                  EMGMResponse::Ready);

    BOOST_TEST(createInstance("B", "luamgmtest::HashedBasic#" + computeTypeHash(basicSource)) == EMGMResponse::Ready);
    BOOST_TEST(createInstance("C", "luamgmtest::HashedComposite#" + computeTypeHash(compositeSource)) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("Reason") == std::string::npos);

    BOOST_TEST(createInstance("WrongB", "luamgmtest::HashedBasic#" + computeTypeHash(compositeSource)) ==
               EMGMResponse::UnsupportedType);
    BOOST_TEST(responseText.find("Reason=\"UNSUPPORTED_TYPE\"") != std::string::npos);
    BOOST_TEST(createInstance("WrongC", "luamgmtest::HashedComposite#cbf29ce484222325") ==
               EMGMResponse::UnsupportedType);

    BOOST_TEST(resource.execute(R"(<Request ID="7" Action="QUERY"><FB Name="*" Type="*" /></Request>)", responseText) ==
               EMGMResponse::Ready);
    BOOST_TEST(responseText.find("Name=\"B\"") != std::string::npos);
    BOOST_TEST(responseText.find("Name=\"C\"") != std::string::npos);
    BOOST_TEST(responseText.find("Name=\"WrongB\"") == std::string::npos);
    BOOST_TEST(responseText.find("Name=\"WrongC\"") == std::string::npos);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::lua::test
