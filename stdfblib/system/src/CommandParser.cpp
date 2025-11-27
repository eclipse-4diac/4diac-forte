/*******************************************************************************
 * Copyright (c) 2024, 2025 Jose Cabral
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

#include "forte/util/string_utils.h"
#include "forte/device.h"

namespace forte::iec61499::system {
  CommandParser::CommandParser(CDevice &paDevice) : mDevice{paDevice} {
    mCommand.mAdditionalParams.reserve(255);
  }

  EMGMResponse CommandParser::parseAndExecuteMGMCommand(const char *const paDest, char *paCommand) {
    mLastResponse = EMGMResponse::InvalidObject;

    if (nullptr != strchr(paCommand, '>')) {
      mCommand.mID = nullptr;
      mCommand.mAdditionalParams.clear();
      mCommand.mFirstParam.clear();
      mCommand.mSecondParam.clear();
      mCommand.mMonitorResponse.clear();

      mCommand.mDestination = (strlen(paDest) != 0) ? StringId::insert(paDest) : StringId{};
      if (255 <= mCommand.mAdditionalParams.capacity()) {
        mCommand.mAdditionalParams.reserve(255);
      }

      char *acRequestPartLeft = parseRequest(paCommand);
      if (nullptr != acRequestPartLeft) {
        acRequestPartLeft = strchr(acRequestPartLeft, '<');
        if (nullptr != acRequestPartLeft) {
          acRequestPartLeft++; // point to the next character after the <
        }
        // we got the command for execution
        // now check the rest of the data
        switch (mCommand.mCMD) {
          case EMGMCommandType::CreateGroup: // create something
            parseCreateData(acRequestPartLeft);
            break;
          case EMGMCommandType::DeleteGroup: // delete something
            parseDeleteData(acRequestPartLeft);
            break;
          case EMGMCommandType::Start:
          case EMGMCommandType::Stop:
          case EMGMCommandType::Kill:
          case EMGMCommandType::Reset: parseAdditionalStateCommandData(acRequestPartLeft); break;
          case EMGMCommandType::Read: parseReadData(acRequestPartLeft); break;
          case EMGMCommandType::Write: parseWriteData(acRequestPartLeft); break;
          case EMGMCommandType::QueryGroup: // query something
            parseQueryData(acRequestPartLeft);
            break;
          default: break;
        }

        if (EMGMCommandType::INVALID != mCommand.mCMD) {
          mLastResponse = mDevice.executeMGMCommand(mCommand);
        }
      } else {
        mLastResponse = EMGMResponse::UnsupportedCmd;
      }
    }
    return mLastResponse;
  }

  void CommandParser::generateResponse(CIEC_STRING &paResponse) {
    paResponse.clear();
    if (!mCommand.mMonitorResponse.empty()) {
      generateMonitorResponse(paResponse);
      return;
    }

    if (!mCommand.mAdditionalParams.empty()) {
      generateLongResponse(paResponse);
    } else {
      generateShortResponse(paResponse);
    }
  }

  void CommandParser::generateShortResponse(CIEC_STRING &paResponse) {
    paResponse.append("<Response ID=\"");
    if (nullptr != mCommand.mID) {
      paResponse.append(mCommand.mID);
    }
    paResponse.append("\"");
    if (EMGMResponse::Ready != mLastResponse) {
      paResponse.append(" Reason=\"");
      paResponse.append(mgm_cmd::getResponseText(mLastResponse));
      paResponse.append("\"");
    }
    paResponse.append(" />");
  }

  void CommandParser::generateLongResponse(CIEC_STRING &paResponse) {
    paResponse.reserve(static_cast<TForteUInt16>(255 + (mCommand.mAdditionalParams.length())));
    paResponse.append("<Response ID=\"");
    if (nullptr != mCommand.mID) {
      paResponse.append(mCommand.mID);
    }
    paResponse.append("\"");
    if (EMGMResponse::Ready != mLastResponse) {
      paResponse.append(" Reason=\"");
      paResponse.append(mgm_cmd::getResponseText(mLastResponse));
      paResponse.append("\">\n  ");
    } else {
      paResponse.append(">\n  ");
    }

    if (mCommand.mCMD == EMGMCommandType::Read) {
      paResponse.append("<Connection Source=\"");
      appendIdentifierName(paResponse, mCommand.mFirstParam);
      paResponse.append("\" Destination=\"");
      paResponse.append(mCommand.mAdditionalParams);
      paResponse.append("\" />");
    } else {
      generateQueryResponse(paResponse);
    }

    paResponse.append("\n</Response>");
  }

  char *CommandParser::parseRequest(char *paRequestString) {
    // first check if it is an management request
    char *acCommandStart = nullptr;
    static const int scnCommandLength[] = {7, 7, 6, 5, 5, 6, 5, 6, 6};

    if (!strncmp("<Request ID=\"", paRequestString, 13)) {
      int i = 13;
      int j;
      mCommand.mID = &(paRequestString[i]);
      for (j = 0; paRequestString[i] != '\"'; ++i, ++j) {
        if (j >= 7) {
          return nullptr;
        }
      }
      paRequestString[i] = '\0'; // close ID
      ++i;
      acCommandStart = strchr((&paRequestString[i]), '\"');
      if (acCommandStart != nullptr) {
        acCommandStart++; // this is the real start of the command
        if (!strncmp("CREATE", acCommandStart, 6)) {
          mCommand.mCMD = EMGMCommandType::CreateGroup;
        } else if (!strncmp("DELETE", acCommandStart, 6)) {
          mCommand.mCMD = EMGMCommandType::DeleteGroup;
        } else if (!strncmp("START", acCommandStart, 5)) {
          mCommand.mCMD = EMGMCommandType::Start;
        } else if (!strncmp("STOP", acCommandStart, 4)) {
          mCommand.mCMD = EMGMCommandType::Stop;
        } else if (!strncmp("KILL", acCommandStart, 4)) {
          mCommand.mCMD = EMGMCommandType::Kill;
        } else if (!strncmp("RESET", acCommandStart, 5)) {
          mCommand.mCMD = EMGMCommandType::Reset;
        } else if (!strncmp("READ", acCommandStart, 4)) {
          mCommand.mCMD = EMGMCommandType::Read;
        } else if (!strncmp("WRITE", acCommandStart, 5)) {
          mCommand.mCMD = EMGMCommandType::Write;
        } else if (!strncmp("QUERY", acCommandStart, 5)) {
          mCommand.mCMD = EMGMCommandType::QueryGroup;
        } else {
          return nullptr;
        }
        acCommandStart += scnCommandLength[static_cast<int>(mCommand.mCMD)];
      }
    }
    return acCommandStart;
  }

#ifdef FORTE_DYNAMIC_TYPE_LOAD
  bool CommandParser::parseXType(char *paRequestPartLeft, const char *paRequestType) {
    bool retVal = false;
    size_t nReqLength = strlen(paRequestType);
    if (!strncmp(paRequestType, paRequestPartLeft, nReqLength)) {
      paRequestPartLeft = &(paRequestPartLeft[nReqLength]);
      if ('*' != paRequestPartLeft[0]) {
        int i = parseIdentifier(paRequestPartLeft, mCommand.mFirstParam);
        paRequestPartLeft = (-1 == i) ? nullptr : strchr(&(paRequestPartLeft[i + 1]), '>');
      }
      if (nullptr != paRequestPartLeft) {
        paRequestPartLeft++;
        char *endOfRequest = strchr(paRequestPartLeft, '<');
        *endOfRequest = '\0';
        forte::util::transformEscapedXMLToNonEscapedText(paRequestPartLeft);
        mCommand.mAdditionalParams = paRequestPartLeft;
        retVal = true;
      }
    }
    return retVal;
  }
#endif // FORTE_DYNAMIC_TYPE_LOAD

  bool CommandParser::parseFBData(char *paRequestPartLeft) {
    bool retVal = false;

    if (!strncmp("FB Name=\"", paRequestPartLeft, 9)) {
      char *acBuf = &(paRequestPartLeft[9]);
      int i = 0;
      if (acBuf[0] != '*') {
        i = parseIdentifier(acBuf, mCommand.mFirstParam);
        acBuf = (-1 == i) ? nullptr : strchr(&(acBuf[i + 1]), '\"');
      } else {
        acBuf = strchr(&(acBuf[i + 2]), '\"');
      }

      if (acBuf != nullptr) {
        if (acBuf[1] != '*') {
          ++acBuf;
          i = parseTypeName(acBuf, mCommand.mSecondParam, mCommand.mAdditionalParams);
          if (-1 != i) {
            acBuf = strchr(&(acBuf[i + 1]), '\"');
            if (acBuf != nullptr) {
              // We have an application name given
              ++acBuf;
              TForteUInt16 nBufLength = static_cast<TForteUInt16>(strcspn(acBuf, "\"") + 1);
              mCommand.mAdditionalParams.assign(acBuf, nBufLength);
            }
          } else {
            return false;
          }
        }
        retVal = true;
      }
    }
    return retVal;
  }

  int CommandParser::parseIdentifier(char *paIdentifierStart, TNameIdentifier &paIdentifier) {
    for (char *runner = paIdentifierStart, *start = paIdentifierStart; '\0' != *runner; ++runner) {
      if ('.' == *runner) {
        *runner = '\0';
        if (!paIdentifier.push_back(StringId::insert(start))) {
          return -1;
        }
        *runner = '.';
        start = runner + 1;
      } else if ('"' == *runner) {
        *runner = '\0';
        if (!paIdentifier.push_back(StringId::insert(start))) {
          return -1;
        }
        *runner = '"';
        return static_cast<int>(runner - paIdentifierStart);
      }
    }
    return -1;
  }

  int CommandParser::parseTypeName(const std::string_view paCmdString,
                                   TNameIdentifier &paIdentifier,
                                   std::string &paTypeHash) {
    size_t endIndex = paCmdString.find('"');
    if (endIndex == std::string::npos) {
      return -1;
    }
    std::string_view fbTypeName = paCmdString.substr(0, endIndex);

    size_t typeHashSeparator = fbTypeName.find('#');
    if (typeHashSeparator != std::string::npos) {
      paTypeHash = fbTypeName.substr(typeHashSeparator + 1);
      fbTypeName = fbTypeName.substr(0, typeHashSeparator);
    }

    if (!paIdentifier.push_back(StringId::insert(fbTypeName))) {
      return -1;
    }
    return static_cast<int>(endIndex + 1);
  }

  bool CommandParser::parseConnectionData(char *paRequestPartLeft) {
    bool bRetVal = false;
    if (!strncmp("Connection Source=\"", paRequestPartLeft, sizeof("Connection Source=\"") - 1)) {
      int i = parseIdentifier(&(paRequestPartLeft[19]), mCommand.mFirstParam);
      if (-1 != i) {
        char *acBuf = strchr(&(paRequestPartLeft[i + 21]), '\"');
        if (acBuf != nullptr) {
          parseIdentifier(&(acBuf[1]), mCommand.mSecondParam);
          bRetVal = (-1 != i);
        }
      }
    }
    return bRetVal;
  }

  bool CommandParser::parseWriteConnectionData(char *paRequestPartLeft) {
    bool retVal = false;
    if (!strncmp("Connection Source=\"", paRequestPartLeft, sizeof("Connection Source=\"") - 1)) {
      paRequestPartLeft = &(paRequestPartLeft[19]);

      char *endOfSource = strchr(paRequestPartLeft, '\"');
      if (nullptr == endOfSource) {
        return false;
      }
      *endOfSource = '\0';
      char *addParams = new char[strlen(paRequestPartLeft) + 1]();
      strcpy(addParams, paRequestPartLeft);
      util::transformEscapedXMLToNonEscapedText(addParams);
      mCommand.mAdditionalParams = addParams;
      delete[] (addParams);
      *endOfSource = '"'; // restore the string
      paRequestPartLeft = strchr(endOfSource + 1, '\"');
      if (nullptr != paRequestPartLeft) {
        retVal = (-1 != parseIdentifier(&paRequestPartLeft[1], mCommand.mFirstParam));
      }
    }
    return retVal;
  }

  void CommandParser::parseCreateData(char *paRequestPartLeft) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    if (nullptr != paRequestPartLeft) {
      switch (paRequestPartLeft[0]) {
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        case 'A': // we have an Adapter to Create
          if (parseXType(paRequestPartLeft, "AdapterType Name=\"")) {
            mCommand.mCMD = EMGMCommandType::CreateAdapterType;
          }
          break;
#endif // FORTE_DYNAMIC_TYPE_LOAD
        case 'F': // we have an FB to Create
          if (parseFBData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::CreateFBInstance;
          }
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          else if (parseXType(paRequestPartLeft, "FBType Name=\"")) {
            mCommand.mCMD = EMGMCommandType::CreateFBType;
          }
#endif // FORTE_DYNAMIC_TYPE_LOAD
          break;
        case 'C': // we have an Connection to Create
          if (parseConnectionData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::CreateConnection;
          }
          break;
        case 'W': // we have an Watch to Add
          if (parseMonitoringData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::MonitoringAddWatch;
          }
          break;
        default: break;
      }
    }
  }

  void CommandParser::parseDeleteData(char *paRequestPartLeft) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    if (nullptr != paRequestPartLeft) {
      switch (paRequestPartLeft[0]) {
        case 'F': // we have an FB to delete
          if (parseFBData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::DeleteFBInstance;
          }
          break;
        case 'C': // we have an Connection to delete
          if (parseConnectionData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::DeleteConnection;
          }
          break;
        case 'W': // we have an Watch to remove
          if (parseMonitoringData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::MonitoringRemoveWatch;
          }
          break;
        default: break;
      }
    }
  }

  void CommandParser::parseAdditionalStateCommandData(char *paRequestPartLeft) {
    if (nullptr != paRequestPartLeft &&
        '/' != paRequestPartLeft[0] && // if we have an additional xml token parse if it is an FB definition
        !parseFBData(paRequestPartLeft)) {
      mCommand.mCMD = EMGMCommandType::INVALID;
    }
  }

  void CommandParser::parseReadData(char *paRequestPartLeft) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    if (nullptr != paRequestPartLeft) {
      if ('W' == paRequestPartLeft[0]) {
        mCommand.mCMD = EMGMCommandType::MonitoringReadWatches;
      } else if (parseConnectionData(paRequestPartLeft)) {
        mCommand.mCMD = EMGMCommandType::Read;
      }
    }
  }

  void CommandParser::parseWriteData(char *paRequestPartLeft) {
    // We need an additional xml connection token parse if it is an connection definition
    mCommand.mCMD = EMGMCommandType::INVALID;
    if (nullptr != paRequestPartLeft && parseWriteConnectionData(paRequestPartLeft)) {
      char *pch = strstr(paRequestPartLeft, "force=\"");
      if (nullptr != pch) {
        if (!strncmp(&pch[7], "true", sizeof("true") - 1)) {
          mCommand.mCMD = EMGMCommandType::MonitoringForce;
        } else if (!strncmp(&pch[7], "false", sizeof("false") - 1)) {
          mCommand.mCMD = EMGMCommandType::MonitoringClearForce;
        }
      } else if ((2 == mCommand.mAdditionalParams.length()) &&
                 (('$' == mCommand.mAdditionalParams[0]) &&
                  (('e' == mCommand.mAdditionalParams[1]) || ('E' == mCommand.mAdditionalParams[1])))) {
        mCommand.mCMD = EMGMCommandType::MonitoringTriggerEvent;
      } else if ((3 == mCommand.mAdditionalParams.length()) &&
                 (('$' == mCommand.mAdditionalParams[0]) &&
                  (('e' == mCommand.mAdditionalParams[1]) || ('E' == mCommand.mAdditionalParams[1])) &&
                  (('r' == mCommand.mAdditionalParams[2]) || ('R' == mCommand.mAdditionalParams[2])))) {
        mCommand.mCMD = EMGMCommandType::MonitoringResetEventCount;
      } else
        mCommand.mCMD = EMGMCommandType::Write;
    }
  }

  namespace {
    constexpr std::string_view FB_TYPE = "FBType";
    constexpr std::string_view ADAPTER_TYPE = "AdapterType";
    constexpr std::string_view DATA_TYPE = "DataType";
    constexpr std::string_view GLOBAL_CONST_TYPE = "GlobalConstType";
  } // namespace

  void CommandParser::parseQueryData(char *paRequestPartLeft) {
    mCommand.mCMD = EMGMCommandType::INVALID;
    if (nullptr != paRequestPartLeft) {
      switch (paRequestPartLeft[0]) {
        case 'F': // query fb or fb type list
          if (!strncmp(paRequestPartLeft, FB_TYPE.data(), FB_TYPE.size())) {
            parseQueryTypes(paRequestPartLeft + FB_TYPE.size(), EMGMCommandType::QueryFBType,
                            EMGMCommandType::QueryFBTypes);
          } else if (parseFBData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::QueryFB;
          }
          break;
        case 'C': // query connection list
          if (parseConnectionData(paRequestPartLeft)) {
            mCommand.mCMD = EMGMCommandType::QueryConnection;
          }
          break;
        case 'D': // query datatype or datatype list
          if (!strncmp(paRequestPartLeft, DATA_TYPE.data(), DATA_TYPE.size())) {
            parseQueryTypes(paRequestPartLeft + DATA_TYPE.size(), EMGMCommandType::QueryDataType,
                            EMGMCommandType::QueryDTTypes);
          }
          break;
        case 'A': // query adaptertype list
          if (!strncmp(paRequestPartLeft, ADAPTER_TYPE.data(), ADAPTER_TYPE.size())) {
            parseQueryTypes(paRequestPartLeft + ADAPTER_TYPE.size(), EMGMCommandType::QueryAdapterType,
                            EMGMCommandType::QueryAdapterTypes);
          }
          break;
        case 'G': // query adaptertype list
          if (!strncmp(paRequestPartLeft, GLOBAL_CONST_TYPE.data(), GLOBAL_CONST_TYPE.size())) {
            parseQueryTypes(paRequestPartLeft + GLOBAL_CONST_TYPE.size(), EMGMCommandType::QueryGlobalConstType,
                            EMGMCommandType::QueryGlobalConstTypes);
          }
          break;
        default: break;
      }
    }
  }

  void CommandParser::parseQueryTypes(std::string_view paRequestPartLeft,
                                      EMGMCommandType paSingleQueryCMD,
                                      EMGMCommandType paListQueryCMD) {
    if (!paRequestPartLeft.starts_with(" Name=\"")) {
      return;
    }

    if (paRequestPartLeft[7] == '*') {
      mCommand.mCMD = paListQueryCMD;
      return;
    }

    if (parseTypeName(paRequestPartLeft.substr(7), mCommand.mFirstParam, mCommand.mAdditionalParams) != -1) {
      mCommand.mCMD = paSingleQueryCMD;
    }
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

  bool CommandParser::parseMonitoringData(char *paRequestPartLeft) {
    bool bRetVal = false;
    if (!strncmp("Watch Source=\"", paRequestPartLeft, sizeof("Watch Source=\"") - 1)) {
      int i = parseIdentifier(&(paRequestPartLeft[14]), mCommand.mFirstParam);
      if (-1 != i) {
        char *acBuf = strchr(&(paRequestPartLeft[i + 16]), '\"');
        if (acBuf != nullptr) {
          parseIdentifier(&(acBuf[1]), mCommand.mSecondParam);
          bRetVal = (-1 != i);
        }
      }
    }
    return bRetVal;
  }

  void CommandParser::generateMonitorResponse(CIEC_STRING &paResponse) {
    if (EMGMResponse::Ready != mLastResponse) {
      generateShortResponse(paResponse);
    } else {
      paResponse.reserve(mCommand.mMonitorResponse.length() + strlen(mCommand.mID) + 74);
      paResponse.append("<Response ID=\"");
      paResponse.append(mCommand.mID);
      paResponse.append("\"");
      paResponse.append(">\n  ");
      if (mCommand.mCMD == EMGMCommandType::MonitoringReadWatches) {
        paResponse.append("<Watches>\n    ");
        paResponse.append(mCommand.mMonitorResponse);
        paResponse.append("\n  </Watches>");
      }
      paResponse.append("\n</Response>");
    }
  }
} // namespace forte::iec61499::system
