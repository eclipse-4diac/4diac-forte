/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/
#include "opcua_handler_abstract.h"
#include <basecommfb.h>
#include <forte_printer.h>
#include <devlog.h>
#include <parameterParser.h>
#include "opcua_layer.h"

const char *LogLevelNames[6] = { "trace", "debug", "info", "warning", "error", "fatal" };
const char *LogCategoryNames[7] = { "network", "channel", "session", "server", "client", "userland" };
//the following didn'work for shared library
//extern const char *LogLevelNames[6];
//extern const char *LogCategoryNames[7];

const char* const COPC_UA_HandlerAbstract::mActionNames[] = { "READ", "WRITE", "CREATE_METHOD", "CALL_METHOD", "SUBSCRIBE", "CREATE_OBJECT",
  "DELETE_OBJECT" };

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
const UA_Logger COPC_UA_HandlerAbstract::UA_Forte_logger = {UA_Log_Forte, 0, UA_Log_Forte_clear};
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

COPC_UA_HandlerAbstract::COPC_UA_HandlerAbstract(CDeviceExecution& paDeviceExecution) :
    CExternalEventHandler(paDeviceExecution) {
}

COPC_UA_HandlerAbstract::~COPC_UA_HandlerAbstract() {
  //do nothing
}

void COPC_UA_HandlerAbstract::setPriority(int) {
  //currently we are doing nothing here.
}

int COPC_UA_HandlerAbstract::getPriority() const {
  //the same as for setPriority
  return 0;
}

UA_Logger COPC_UA_HandlerAbstract::getLogger() {
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  return UA_Forte_logger;
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
  return UA_Log_Forte;
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
}

void COPC_UA_HandlerAbstract::UA_Log_Forte(
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    void*,
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_LogLevel paLevel, UA_LogCategory paCategory, const char *paMsg, va_list paArgs) {

  char tmpStr[mMaxLogLenght];
  forte_snprintf(tmpStr, mMaxLogLenght, "[OPC UA LOGGER] %s/%s\t", LogLevelNames[paLevel], LogCategoryNames[paCategory]);
  char *start = &tmpStr[strlen(tmpStr)];

  forte_vsnprintf(start, mMaxLogLenght, paMsg, paArgs);

  size_t len = strlen(tmpStr);
  tmpStr[len] = '\n';
  tmpStr[len + 1] = '\0';

  switch(paLevel){
    case UA_LOGLEVEL_TRACE:
    case UA_LOGLEVEL_DEBUG:
      DEVLOG_DEBUG(tmpStr);
      break;
    case UA_LOGLEVEL_INFO:
      DEVLOG_INFO(tmpStr);
      break;
    case UA_LOGLEVEL_WARNING:
      DEVLOG_WARNING(tmpStr);
      break;
    case UA_LOGLEVEL_ERROR:
    case UA_LOGLEVEL_FATAL:
      DEVLOG_ERROR(tmpStr);
      break;
  }
}

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
void COPC_UA_HandlerAbstract::UA_Log_Forte_clear(void *logContext) {
  //do nothing
}
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

COPC_UA_HandlerAbstract::CActionInfo* COPC_UA_HandlerAbstract::getInfoFromParams(const char* paParams, COPC_UA_Layer* paLayer) {
  COPC_UA_HandlerAbstract::CActionInfo* retVal = COPC_UA_Params::getInfoFromParams(paParams, paLayer);

  if(retVal && (!checkNodePairInfo(*retVal) || !checkAction(*retVal))) {
    delete retVal;
    retVal = 0;
  }

  return retVal;
}

