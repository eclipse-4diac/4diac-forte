/*******************************************************************************
 * Copyright (c) 2019, 2023 fortiss GmbH, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *    Martin Melik Merkumians - Change CIEC_STRING to std::string
 *******************************************************************************/

#include "opcua_action_info.h"
#include "core/util/parameterParser.h"
#include "core/cominfra/basecommfb.h"

const char *const CActionInfo::mActionNames[] = { "READ", "WRITE", "CREATE_METHOD", "CALL_METHOD", "SUBSCRIBE", "CREATE_OBJECT", "CREATE_VARIABLE",
  "DELETE_OBJECT", "DELETE_VARIABLE" };

CActionInfo::CActionInfo(COPC_UA_Layer &paLayer, UA_ActionType paAction, const std::string &paEndpoint) :
    mAction(paAction), mLayer(paLayer), mEndpoint(paEndpoint) {
}

bool CActionInfo::isRemote() const {
  return (!mEndpoint.empty());
}

std::unique_ptr<CActionInfo> CActionInfo::getActionInfoFromParams(const char *paParams, COPC_UA_Layer &paLayer) {
  std::unique_ptr<CActionInfo> retVal = nullptr;
  CParameterParser mainParser(paParams, ';');
  size_t amountOfParameters = mainParser.parseParameters();

  if(scmMinimumAmounOfParameters <= amountOfParameters) {
    CActionInfo::UA_ActionType action = CActionParser::getActionEnum(mainParser[CActionParser::eActionType]);
    if(CActionInfo::eActionUnknown != action) {

      std::string endpoint;
      size_t startOfNodePairs = CActionParser::eNodePairs;

      if(!CActionParser::getEndpoint(mainParser[CActionParser::eEndpoint], endpoint)) {
        startOfNodePairs--;
      }

      if(CActionInfo::eCreateMethod == action) {
        retVal.reset(new CLocalMethodInfo(paLayer, endpoint));
      } else {
        retVal.reset(new CActionInfo(paLayer, action, endpoint));
      }

      bool somethingFailed = false;
      for(size_t i = startOfNodePairs; i < amountOfParameters; i++) {
        if(!CActionParser::handlePair(mainParser[i], retVal->getNodePairInfo())) {
          somethingFailed = true;
          break;
        }
      }

      if(!somethingFailed) {
        somethingFailed = !retVal->checkAction();
      }

      if(somethingFailed) {
        retVal = nullptr;
      }
    }
  } else {
    DEVLOG_ERROR("[OPC UA ACTION]: Parameters %s should have at least %d parts, separated by a semicolon\n", paParams, scmMinimumAmounOfParameters);
  }

  return retVal;
}

const CIEC_ANY *const *CActionInfo::getDataToSend() {
  return mLayer.getCommFB()->getSDs();
}

CIEC_ANY *const *CActionInfo::getDataToReceive() const {
  return mLayer.getCommFB()->getRDs();
}

size_t CActionInfo::getSendSize() const {
  return static_cast<size_t>(mLayer.getCommFB()->getNumSD());
}

size_t CActionInfo::getReceiveSize() const {
  return static_cast<size_t>(mLayer.getCommFB()->getNumRD());
}

bool CActionInfo::checkAction() const {
  bool retVal = false;
  if(checkNodePairInfo()) {

    forte::com_infra::EComServiceType fbType = mLayer.getCommFB()->getComServiceType();
    TPortId noOfRDs = mLayer.getCommFB()->getNumRD();
    TPortId noOfSDs = mLayer.getCommFB()->getNumSD();

    switch(mAction){
      case eRead:
        retVal = checkReadAction(fbType, noOfRDs, noOfSDs);
        break;
      case eWrite:
        retVal = checkWriteAction(fbType, noOfRDs, noOfSDs);
        break;
      case eCreateMethod:
        retVal = checkCreateMethodAction(fbType, noOfRDs, noOfSDs);
        break;
      case eCallMethod:
        retVal = checkCallMethodAction(fbType, noOfRDs, noOfSDs);
        break;
      case eSubscribe:
        retVal = checkSubscribeAction(fbType, noOfRDs, noOfSDs);
        break;
      case eCreateObject:
        retVal = checkCreateObjectAction(fbType, noOfRDs, noOfSDs);
        break;
      case eCreateVariable:
        retVal = checkCreateVariableAction(fbType, noOfRDs, noOfSDs);
        break;
      case eDeleteObject:
      case eDeleteVariable:
        retVal = checkDeleteNodeAction(fbType, noOfRDs, noOfSDs);
        break;
      default:
        DEVLOG_ERROR("[OPC UA ACTION]: Unknown action %d\n", mAction);
    }
  }

  return retVal;
}

