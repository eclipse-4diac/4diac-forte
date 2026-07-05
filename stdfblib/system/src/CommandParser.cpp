/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral, Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "CommandParser.h"

#include <cstring>
#include <string_view>

#include "forte/mgmcmd.h"
#include "forte/util/string_utils.h"

namespace forte::iec61499::system {

  namespace detail {
    CommandScanner::CommandScanner(std::string_view paRemaining) : mRemaining(paRemaining) {
    }

    void CommandScanner::skipWhiteSpace() {
      auto pos = mRemaining.find_first_not_of(" \t\r\n");
      mRemaining = (pos == std::string_view::npos) ? std::string_view{} : mRemaining.substr(pos);
    }

    bool CommandScanner::consume(std::string_view paToConsume) {
      if (!mRemaining.starts_with(paToConsume)) {
        return false;
      }
      mRemaining = mRemaining.substr(paToConsume.size());
      return true;
    }

    std::string_view CommandScanner::takeUntil(char paDelimiter) {
      auto pos = mRemaining.find(paDelimiter);
      if (pos == std::string_view::npos) {
        return {};
      }
      auto token = mRemaining.substr(0, pos);
      mRemaining = mRemaining.substr(pos + 1);
      return token;
    }

    char CommandScanner::peek() const {
      return mRemaining.empty() ? '\0' : mRemaining.front();
    }

    bool CommandScanner::empty() const {
      return mRemaining.empty();
    }
  } // namespace detail

  namespace {
    struct ActionEntry {
        std::string_view mActionName;
        EMGMCommandType mCommand;
    };

    constexpr ActionEntry actionTable[] = {
        {"CREATE", EMGMCommandType::CreateGroup}, {"DELETE", EMGMCommandType::DeleteGroup},
        {"START", EMGMCommandType::Start},        {"STOP", EMGMCommandType::Stop},
        {"KILL", EMGMCommandType::Kill},          {"RESET", EMGMCommandType::Reset},
        {"READ", EMGMCommandType::Read},          {"WRITE", EMGMCommandType::Write},
        {"QUERY", EMGMCommandType::QueryGroup},
    };

    EMGMCommandType parseActionAttributeValue(detail::CommandScanner &paScanner) {
      auto action = paScanner.takeUntil('"');
      for (const auto &entry : actionTable) {
        if (entry.mActionName == action) {
          return entry.mCommand;
        }
      }
      return EMGMCommandType::INVALID;
    }

    bool proceedToOpenBracket(detail::CommandScanner &paScanner) {
      paScanner.skipWhiteSpace();
      return paScanner.consume("<");
    }

    bool proceedToClosingBracket(detail::CommandScanner &paScanner) {
      paScanner.skipWhiteSpace();
      return paScanner.consume("/>");
    }

    bool checkRequestClosingTag(detail::CommandScanner &paScanner) {
      if (paScanner.empty()) {
        // we had a single request tag command
        return true;
      }
      paScanner.skipWhiteSpace();
      if (!paScanner.consume("/Request>")) {
        return false;
      }
      paScanner.skipWhiteSpace();
      return paScanner.empty();
    }

    bool isAsteriskValue(std::string_view paValue) {
      return paValue.size() == 1 && paValue[0] == '*';
    }

    bool isTriggerEvent(std::string_view paValue) {
      return paValue.size() == 2 && paValue[0] == '$' && (paValue[1] == 'e' || paValue[1] == 'E');
    }

    bool isResetEventCount(std::string_view paValue) {
      return paValue.size() == 3 && paValue[0] == '$' && (paValue[1] == 'e' || paValue[1] == 'E') &&
             (paValue[1] == 'r' || paValue[1] == 'R');
    }

  } // namespace

  CommandParser::CommandParser(SManagementCMD &paCommand) : mCommand(paCommand) {
  }

