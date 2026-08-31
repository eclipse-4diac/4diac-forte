/*******************************************************************************
 * Copyright (c) 2026 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include "forte/mgmcmd.h"
#include "../../stdfblib/system/src/CommandParser.h"
#include "../../forte_boost_output_support.h"

namespace data = boost::unit_test::data;

namespace forte::iec61499::system::test {

  namespace {

    bool nameIdentifierEquals(const TNameIdentifier &paName, std::initializer_list<std::string_view> paExpected) {
      if (paName.size() != paExpected.size()) {
        return false;
      }
      size_t i = 0;
      for (const auto &segment : paName) {
        if (std::string_view(segment) != *(paExpected.begin() + i)) {
          return false;
        }
        ++i;
      }
      return true;
    }

    std::string makeHierarchicalName(size_t paNumSegments) {
      std::string result;
      for (size_t i = 0; i < paNumSegments; ++i) {
        if (i > 0)
          result += '.';
        result += "seg" + std::to_string(i);
      }
      return result;
    }

    const std::vector<std::string> stateChangeActions = {"START", "STOP", "KILL", "RESET"};
    const std::vector<EMGMCommandType> stateChangeCommands = {EMGMCommandType::Start, EMGMCommandType::Stop,
                                                              EMGMCommandType::Kill, EMGMCommandType::Reset};

    const std::vector<std::string> validForceValues = {"true", "false"};
    const std::vector<EMGMCommandType> validForceExpectedCmds = {EMGMCommandType::MonitoringForce,
                                                                 EMGMCommandType::MonitoringClearForce};

    const std::vector<std::string> invalidForceValues = {"TRUE", "1", "yes", ""};

    const std::vector<std::string> eventSources = {"$e", "$E", "$er", "$ER", "$Er", "$eR"};

    const std::vector<EMGMCommandType> eventExpectedCmds = {
        EMGMCommandType::MonitoringTriggerEvent,    EMGMCommandType::MonitoringTriggerEvent,
        EMGMCommandType::MonitoringResetEventCount, EMGMCommandType::MonitoringResetEventCount,
        EMGMCommandType::MonitoringResetEventCount, EMGMCommandType::MonitoringResetEventCount};

    const std::vector<std::string> queryTypeTags = {"AdapterType", "FBType", "DataType", "GlobalConstType"};

    const std::vector<EMGMCommandType> queryTypeExpectedCmds = {
        EMGMCommandType::QueryAdapterType, EMGMCommandType::QueryFBType, EMGMCommandType::QueryDataType,
        EMGMCommandType::QueryGlobalConstType};

    const std::vector<EMGMCommandType> queryTypesExpectedCmds = {
        EMGMCommandType::QueryAdapterTypes, EMGMCommandType::QueryFBTypes, EMGMCommandType::QueryDTTypes,
        EMGMCommandType::QueryGlobalConstTypes};

  } // namespace

  BOOST_AUTO_TEST_SUITE(commandparser)

  BOOST_AUTO_TEST_CASE(createFbInstance) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::CreateFBInstance);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger"}));
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"E_CYCLE"}));
    BOOST_TEST(cmd.mID == "1");
  }

  BOOST_AUTO_TEST_CASE(createFbInstanceAcceptsWhitespaceAtEnd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)"
                                   "\n");

    BOOST_TEST(response == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(createFBInstanceHierarchicalName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><FB Name="SubApp1.Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"SubApp1", "Trigger"}));
  }

  BOOST_AUTO_TEST_CASE(createFBInstanceWithinResource) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "res1", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(std::string_view(cmd.mDestination) == "res1");
  }

  BOOST_AUTO_TEST_CASE(createFBInstanceEmptyNameIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createFBInstanceMissingCloseingNameAttributeQuote) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(createFBInstanceMissingCloseingTypeAttributeQuote) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(createConnection) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="5" Action="CREATE"><Connection Source="Test.EO" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::CreateConnection);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Test", "EO"}));
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"Trigger", "DT"}));
  }

  BOOST_AUTO_TEST_CASE(createConnectionEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="5" Action="CREATE"><Connection Source="" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createConnectionEmptyDestinationIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="5" Action="CREATE"><Connection Source="Test.EO" Destination="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createConnectionDoubleDotInSource) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><Connection Source="Test..EO" Destination="Trigger.DT" />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // A Connection's Source ending in '.' means the final segment (the port name) is itself the
  // empty string. That's valid: IEC 61131-3 Functions use the empty string as the name of their
  // return-value output port, so a Connection sourced from e.g. an ASSEMBLE_BYTE_FROM_BOOLS
  // instance's return value looks exactly like this on the wire.
  BOOST_AUTO_TEST_CASE(createConnectionSourceEndsWithEmptyPortName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><Connection Source="Test." Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Test", ""}));
  }

  BOOST_AUTO_TEST_CASE(createConnectionDoubleDotInDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><Connection Source="Test.EO" Destination="Trigger..DT" />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createConnectionDotAtEndOfDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><Connection Source="Test.EO" Destination="Trigger.DT." />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createWatchWithWildcardDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="9" Action="CREATE"><Watch Source="FF.Q" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::MonitoringAddWatch);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"FF", "Q"}));
    BOOST_TEST(cmd.mSecondParam.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(createWatchWithLiteralDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="9" Action="CREATE"><Watch Source="FF.Q" Destination="Switch.G" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::MonitoringAddWatch);
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"Switch", "G"}));
  }

  BOOST_AUTO_TEST_CASE(createWatchEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="9" Action="CREATE"><Watch Source="" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Unlike a Connection's Source, a Watch's Destination must always have a real final segment.
  BOOST_AUTO_TEST_CASE(createWatchDotAtEndOfDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="9" Action="CREATE"><Watch Source="FF.Q" Destination="Switch.G." /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createUnknownChildTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><Bogus Name="x" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(createMissingClosingTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" />)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(createFBDoubleDotInName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger..Test" Type="E_CYCLE" />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createFBDotAtEnd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger." Type="E_CYCLE" />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(createNoChild) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(createSimpleTag) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"/>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  /****************************** delete tests **************************************/
  BOOST_AUTO_TEST_CASE(deleteFbInstance) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><FB Name="Trigger" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::DeleteFBInstance);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger"}));
  }

  BOOST_AUTO_TEST_CASE(deleteFbInstanceHierarchicalName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><FB Name="SubApp1.Trigger" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"SubApp1", "Trigger"}));
  }

  BOOST_AUTO_TEST_CASE(deleteFbInstanceWithinResource) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("res1", R"(<Request ID="1" Action="DELETE"><FB Name="Trigger" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(std::string_view(cmd.mDestination) == "res1");
  }

  BOOST_AUTO_TEST_CASE(deleteFbInstanceIgnoresNonEmptyType) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::DeleteFBInstance);
  }

  BOOST_AUTO_TEST_CASE(deleteFbInstanceEmptyNameIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><FB Name="" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Symmetric with createFBDotAtEnd: an FB instance name must always have a real final segment.
  BOOST_AUTO_TEST_CASE(deleteFBDotAtEnd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><FB Name="Trigger." Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(deleteConnection) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Connection Source="Test.EO" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::DeleteConnection);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Test", "EO"}));
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"Trigger", "DT"}));
  }

  BOOST_AUTO_TEST_CASE(deleteConnectionEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Connection Source="" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(deleteConnectionEmptyDestinationIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Connection Source="T#500ms" Destination="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Symmetric with createConnectionSourceEndsWithEmptyPortName: deleting a Connection sourced
  // from an IEC 61131-3 Function's (unnamed) return-value output must work the same way.
  BOOST_AUTO_TEST_CASE(deleteConnectionSourceEndsWithEmptyPortName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Connection Source="Test." Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Test", ""}));
  }

  // Symmetric with createConnectionDotAtEndOfDestination: a Connection's Destination must always
  // have a real final segment, for DELETE as well as CREATE.
  BOOST_AUTO_TEST_CASE(deleteConnectionDotAtEndOfDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Connection Source="Test.EO" Destination="Trigger.DT." />)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(deleteWatchWithWildcardDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Watch Source="FF.Q" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::MonitoringRemoveWatch);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"FF", "Q"}));
    BOOST_TEST(cmd.mSecondParam.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(deleteWatchWithLiteralDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Watch Source="FF.Q" Destination="Switch.G" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"Switch", "G"}));
  }

  BOOST_AUTO_TEST_CASE(deleteWatchEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><Watch Source="" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Symmetric with createWatchDotAtEndOfDestination.
  BOOST_AUTO_TEST_CASE(deleteWatchDotAtEndOfDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="DELETE"><Watch Source="FF.Q" Destination="Switch.G." /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(deleteUnknownChildTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"><Bogus Name="x" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(deleteNoChild) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(deleteSimpleTag) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="DELETE"/>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  /****************************** state change tests **************************************/
  BOOST_DATA_TEST_CASE(selfClosingIsAccepted,
                       data::make(stateChangeActions) ^ data::make(stateChangeCommands),
                       actionName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action=")" + actionName + R"("/>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
  }

  BOOST_DATA_TEST_CASE(selfClosingWithWhiteSpaceAtEndIsAccepted,
                       data::make(stateChangeActions) ^ data::make(stateChangeCommands),
                       actionName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action=")" + actionName +
                      R"("/>)"
                      "\n";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
  }

  BOOST_DATA_TEST_CASE(openCloseFormIsEquivalentToSelfClosing,
                       data::make(stateChangeActions) ^ data::make(stateChangeCommands),
                       actionName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action=")" + actionName + R"("></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
  }

  BOOST_DATA_TEST_CASE(fbChildIsParsedCorrectly,
                       data::make(stateChangeActions) ^ data::make(stateChangeCommands),
                       actionName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml =
        R"(<Request ID="1" Action=")" + actionName + R"("><FB Name="Trigger" Type="E_CYCLE" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger"}));

    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"E_CYCLE"}));
  }

  BOOST_DATA_TEST_CASE(fbChildHierarchicalNameIsSplit, data::make(stateChangeActions), actionName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml =
        R"(<Request ID="1" Action=")" + actionName + R"("><FB Name="SubApp1.Trigger" Type="" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"SubApp1", "Trigger"}));
  }

  BOOST_DATA_TEST_CASE(fbChildEmptyNameIsBadParams, data::make(stateChangeActions), actionName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action=")" + actionName + R"("><FB Name="" Type="" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_DATA_TEST_CASE(unknownChildTagIsInvalidObject, data::make(stateChangeActions), actionName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml =
        R"(<Request ID="1" Action=")" + actionName + R"("><Connection Source="a" Destination="b" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  /****************************** write tests **************************************/
  BOOST_AUTO_TEST_CASE(writeOrdinaryWrite) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::Write);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger", "DT"}));
    BOOST_TEST(cmd.mAdditionalParams == "42");
  }

  BOOST_AUTO_TEST_CASE(writeSourceIsXmlUnescaped) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "",
        R"(<Request ID="1" Action="WRITE"><Connection Source="&lt;a&gt; &amp; &apos;b&apos; &quot;c&quot;" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mAdditionalParams == R"(<a> & 'b' "c")");
  }

  BOOST_AUTO_TEST_CASE(writeDestinationHierarchicalNameIsSplit) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="SubApp1.Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"SubApp1", "Trigger", "DT"}));
  }

  BOOST_AUTO_TEST_CASE(writeEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="WRITE"><Connection Source="" Destination="Trigger.DT" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(writeEmptyDestinationIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Unlike a Connection's Source in CREATE/DELETE, a WRITE's Destination must always have a real
  // final segment -- it identifies the port being written to, not a Function's return value.
  BOOST_AUTO_TEST_CASE(writeDestinationDotAtEndIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="Trigger.DT." /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(writeUnknownChildTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="WRITE"><FB Name="Trigger" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_DATA_TEST_CASE(validForceValueResolvesToExpectedCmd,
                       data::make(validForceValues) ^ data::make(validForceExpectedCmds),
                       forceValue,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="Trigger.DT" force=")" +
                      forceValue + R"(" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger", "DT"}));
    BOOST_TEST(cmd.mAdditionalParams == "42");
  }

  BOOST_DATA_TEST_CASE(invalidForceValueIsBadParams, data::make(invalidForceValues), forceValue) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="WRITE"><Connection Source="42" Destination="Trigger.DT" force=")" +
                      forceValue + R"(" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(forcePresentTakesPrecedenceOverEventMarker) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "",
        R"(<Request ID="1" Action="WRITE"><Connection Source="$e" Destination="Trigger.DT" force="true" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::MonitoringForce);
  }

  BOOST_DATA_TEST_CASE(eventMarkerSourceResolvesToExpectedCmd,
                       data::make(eventSources) ^ data::make(eventExpectedCmds),
                       sourceValue,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="WRITE"><Connection Source=")" + sourceValue +
                      R"(" Destination="Trigger.DT" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(cmd.mAdditionalParams == sourceValue);
  }

  /****************************** read tests **************************************/
  BOOST_AUTO_TEST_CASE(readConnectionWithWildcardDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="READ"><Connection Source="Trigger.DT" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::Read);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger", "DT"}));
    BOOST_TEST(cmd.mSecondParam.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(readConnectionWithLiteralDestination) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="READ"><Connection Source="Trigger.DT" Destination="Switch.G" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::Read);
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"Switch", "G"}));
  }

  BOOST_AUTO_TEST_CASE(readConnectionEmptySourceIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="READ"><Connection Source="" Destination="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(readConnectionEmptyDestinationIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="READ"><Connection Source="Trigger.DT" Destination="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  // Regression test: appendIdentifierName() appended a separator after every segment (including
  // the last) and then appended the last segment a second time, duplicating it in READ responses
  // (e.g. "SubApp1.Trigger.DT" became "SubApp1.Trigger.DT.DT").
  BOOST_AUTO_TEST_CASE(readConnectionResponseDoesNotDuplicateLastSourceSegment) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="READ"><Connection Source="SubApp1.Trigger.DT" Destination="*" /></Request>)");

    // Simulate the device manager having filled in the read value before the response is generated.
    cmd.mAdditionalParams = "TRUE";
    CIEC_STRING responseText;
    parser.generateResponse(responseText, response);

    BOOST_TEST(static_cast<std::string>(responseText) ==
               "<Response ID=\"1\">\n  <Connection Source=\"SubApp1.Trigger.DT\" Destination=\"TRUE\" />\n</Response>");
  }

  BOOST_AUTO_TEST_CASE(readWatches) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="READ"><Watches/></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::MonitoringReadWatches);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
    BOOST_TEST(cmd.mSecondParam.size() == 0);
    BOOST_TEST(cmd.mAdditionalParams.empty());
  }

  BOOST_AUTO_TEST_CASE(readWatchesWithinResource) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("res1", R"(<Request ID="1" Action="READ"><Watches/></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(std::string_view(cmd.mDestination) == "res1");
  }

  BOOST_AUTO_TEST_CASE(readMissingChildIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="READ"/>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(readMissingChildOpenCloseFormIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="READ"></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(readUnknownChildTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="READ"><FB Name="Trigger" Type="" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  /****************************** query tests **************************************/
  BOOST_AUTO_TEST_CASE(queryFbBothWildcards) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><FB Name="*" Type="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == EMGMCommandType::QueryFB);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
    BOOST_TEST(cmd.mSecondParam.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(queryFbNameOnly) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><FB Name="Trigger" Type="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger"}));
    BOOST_TEST(cmd.mSecondParam.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(queryFbTypeOnly) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><FB Name="*" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"E_CYCLE"}));
  }

  BOOST_AUTO_TEST_CASE(queryFbBothSpecified) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"Trigger"}));
    BOOST_TEST(nameIdentifierEquals(cmd.mSecondParam, {"E_CYCLE"}));
  }

  BOOST_AUTO_TEST_CASE(queryFbHierarchicalName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="QUERY"><FB Name="SubApp1.Trigger" Type="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"SubApp1", "Trigger"}));
  }

  BOOST_AUTO_TEST_CASE(queryFbEmptyNameIsBadParams) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><FB Name="" Type="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_DATA_TEST_CASE(queryTypeWildcardNameLeavesFirstParamEmpty,
                       data::make(queryTypeTags) ^ data::make(queryTypesExpectedCmds),
                       tagName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="QUERY"><)" + tagName + R"( Name="*" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(cmd.mFirstParam.size() == 0);
  }

  BOOST_DATA_TEST_CASE(queryTypeLiteralNameIsStored,
                       data::make(queryTypeTags) ^ data::make(queryTypeExpectedCmds),
                       tagName,
                       expectedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="QUERY"><)" + tagName + R"( Name="MyType" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(cmd.mCMD == expectedCmd);
    BOOST_TEST(nameIdentifierEquals(cmd.mFirstParam, {"MyType"}));
  }

  BOOST_DATA_TEST_CASE(queryTypeEmptyNameIsBadParams, data::make(queryTypeTags), tagName) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    std::string xml = R"(<Request ID="1" Action="QUERY"><)" + tagName + R"( Name="" /></Request>)";
    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::BadParams);
  }

  BOOST_AUTO_TEST_CASE(queryWithinResource) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("res1", R"(<Request ID="1" Action="QUERY"><FB Name="*" Type="*" /></Request>)");

    BOOST_TEST(response == EMGMResponse::Ready);
    BOOST_TEST(std::string_view(cmd.mDestination) == "res1");
  }

  BOOST_AUTO_TEST_CASE(queryMissingChildIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"/>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(queryUnknownChildTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="QUERY"><Watches /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  /****************************** malformed XML **************************************/

  BOOST_AUTO_TEST_CASE(missingClosingQuoteOnIdIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1 Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(missingClosingAngleBracketIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"<FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(missingIdAttributeIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(missingActionAttributeIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(attributesInWrongOrderIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request Action="CREATE" ID="1"><FB Name="Trigger" Type="E_CYCLE" /></Request>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(missingClosingRequestTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response =
        parser.parseMGMCommand("", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" />)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(mismatchedClosingTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Request ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Response>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(wrongRootTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand(
        "", R"(<Response ID="1" Action="CREATE"><FB Name="Trigger" Type="E_CYCLE" /></Response>)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(trailingContentAfterClosingTagIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="START"/> garbage)");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(emptyInputIsInvalidObject) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", "");

    BOOST_TEST(response == EMGMResponse::InvalidObject);
  }

  BOOST_AUTO_TEST_CASE(unknownActionIsUnsupportedCmd) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto response = parser.parseMGMCommand("", R"(<Request ID="1" Action="FOOBAR"/>)");

    BOOST_TEST(response == EMGMResponse::UnsupportedCmd);
  }

  BOOST_AUTO_TEST_CASE(nameHierarchyAtMaxIsAccepted) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto name = makeHierarchicalName(forte::cgMaxSupportedNameHierarchy);
    std::string xml = R"(<Request ID="1" Action="CREATE"><FB Name=")" + name + R"(" Type="E_CYCLE" /></Request>)";

    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Ready);
  }

  BOOST_AUTO_TEST_CASE(nameHierarchyOverMaxIsOverflow) {
    SManagementCMD cmd;
    CommandParser parser(cmd);

    auto name = makeHierarchicalName(forte::cgMaxSupportedNameHierarchy + 1);
    std::string xml = R"(<Request ID="1" Action="CREATE"><FB Name=")" + name + R"(" Type="E_CYCLE" /></Request>)";

    auto response = parser.parseMGMCommand("", xml);

    BOOST_TEST(response == EMGMResponse::Overflow);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::iec61499::system::test