bool CActionInfo::checkNodePairInfo() const {
  bool retVal = true;
  for(auto it = mNodePair.begin(); it != mNodePair.end(); ++it) {
    if(it->getBrowsePath().empty() && nullptr == it->getNodeId()) { //browsePath AND/OR NodeId must be given. If both are empty there's a problem
      DEVLOG_ERROR("[OPC UA ACTION]: BrowsePath and NodeId are empty in FB %s\n", mLayer.getCommFB()->getInstanceName());
      retVal = false;
      break;
    }
  }
  return retVal;
}

bool CActionInfo::checkReadAction(forte::com_infra::EComServiceType paFbType, TPortId paNoOfRDs, TPortId paNoOfSDs) const {
  bool retVal = false;
  if(mEndpoint.empty()) {
    if(forte::com_infra::e_Subscriber == paFbType && paNoOfRDs == getNoOfNodePairs()) {
      retVal = true;
    } else {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Local %s action is only allowed using a Subscriber FB and the amount of BrowseName,NodeId pairs should match the number of RDs\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eRead]);
    }
  } else {
    if(paFbType != forte::com_infra::e_Client) {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Remote %s action is only allowed using a Client FB\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eRead]);
    } else if(getNoOfNodePairs() != paNoOfRDs)  {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Remote %s action needs as many BrowseName,NodeId pairs as there are RDs. (have: %i, need: %i)\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eRead], getNoOfNodePairs(), paNoOfRDs);
    } else if(paNoOfSDs != 0) {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Remote %s action must have no SDs. (have: %i)\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eRead], paNoOfSDs);
    } else {
      retVal = true;
    }
  }
  return retVal;
}

bool CActionInfo::checkWriteAction(forte::com_infra::EComServiceType paFbType, TPortId paNoOfRDs, TPortId paNoOfSDs) const {
  bool retVal = false;
  if(mEndpoint.empty()) {
    if(forte::com_infra::e_Publisher == paFbType && paNoOfSDs == getNoOfNodePairs()) {
      retVal = true;
    } else {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Local action %s is only allowed using a Publisher FB and the amount of BrowseName,NodeId pairs should match the number of SDs\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eWrite]);
    }
  } else {
    if(forte::com_infra::e_Client == paFbType && paNoOfSDs == getNoOfNodePairs() && 0 == paNoOfRDs) {
      retVal = true;
    } else {
      DEVLOG_ERROR(
        "[OPC UA ACTION]: In FB %s: Remote action %s is only allowed using a Client FB, the amount of BrowseName,NodeId pairs should match the number of SDs and must have no RDs\n",
        mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eWrite]);
    }
  }
  return retVal;
}

bool CActionInfo::checkCreateMethodAction(forte::com_infra::EComServiceType paFbType, TPortId, TPortId) const {
  bool retVal = false;
  if(forte::com_infra::e_Server == paFbType && 1 == getNoOfNodePairs()) {
    retVal = true;
  } else {
    DEVLOG_ERROR("[OPC UA ACTION]: In FB %s: %s action is only allowed using a Server FB, the amount of BrowseName,NodeId pairs should be 1\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCreateMethod]);
  }
  return retVal;
}

bool CActionInfo::checkCallMethodAction(forte::com_infra::EComServiceType paFbType, TPortId, TPortId) const {
  bool retVal = false;
  if (paFbType != forte::com_infra::e_Client) {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed using a Client FB\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCallMethod]);
  } else if(getNoOfNodePairs() != 1) {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed with a single BrowseName,NodeId pair. (have: %i)\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCallMethod], getNoOfNodePairs());
  } else if(mNodePair.begin()->getBrowsePath().empty()) {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed with a non-empty browsepath\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCallMethod]);
  } else {
    retVal = true;
  }
  return retVal;
}