  EMGMResponse CommandParser::parseMGMCommand(std::string_view paDest, std::string_view paCommand) {
    detail::CommandScanner scanner(paCommand);
    mCommand.clear();
    mCommand.mDestination = (!paDest.empty()) ? StringId::insert(paDest) : StringId{};

    EMGMResponse resp = parseRequest(scanner);
    if (resp != EMGMResponse::Ready) {
      return resp;
    }

    if (!scanner.empty() && !proceedToOpenBracket(scanner)) {
      return EMGMResponse::InvalidObject;
    }

    switch (mCommand.mCMD) {
      case EMGMCommandType::CreateGroup: // create something
        resp = parseCreateData(scanner);
        break;
      case EMGMCommandType::DeleteGroup: // delete something
        resp = parseDeleteData(scanner);
        break;
      case EMGMCommandType::Start:
      case EMGMCommandType::Stop:
      case EMGMCommandType::Kill:
      case EMGMCommandType::Reset: resp = parseAdditionalStateCommandData(scanner); break;
      case EMGMCommandType::Read: resp = parseReadData(scanner); break;
      case EMGMCommandType::Write: resp = parseWriteData(scanner); break;
      case EMGMCommandType::QueryGroup: // query something
        resp = parseQueryData(scanner);
        break;
      default: break;
    }

    if (resp != EMGMResponse::Ready) {
      return resp;
    }

    return checkRequestClosingTag(scanner) ? EMGMResponse::Ready : EMGMResponse::InvalidObject;
  }