COPC_UA_HandlerAbstract::CActionInfo* COPC_UA_HandlerAbstract::COPC_UA_Params::getInfoFromParams(const char* paParams, COPC_UA_Layer* paLayer) {
  COPC_UA_HandlerAbstract::CActionInfo* retVal = 0;
  CParameterParser mainParser(paParams, ';'); // ACTION;[ENDPOINT#];BROWSENAME,NODEID;[BROSWENAME,NODEID] //255 maximum browsename,nodeid
  size_t amountOfParameters = mainParser.parseParameters();

  if(2 > amountOfParameters) { //at least two are needed
    DEVLOG_ERROR("[OPC UA PARAMS]: Parameters %s should have at least two parts, separated by a semicolon\n", paParams);
  } else {
    COPC_UA_HandlerAbstract::UA_ActionType action = getActionFromParams(mainParser[0]);
    if(eActionUnknown != action) {

      if(eCreateMethod == action) {
        retVal = new COPC_UA_HandlerAbstract::CLocalMethodInfo(paLayer);
      } else {
        retVal = new COPC_UA_HandlerAbstract::CActionInfo(paLayer);
      }

      retVal->mAction = action;

      unsigned int startOfpairs = 1;
      if(0 != strchr(mainParser[1], '#')) { //Endpoint is present
        startOfpairs++;
        retVal->mEndpoint = mainParser[1];
        retVal->mEndpoint.assign(retVal->mEndpoint.getValue(), static_cast<TForteUInt16>(retVal->mEndpoint.length() - 1)); //remove #
      }

      bool somethingFailed = false;
      for(size_t i = startOfpairs; i < amountOfParameters; i++) {
        if(!handlePair(mainParser[i], retVal->mNodePair)) {
          somethingFailed = true;
          break;
        }
      }

      if(somethingFailed) {
        delete retVal;
        retVal = 0;
      }
    }
  }

  return retVal;
}

COPC_UA_HandlerAbstract::UA_ActionType COPC_UA_HandlerAbstract::COPC_UA_Params::getActionFromParams(const char* paActionString) {
  COPC_UA_HandlerAbstract::UA_ActionType action = eActionUnknown;
  for(size_t i = 0; i < eActionUnknown; i++) {
    if(0 == strcmp(paActionString, COPC_UA_HandlerAbstract::mActionNames[i])) {
      action = static_cast<UA_ActionType>(i);
      break;
    }
  }

  if(eActionUnknown == action) {
    DEVLOG_ERROR("[OPC UA PARAMS]: The action %s is unrecognized\n", paActionString);
  }
  return action;
}

bool COPC_UA_HandlerAbstract::COPC_UA_Params::handlePair(const char * paPair, CSinglyLinkedList<CNodePairInfo*>& paResult) {
  bool retVal = false;
  CParameterParser pairParser(paPair, ',');
  CIEC_STRING browsePathResult;
  UA_NodeId* nodeIdResult = 0;
  size_t noOfParameters = pairParser.parseParameters();
  if(1 == noOfParameters) { //no NodeId was provided
    browsePathResult = paPair;
    retVal = true;
  } else if(2 == noOfParameters) {
    browsePathResult = pairParser[0];
    nodeIdResult = parseNodeId(pairParser[1]);
    retVal = nodeIdResult;
  } else {
    DEVLOG_ERROR("[OPC UA PARAMS]: The pair %s doesn't have the proper format BROWSENAME,NODEID\n", paPair);
  }

  if(retVal) {
    paResult.pushBack(new CNodePairInfo(nodeIdResult, browsePathResult.getValue()));
  }

  return retVal;
}

UA_NodeId* COPC_UA_HandlerAbstract::COPC_UA_Params::parseNodeId(const char* paNodeIdString) {
  bool somethingFailed = false;
  UA_NodeId* resultNodeId = UA_NodeId_new();
  UA_NodeId_init(resultNodeId); //will set to default namespace 0
  unsigned int identifierPosition = 0;

  CParameterParser mainParser(paNodeIdString, ':'); //namespace and identifier should be divided by a colon. If first parameter is omitted, namespace 0 is assumed
  size_t numberOfParameters = mainParser.parseParameters();

  switch(numberOfParameters){
    case 2:
      identifierPosition++;
      if(!parseNamespace(mainParser[0], *resultNodeId)) {
        somethingFailed = true;
        break;
      }
      // fall through
    case 1: //NOSONAR
      if(!parseIdentifier(mainParser[identifierPosition], *resultNodeId)) {
        somethingFailed = true;
      }
      break;
    default:
      DEVLOG_ERROR("[OPC UA PARAMS]: NodeId %s has wrong number of colon\n", paNodeIdString);
      somethingFailed = true;
      break;
  }

  if(somethingFailed) {
    DEVLOG_ERROR(
      "[OPC UA PARAMS]: Parsing the NodeId %s failed. The format should follow the notation '<namespaceIndex>:<identifiertype>=<identifier>'. if the part before the : is omitted, namespace 0 is assumed\n",
      paNodeIdString);
    UA_NodeId_delete(resultNodeId);
    resultNodeId = 0;
  }

  return resultNodeId;
}