bool CActionInfo::checkSubscribeAction(forte::com_infra::EComServiceType paFbType, TPortId paNoOfRDs, TPortId) const {
  bool retVal = false;
  if(forte::com_infra::e_Subscriber == paFbType && paNoOfRDs == getNoOfNodePairs()) {
    retVal = true;
  } else {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed using a Subscribe FB, the amount of BrowseName,NodeId pairs should match the number of RDs\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eSubscribe]);
  }
  return retVal;
}

bool CActionInfo::checkCreateObjectAction(forte::com_infra::EComServiceType paFbType, TPortId, TPortId paNoOfSDs) const {
  bool retVal = false;
  if(forte::com_infra::e_Publisher == paFbType && 2 == getNoOfNodePairs() && 0 == paNoOfSDs) {
    retVal = true;
  } else {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed using a Publish FB, the amount of BrowseName,NodeId pairs should be 2, and no SD must be provided\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCreateObject]);
  }
  return retVal;
}

bool CActionInfo::checkCreateVariableAction(forte::com_infra::EComServiceType paFbType, TPortId, TPortId paNoOfSDs) const {
  bool retVal = false;
  if(forte::com_infra::e_Publisher == paFbType && 3 == getNoOfNodePairs() && 0 == paNoOfSDs) {
    retVal = true;
  } else {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed using a Publish FB, the amount of BrowseName,NodeId pairs should be 3, and no SD must be provided\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eCreateVariable]);
  }
  return retVal;
}

bool CActionInfo::checkDeleteNodeAction(forte::com_infra::EComServiceType paFbType, TPortId, TPortId paNoOfSDs) const {
  bool retVal = false;
  if(forte::com_infra::e_Publisher == paFbType && 1 == getNoOfNodePairs() && 0 == paNoOfSDs) {
    retVal = true;
  } else {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: In FB %s: %s action is only allowed using a Publish FB, the amount of BrowseName,NodeId pairs should be 1, and no SD must be provided\n",
      mLayer.getCommFB()->getInstanceName(), CActionInfo::mActionNames[eDeleteObject]);
  }
  return retVal;
}

// **************  ACTION PARSER ******************* //

CActionInfo::UA_ActionType CActionInfo::CActionParser::getActionEnum(const char *paActionString) {
  CActionInfo::UA_ActionType action = CActionInfo::eActionUnknown;
  for(size_t i = 0; i < eActionUnknown; i++) {
    if(0 == strcmp(paActionString, CActionInfo::mActionNames[i])) {
      action = static_cast<UA_ActionType>(i);
      break;
    }
  }

  if(eActionUnknown == action) {
    DEVLOG_ERROR("[OPC UA ACTION]: The action %s is unrecognized\n", paActionString);
  }
  return action;
}

bool CActionInfo::CActionParser::getEndpoint(const char *paEndpoint, std::string &paResult) {
  std::string endPoint = std::string(paEndpoint);
  if(endPoint.back() == '#') {
    endPoint.pop_back();
    paResult = endPoint;
    return true;
  }
  return false;
}

bool CActionInfo::CActionParser::handlePair(const char *paPair, std::vector<CNodePairInfo> &paResult) {
  bool retVal = false;
  CParameterParser pairParser(paPair, ',');
  std::string browsePathResult;
  UA_NodeId *nodeIdResult = nullptr;
  size_t noOfParameters = pairParser.parseParameters();
  if(CActionParser::eMaxNumberOfPositions == noOfParameters) {
    browsePathResult = std::string(pairParser[CActionParser::eBrowseName]);
    nodeIdResult = parseNodeId(pairParser[CActionParser::eNodeId]);
    retVal = nodeIdResult;
  } else if(CActionParser::eMaxNumberOfPositions - 1 == noOfParameters) { //no NodeId was provided
    browsePathResult = std::string(pairParser[CActionParser::eBrowseName]);
    retVal = true;
  } else {
    DEVLOG_ERROR("[OPC UA ACTION]: The pair %s doesn't have the proper format BROWSENAME,NODEID\n", paPair);
  }

  if(retVal) {
    paResult.emplace_back(nodeIdResult, browsePathResult);
  }

  return retVal;
}