  void CommandParser::generateResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse) {
    paResponseText.clear();
    if (!mCommand.mMonitorResponse.empty()) {
      generateMonitorResponse(paResponseText, paResponse);
      return;
    }

    if (!mCommand.mAdditionalParams.empty()) {
      generateLongResponse(paResponseText, paResponse);
    } else {
      generateShortResponse(paResponseText, paResponse);
    }
  }

  void CommandParser::generateShortResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse) {
    paResponseText.append("<Response ID=\"");
    paResponseText.append(mCommand.mID);
    paResponseText.append("\"");
    if (EMGMResponse::Ready != paResponse) {
      paResponseText.append(" Reason=\"");
      paResponseText.append(mgm_cmd::getResponseText(paResponse));
      paResponseText.append("\"");
    }
    paResponseText.append(" />");
  }

  void CommandParser::generateLongResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse) {
    paResponseText.reserve(static_cast<TForteUInt16>(255 + (mCommand.mAdditionalParams.length())));
    paResponseText.append("<Response ID=\"");
    paResponseText.append(mCommand.mID);
    paResponseText.append("\"");
    if (EMGMResponse::Ready != paResponse) {
      paResponseText.append(" Reason=\"");
      paResponseText.append(mgm_cmd::getResponseText(paResponse));
      paResponseText.append("\">\n  ");
    } else {
      paResponseText.append(">\n  ");
    }

    if (mCommand.mCMD == EMGMCommandType::Read) {
      paResponseText.append("<Connection Source=\"");
      appendIdentifierName(paResponseText, mCommand.mFirstParam);
      paResponseText.append("\" Destination=\"");
      paResponseText.append(mCommand.mAdditionalParams);
      paResponseText.append("\" />");
    } else {
      generateQueryResponse(paResponseText);
    }

    paResponseText.append("\n</Response>");
  }

  EMGMResponse CommandParser::parseRequest(detail::CommandScanner &paScanner) {
    if (!paScanner.consume(R"(<Request ID=")")) {
      return EMGMResponse::InvalidObject;
    }

    mCommand.mID = paScanner.takeUntil('"');

    paScanner.skipWhiteSpace();
    if (!paScanner.consume(R"(Action=")")) {
      return EMGMResponse::InvalidObject;
    }

    mCommand.mCMD = parseActionAttributeValue(paScanner);
    if (mCommand.mCMD == EMGMCommandType::INVALID) {
      return EMGMResponse::UnsupportedCmd;
    }

    paScanner.skipWhiteSpace();
    if (!paScanner.consume(">") && !paScanner.consume("/>")) {
      // we have not just the closing bracket
      return EMGMResponse::InvalidObject;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CommandParser::parseFBData(detail::CommandScanner &paScanner) {
    if (!paScanner.consume(R"(FB Name=")")) {
      return EMGMResponse::InvalidObject;
    }

    auto fbInstanceName = paScanner.takeUntil('"');
    if (!isAsteriskValue(fbInstanceName) && !parseIdentifier(fbInstanceName, mCommand.mFirstParam)) {
      return EMGMResponse::Overflow;
    }

    paScanner.skipWhiteSpace();
    if (!paScanner.consume(R"(Type=")")) {
      return EMGMResponse::InvalidObject;
    }

    auto fbTypeName = paScanner.takeUntil('"');
    if (!isAsteriskValue(fbInstanceName) &&
        !parseTypeName(fbTypeName, mCommand.mSecondParam, mCommand.mAdditionalParams)) {
      return EMGMResponse::Overflow;
    }

    if (!proceedToClosingBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return EMGMResponse::Ready;
  }

  bool CommandParser::parseIdentifier(std::string_view paIdentifierString, TNameIdentifier &paIdentifier) {
    while (!paIdentifierString.empty()) {
      auto pos = paIdentifierString.find('.');
      if (pos == std::string_view::npos) {
        if (!paIdentifier.push_back(StringId::insert(paIdentifierString))) {
          return false;
        }
        return true;
      }
      if (!paIdentifier.push_back(StringId::insert(paIdentifierString.substr(0, pos)))) {
        return false;
      }
      paIdentifierString = paIdentifierString.substr(pos + 1);
    }
    return true;
  }

  bool CommandParser::parseTypeName(const std::string_view paTypeString,
                                    TNameIdentifier &paIdentifier,
                                    std::string &paTypeHash) {
    std::string_view fbTypeName = paTypeString;

    size_t typeHashSeparator = fbTypeName.find('#');
    if (typeHashSeparator != std::string::npos) {
      paTypeHash = fbTypeName.substr(typeHashSeparator + 1);
      fbTypeName = fbTypeName.substr(0, typeHashSeparator);
    }

    return paIdentifier.push_back(StringId::insert(fbTypeName));
  }

  EMGMResponse CommandParser::parseConnectionData(detail::CommandScanner &paScanner) {
    if (!paScanner.consume("Connection ")) {
      return EMGMResponse::InvalidObject;
    }
    return parseConnectionContent(paScanner);
  }

  EMGMResponse CommandParser::parseConnectionContent(detail::CommandScanner &paScanner) {
    if (!paScanner.consume(R"(Source=")")) {
      return EMGMResponse::InvalidObject;
    }
    auto source = paScanner.takeUntil('"');
    if (!parseIdentifier(source, mCommand.mFirstParam)) {
      return EMGMResponse::Overflow;
    }

    paScanner.skipWhiteSpace();
    if (!paScanner.consume(R"(Destination=")")) {
      return EMGMResponse::InvalidObject;
    }

    auto destination = paScanner.takeUntil('"');
    if (!parseIdentifier(destination, mCommand.mSecondParam)) {
      return EMGMResponse::Overflow;
    }

    if (!proceedToClosingBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CommandParser::parseCreateData(detail::CommandScanner &paScanner) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    EMGMResponse resp = EMGMResponse::InvalidObject;
    switch (paScanner.peek()) {
      case 'F': // we have an FB to Create
        resp = parseFBData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::CreateFBInstance;
        }
        break;
      case 'C': // we have an Connection to Create
        resp = parseConnectionData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::CreateConnection;
        }
        break;
      case 'W': // we have an Watch to Add
        resp = parseMonitoringData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::MonitoringAddWatch;
        }
        break;
      default: break;
    }
    if (resp == EMGMResponse::Ready && !proceedToOpenBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return resp;
  }

  EMGMResponse CommandParser::parseDeleteData(detail::CommandScanner &paScanner) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    EMGMResponse resp = EMGMResponse::InvalidObject;
    switch (paScanner.peek()) {
      case 'F': // we have an FB to delete
        resp = parseFBData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::DeleteFBInstance;
        }
        break;
      case 'C': // we have an Connection to delete
        resp = parseConnectionData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::DeleteConnection;
        }
        break;
      case 'W': // we have an Watch to remove
        resp = parseMonitoringData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::MonitoringRemoveWatch;
        }
        break;
      default: break;
    }
    if (resp == EMGMResponse::Ready && !proceedToOpenBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return resp;
  }

  EMGMResponse CommandParser::parseAdditionalStateCommandData(detail::CommandScanner &paScanner) {
    if (paScanner.peek() == 'F') {
      EMGMResponse resp = parseFBData(paScanner);
      return (resp == EMGMResponse::Ready && !proceedToOpenBracket(paScanner)) ? EMGMResponse::InvalidObject : resp;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CommandParser::parseReadData(detail::CommandScanner &paScanner) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    EMGMResponse resp = EMGMResponse::InvalidObject;
    switch (paScanner.peek()) {
      case 'C':
        resp = parseConnectionData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::Read;
        }
        break;
      case 'W':
        if (!paScanner.consume("Watches/>")) {
          return EMGMResponse::InvalidObject;
        }
        resp = EMGMResponse::Ready;
        mCommand.mCMD = EMGMCommandType::MonitoringReadWatches;
      default: break;
    }
    if (resp == EMGMResponse::Ready && !proceedToOpenBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return resp;
  }

  EMGMResponse CommandParser::parseWriteData(detail::CommandScanner &paScanner) {
    if (!paScanner.consume(R"(Connection Source=")")) {
      return EMGMResponse::InvalidObject;
    }
    auto source = paScanner.takeUntil('"');
    char *addParams = new char[source.size() + 1]();
    memcpy(addParams, source.data(), source.size());
    addParams[source.size()] = '\0';
    util::transformEscapedXMLToNonEscapedText(addParams);
    mCommand.mAdditionalParams = addParams;
    delete[] (addParams);

    paScanner.skipWhiteSpace();
    if (!paScanner.consume(R"(Destination=")")) {
      return EMGMResponse::InvalidObject;
    }
    auto destination = paScanner.takeUntil('"');
    if (!parseIdentifier(destination, mCommand.mFirstParam)) {
      return EMGMResponse::Overflow;
    }

    paScanner.skipWhiteSpace();
    if (paScanner.consume(R"(force=")")) {
      auto forceVal = paScanner.takeUntil('"');
      if (forceVal == "true") {
        mCommand.mCMD = EMGMCommandType::MonitoringForce;
      } else if (forceVal == "false") {
        mCommand.mCMD = EMGMCommandType::MonitoringClearForce;
      } else {
        return EMGMResponse::BadParams;
      }
    } else if (isTriggerEvent(mCommand.mAdditionalParams)) {
      mCommand.mCMD = EMGMCommandType::MonitoringTriggerEvent;
    } else if (isResetEventCount(mCommand.mAdditionalParams)) {
      mCommand.mCMD = EMGMCommandType::MonitoringResetEventCount;
    } else {
      mCommand.mCMD = EMGMCommandType::Write;
    }

    if (!proceedToClosingBracket(paScanner) || !proceedToOpenBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CommandParser::parseQueryData(detail::CommandScanner &paScanner) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    EMGMResponse resp = EMGMResponse::InvalidObject;
    switch (paScanner.peek()) {
      case 'F': // query fb or fb type list
        if (paScanner.consume("FBType")) {
          resp = parseQueryTypes(paScanner, EMGMCommandType::QueryFBType, EMGMCommandType::QueryFBTypes);
        } else {
          resp = parseFBData(paScanner);
          if (resp == EMGMResponse::Ready) {
            mCommand.mCMD = EMGMCommandType::QueryFB;
          }
        }
        break;
      case 'C': // query connection list
        resp = parseConnectionData(paScanner);
        if (resp == EMGMResponse::Ready) {
          mCommand.mCMD = EMGMCommandType::QueryConnection;
        }
        break;
      case 'D': // query datatype or datatype list
        if (paScanner.consume("DataType")) {
          parseQueryTypes(paScanner, EMGMCommandType::QueryDataType, EMGMCommandType::QueryDTTypes);
        }
        break;
      case 'A': // query adaptertype list
        if (paScanner.consume("AdapterType")) {
          resp = parseQueryTypes(paScanner, EMGMCommandType::QueryAdapterType, EMGMCommandType::QueryAdapterTypes);
        }
        break;
      case 'G': // query adaptertype list
        if (paScanner.consume("GlobalConstType")) {
          resp =
              parseQueryTypes(paScanner, EMGMCommandType::QueryGlobalConstType, EMGMCommandType::QueryGlobalConstTypes);
        }
        break;
      default: break;
    }
    if (resp == EMGMResponse::Ready && !proceedToOpenBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return resp;
  }

  EMGMResponse CommandParser::parseQueryTypes(detail::CommandScanner &paScanner,
                                              EMGMCommandType paSingleQueryCMD,
                                              EMGMCommandType paListQueryCMD) {
    paScanner.skipWhiteSpace();
    if (!paScanner.consume(R"(Name=")")) {
      return EMGMResponse::InvalidObject;
    }

    auto nameVal = paScanner.takeUntil('"');

    if (nameVal == "*") {
      mCommand.mCMD = paListQueryCMD;
    } else {
      if (!parseTypeName(nameVal, mCommand.mFirstParam, mCommand.mAdditionalParams)) {
        return EMGMResponse::InvalidObject;
      }
      mCommand.mCMD = paSingleQueryCMD;
    }

    if (!proceedToClosingBracket(paScanner)) {
      return EMGMResponse::InvalidObject;
    }
    return EMGMResponse::Ready;
  }

  void CommandParser::generateQueryResponse(CIEC_STRING &paResponse) {
    switch (mCommand.mCMD) {
      case EMGMCommandType::QueryConnection:
        if ((mCommand.mFirstParam.empty()) && (mCommand.mSecondParam.empty())) { // src & dst = *
          paResponse.append(mCommand.mAdditionalParams);
        } else { // either src or dst = * (both != * should be treated by generateResponse
          paResponse.append("<EndpointList>\n    ");
          paResponse.append(mCommand.mAdditionalParams);
          paResponse.append("\n  </EndpointList>");
        }
        break;
      case EMGMCommandType::QueryFB:
        if (!mCommand.mFirstParam.empty()) { // Name != "*"
          if (!mCommand.mSecondParam.empty()) { // Type != "*"
            paResponse.append("<FBStatus Status=\"");
            paResponse.append(mCommand.mAdditionalParams);
            paResponse.append("\" />");
          } else { // Type == "*"
            paResponse.append("<FB Name=\"");
            appendIdentifierName(paResponse, mCommand.mFirstParam);
            paResponse.append("\" Type=\"");
            paResponse.append(mCommand.mAdditionalParams);
            paResponse.append("\" />");
          }
        } else {
          paResponse.append("<FBList>\n    ");
          paResponse.append(mCommand.mAdditionalParams);
          paResponse.append("\n  </FBList>");
        }
        break;
      case EMGMCommandType::QueryFBTypes:
      case EMGMCommandType::QueryAdapterTypes:
      case EMGMCommandType::QueryGlobalConstTypes:
        paResponse.append("<NameList>\n    ");
        paResponse.append(mCommand.mAdditionalParams);
        paResponse.append("\n  </NameList>");
        break;
      case EMGMCommandType::QueryDTTypes:
        paResponse.append("<DTList>\n    ");
        paResponse.append(mCommand.mAdditionalParams);
        paResponse.append("\n  </DTList>");
        break;
      case EMGMCommandType::QueryFBType:
      case EMGMCommandType::QueryDataType:
      case EMGMCommandType::QueryAdapterType:
      case EMGMCommandType::QueryGlobalConstType: paResponse.append(mCommand.mAdditionalParams); break;
      default: break;
    }
  }

  void CommandParser::appendIdentifierName(CIEC_STRING &paDest, TNameIdentifier &paIdentifier) {
    if (!paIdentifier.empty()) {
      for (const auto &runner : paIdentifier) {
        paDest.append(runner.data());
        paDest.append(".");
      }
      paDest.append(paIdentifier.back().data());
    }
  }

  EMGMResponse CommandParser::parseMonitoringData(detail::CommandScanner &paScanner) {
    if (!paScanner.consume("Watch ")) {
      return EMGMResponse::InvalidObject;
    }
    return parseConnectionContent(paScanner);
  }

  void CommandParser::generateMonitorResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse) {
    if (paResponse != EMGMResponse::Ready) {
      generateShortResponse(paResponseText, paResponse);
    } else {
      paResponseText.reserve(mCommand.mMonitorResponse.length() + mCommand.mID.length() + 74);
      paResponseText.append("<Response ID=\"");
      paResponseText.append(mCommand.mID);
      paResponseText.append("\"");
      paResponseText.append(">\n  ");
      if (mCommand.mCMD == EMGMCommandType::MonitoringReadWatches) {
        paResponseText.append("<Watches>\n    ");
        paResponseText.append(mCommand.mMonitorResponse);
        paResponseText.append("\n  </Watches>");
      }
      paResponseText.append("\n</Response>");
    }
  }
} // namespace forte::iec61499::system