bool COPC_UA_HandlerAbstract::COPC_UA_Params::parseNamespace(const char* paNamespace, UA_NodeId& paResult) {
  paResult.namespaceIndex = static_cast<UA_UInt16>(forte::core::util::strtoul(paNamespace, 0, 10)); //TODO: should we check for return value here?
  return true;
}

bool COPC_UA_HandlerAbstract::COPC_UA_Params::parseIdentifier(const char* paIdentifier, UA_NodeId& paResult) {
  CParameterParser identifierParser(paIdentifier, '='); //<identifiertype>=<identifier>
  if(2 == identifierParser.parseParameters()) {
    if(0 == strcmp(identifierParser[0], "i")) { //numeric
      paResult.identifierType = UA_NODEIDTYPE_NUMERIC;
      paResult.identifier.numeric = static_cast<UA_UInt32>(forte::core::util::strtoul(identifierParser[1], 0, 10)); //TODO: should we check for return value here?
    } else if(0 == strcmp(identifierParser[0], "s")) { //string
      paResult.identifierType = UA_NODEIDTYPE_STRING;
      paResult.identifier.string = UA_String_fromChars(identifierParser[1]);
    } else if(0 == strcmp(identifierParser[0], "g")) { //GUID
      DEVLOG_ERROR("[OPC UA PARAMS]:GUID type is not yet implemented\n");
      return false;
    } else if(0 == strcmp(identifierParser[0], "b")) { //byteString
      paResult.identifierType = UA_NODEIDTYPE_BYTESTRING;
      paResult.identifier.byteString = UA_BYTESTRING_ALLOC(identifierParser[1]);
    } else {
      DEVLOG_ERROR("[OPC UA PARAMS]: The identifier type %s wasn't recognized among the possible values [i, s, g, b]\n", identifierParser[0]);
      return false;
    }
  } else {
    DEVLOG_ERROR("[OPC UA PARAMS]: Parsing the identifier %s failed. The format should follow the notation '<identifiertype>=<identifier>'\n", paIdentifier);
    return false;
  }
  return true;
}

bool COPC_UA_HandlerAbstract::checkNodePairInfo(COPC_UA_HandlerAbstract::CActionInfo& paResult) {
  bool retVal = true;
  for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CNodePairInfo*>::Iterator it = paResult.getNodePairInfo().begin(); it != paResult.getNodePairInfo().end();
      ++it) {
    if("" == paResult.getEndpoint()) { //local
      if("" == (*it)->mBrowsePath) { //browsePath is  mandatory in local
        DEVLOG_ERROR("[OPC UA PARAMS]: BrowsePath is mandatory for local actions. Check FB %s\n", paResult.getLayer()->getCommFB()->getInstanceName());
        retVal = false;
        break;
      }
    } else { //remote
      if("" == (*it)->mBrowsePath && 0 == (*it)->mNodeId) { //browsePath AND/OR NodeId must be given. If both are empty there's a problem
        DEVLOG_ERROR("[OPC UA PARAMS]: BrowsePath and NodeId are empty in FB %s\n", paResult.getLayer()->getCommFB()->getInstanceName());
        retVal = false;
        break;
      }
    }
  }

  return retVal;
}