UA_NodeId* CActionInfo::CActionParser::parseNodeId(const char *paNodeIdString) {
  bool somethingFailed = false;
  UA_NodeId *resultNodeId = UA_NodeId_new();
  UA_NodeId_init(resultNodeId); //will set to default namespace 0. When the nodeId is later used, the default namespace should come from the browsename
  unsigned int identifierPosition = 0;

  CParameterParser mainParser(paNodeIdString, ':'); //namespace and identifier should be divided by a colon. If first parameter is omitted, namespace 0 is assumed
  size_t numberOfParameters = mainParser.parseParameters();

  switch(numberOfParameters){
    case CActionParser::eMaxNumberOfNodeIdPositions: //Namespace is present
      identifierPosition++;
      if(!parseNamespace(mainParser[0], *resultNodeId)) {
        somethingFailed = true;
        break;
      }
      // fall through
    case CActionParser::eMaxNumberOfNodeIdPositions - 1: //NOSONAR
      if(!parseIdentifier(mainParser[identifierPosition], *resultNodeId)) {
        somethingFailed = true;
      }
      break;
    default:
      DEVLOG_ERROR("[OPC UA ACTION]: NodeId %s has wrong number of colon\n", paNodeIdString);
      somethingFailed = true;
      break;
  }

  if(somethingFailed) {
    DEVLOG_ERROR(
      "[OPC UA ACTION]: Parsing the NodeId %s failed. The format should follow the notation '<namespaceIndex>:<identifiertype>=<identifier>'. if the part before the : is omitted, namespace 0 is assumed\n",
      paNodeIdString);
    UA_NodeId_delete(resultNodeId);
    resultNodeId = nullptr;
  }

  return resultNodeId;
}

bool CActionInfo::CActionParser::parseNamespace(const char *paNamespace, UA_NodeId &paResult) {
  paResult.namespaceIndex = static_cast<UA_UInt16>(forte::core::util::strtoul(paNamespace, nullptr, 10)); //TODO: should we check for return value here?
  return true;
}

bool CActionInfo::CActionParser::parseIdentifier(const char *paIdentifier, UA_NodeId &paResult) {
  CParameterParser identifierParser(paIdentifier, '='); //<identifiertype>=<identifier>
  if(CActionParser::eMaxNumberOfNodeIdIdenfiertPositions == identifierParser.parseParameters()) {
    if(0 == strcmp(identifierParser[CActionParser::eIdenfierType], "i")) { //numeric
      paResult.identifierType = UA_NODEIDTYPE_NUMERIC;
      paResult.identifier.numeric = static_cast<UA_UInt32>(forte::core::util::strtoul(identifierParser[CActionParser::eIdenfierValue], nullptr, 10)); //TODO: should we check for return value here?
    } else if(0 == strcmp(identifierParser[CActionParser::eIdenfierType], "s")) { //string
      paResult.identifierType = UA_NODEIDTYPE_STRING;
      paResult.identifier.string = UA_String_fromChars(identifierParser[CActionParser::eIdenfierValue]);
    } else if(0 == strcmp(identifierParser[CActionParser::eIdenfierType], "g")) { //GUID
      DEVLOG_ERROR("[OPC UA ACTION]:GUID type is not yet implemented\n");
      return false;
    } else if(0 == strcmp(identifierParser[CActionParser::eIdenfierType], "b")) { //byteString
      paResult.identifierType = UA_NODEIDTYPE_BYTESTRING;
      paResult.identifier.byteString = UA_BYTESTRING_ALLOC(identifierParser[CActionParser::eIdenfierValue]);
    } else {
      DEVLOG_ERROR("[OPC UA ACTION]: The identifier type %s wasn't recognized among the possible values [i, s, b]\n", identifierParser[0]);
      return false;
    }
  } else {
    DEVLOG_ERROR("[OPC UA ACTION]: Parsing the identifier %s failed. The format should follow the notation '<identifiertype>=<identifier>'\n", paIdentifier);
    return false;
  }
  return true;
}

// **** METHOD ACTION *****//

CLocalMethodInfo::CLocalMethodInfo(COPC_UA_Layer &paLayer, const std::string &paEndpoint) :
    CActionInfo(paLayer, eCreateMethod, paEndpoint) {
}

CLocalMethodInfo::~CLocalMethodInfo() = default;

forte::arch::CSemaphore& CLocalMethodInfo::getResultReady() {
  return mResultIsReady;
}