bool COPC_UA_HandlerAbstract::checkAction(COPC_UA_HandlerAbstract::CActionInfo& paResult) {

  forte::com_infra::EComServiceType fbType = paResult.getLayer()->getCommFB()->getComServiceType();
  unsigned int noOfRDs = paResult.getLayer()->getCommFB()->getNumRD();
  unsigned int noOfSDs = paResult.getLayer()->getCommFB()->getNumSD();

  bool retVal = false;

  switch(paResult.getAction()){
    case eRead:
      if("" == paResult.getEndpoint()) {
        if(forte::com_infra::EComServiceType::e_Subscriber == fbType && noOfRDs == paResult.getNoOfNodePairs()) {
          retVal = true;
        } else {
          DEVLOG_ERROR(
            "[OPC UA PARAMS]: In FB %s: Local %s action is only allowed using a Subscriber FB and the amount of BrowseName,NodeId pairs should match the number of RDs\n",
            paResult.getLayer()->getCommFB()->getInstanceName(),
            COPC_UA_HandlerAbstract::mActionNames[eRead]);
        }
      } else {
        if(forte::com_infra::EComServiceType::e_Client == fbType && noOfRDs == paResult.getNoOfNodePairs() && 0 == noOfSDs) {
          retVal = true;
        } else {
          DEVLOG_ERROR(
            "[OPC UA PARAMS]: Remote %s action is only allowed using a Client FB, the amount of BrowseName,NodeId pairs should match the number of RDs, and must have no SDs\n",
            paResult.getLayer()->getCommFB()->getInstanceName(),
            COPC_UA_HandlerAbstract::mActionNames[eRead]);
        }
      }
      break;
    case eWrite:
      if(forte::com_infra::EComServiceType::e_Publisher == fbType && noOfSDs == paResult.getNoOfNodePairs()) {
        retVal = true;
      } else {
        DEVLOG_ERROR(
          "[OPC UA P[OPC UA PARAMS]: In FB %s: In FB %s:  is only allowed using a Publisher FB and the amount of BrowseName,NodeId pairs should match the number of SDs\n",
          paResult.getLayer()->getCommFB()->getInstanceName(),
          COPC_UA_HandlerAbstract::mActionNames[eWrite]);
      }
      break;
    case eCreateMethod:
      if(forte::com_infra::EComServiceType::e_Server == fbType && 1 == paResult.getNoOfNodePairs()) {
        retVal = true;
      } else {
        DEVLOG_ERROR(
          "[OPC UA PARAMS]: In FB %s: %s action is only allowed using a Server FB, the amount of BrowseName,NodeId pairs should be 1\n",
          paResult.getLayer()->getCommFB()->getInstanceName(),
          COPC_UA_HandlerAbstract::mActionNames[eCreateMethod]);
      }
      break;
    case eCallMethod:
      if(forte::com_infra::EComServiceType::e_Client == fbType && 1 == paResult.getNoOfNodePairs()
        && "" != (*(paResult.getNodePairInfo().begin()))->mBrowsePath) {
        retVal = true;
      } else {
        DEVLOG_ERROR(
          "[OPC UA PARAMS]: In FB %s: %s action is only allowed using a Client FB, the amount of BrowseName,NodeId pairs should be 1, and the browsepath shouldn't be empty\n",
          paResult.getLayer()->getCommFB()->getInstanceName(),
          COPC_UA_HandlerAbstract::mActionNames[eCallMethod]);
      }
      break;
    case eSubscribe:
      if(forte::com_infra::EComServiceType::e_Subscriber == fbType && noOfRDs == paResult.getNoOfNodePairs()) {
        retVal = true;
      } else {
        DEVLOG_ERROR(
          "[OPC UA PARAMS]: In FB %s: %s action is only allowed using a Subscribe FB, the amount of BrowseName,NodeId pairs should match the number of RDs\n",
          paResult.getLayer()->getCommFB()->getInstanceName(),
          COPC_UA_HandlerAbstract::mActionNames[eSubscribe]);
      }
      break;
    case eCreateObject:
      break;
    case eDeleteObject:
      break;
    default:
      DEVLOG_ERROR("[OPC UA PARAMS]: Unknown action %d", paResult.getAction());
  }

  return retVal;
}
