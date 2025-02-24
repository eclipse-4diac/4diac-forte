/*******************************************************************************
 * Copyright (c) 2016, 2023 fortiss GmbH, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stefan Profanter
 *      - initial implementation
 *    Martin Melik Merkumians - Change CIEC_STRING to std::string
 *******************************************************************************/

#include "opcua_helper.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "opcua_helper_gen.cpp"
#endif
#include "core/util/parameterParser.h"
#include "arch/devlog.h"
#include "convert_functions.h"
#include "forte_date.h"
#include "forte_struct.h"
#include "forte_array.h"

static const CIEC_DATE::TValueType internalToOPCUAEpochDifferenceInNanoseconds = 11644473600000000LL;

template<typename T_FORTE_TYPE, typename T_OPCUA_TYPE>
size_t convertFromIECToOPCUASpecific(const CIEC_ANY &paSrc, void *paDest) {
  *static_cast<T_OPCUA_TYPE *>(paDest) = static_cast<typename T_FORTE_TYPE::TValueType>(static_cast<const T_FORTE_TYPE &>(paSrc));
  return sizeof(T_OPCUA_TYPE);
}

template<typename T_FORTE_TYPE, typename T_OPCUA_TYPE>
size_t convertFromOPCUAToIECSpecific(const void *paSrc, CIEC_ANY &paDest) {
  static_cast<T_FORTE_TYPE&>(paDest) = T_FORTE_TYPE(*static_cast<const T_OPCUA_TYPE*>(paSrc));
  return sizeof(T_OPCUA_TYPE);
}

template<>
size_t convertFromIECToOPCUASpecific<CIEC_DATE, UA_DateTime>(const CIEC_ANY &paSrc, void *paDest) {
  const CIEC_DATE::TValueType timeInNsFromInternalEpoch = static_cast<CIEC_DATE::TValueType>(static_cast<const CIEC_DATE &>(paSrc));

  *static_cast<UA_DateTime *>(paDest) = (timeInNsFromInternalEpoch + internalToOPCUAEpochDifferenceInNanoseconds) / 100LL;
  return sizeof(UA_DateTime);
}

template<>
size_t convertFromOPCUAToIECSpecific<CIEC_DATE, UA_DateTime>(const void *paSrc, CIEC_ANY &paDest) {
  const TForteInt64 cNanosecondsPerDay = 24LL * 60LL * 60LL * 1000000000LL;
  TForteInt64 opcUaTime = *reinterpret_cast<const TForteInt64 *>(static_cast<const UA_DateTime *>(paSrc)) - (internalToOPCUAEpochDifferenceInNanoseconds / 100LL);
  opcUaTime = opcUaTime >= 0 ? opcUaTime : 0; // Workaround: CIEC_DATE internal representation is unsigned from Epoch, so dates before Epoch are not representable
  opcUaTime /= cNanosecondsPerDay; // Timestap divided by nanoseconds per day -> floored result will give date only
  opcUaTime *= cNanosecondsPerDay; // Multiply again to get correct timestamp value
  static_cast<CIEC_DATE &>(paDest) = CIEC_DATE(opcUaTime);
  return sizeof(UA_DateTime);
}

template<>
size_t convertFromIECToOPCUASpecific<CIEC_TIME_OF_DAY, UA_DateTime>(const CIEC_ANY&, void*) {
  //TODO how convert TOD to UA_DateTime?
  return 0;
}

template<>
size_t convertFromOPCUAToIECSpecific<CIEC_TIME_OF_DAY, UA_DateTime>(const void*, CIEC_ANY&) {
  //TODO how convert UA_DateTime to TOD?
  return 0;
}

template<>
size_t convertFromIECToOPCUASpecific<CIEC_STRING, UA_String>(const CIEC_ANY &paSrc, void *paDest) {
  *static_cast<UA_String*>(paDest) = UA_String_fromChars(static_cast<const CIEC_STRING&>(paSrc).getStorage().c_str());
  return sizeof(UA_String);
}

template<>
size_t convertFromOPCUAToIECSpecific<CIEC_STRING, UA_String>(const void *paSrc, CIEC_ANY &paDest) {
  const UA_String *str = static_cast<const UA_String*>(paSrc);
  static_cast<CIEC_STRING&>(paDest).assign(reinterpret_cast<const char*>(str->data), static_cast<TForteUInt16>(str->length));
  return sizeof(UA_String);
}

template<>
size_t convertFromIECToOPCUASpecific<CIEC_WSTRING, UA_String>(const CIEC_ANY &paSrc, void *paDest) {
  CIEC_STRING str = func_WSTRING_TO_STRING(static_cast<const CIEC_WSTRING&>(paSrc));
  return convertFromIECToOPCUASpecific<CIEC_STRING, UA_String>(str, paDest);
}

template<>
size_t convertFromOPCUAToIECSpecific<CIEC_WSTRING, UA_String>(const void *paSrc, CIEC_ANY &paDest) {
  const UA_String *str = static_cast<const UA_String*>(paSrc);
  static_cast<CIEC_WSTRING&>(paDest).assign(reinterpret_cast<const char*>(str->data), static_cast<TForteUInt16>(str->length));
  return sizeof(UA_String);
}

const COPC_UA_Helper::UA_TypeConvert COPC_UA_Helper::scmMapForteTypeIdToOpcUa[] = {
    UA_TypeConvert(nullptr, nullptr, nullptr), // dummy for e_ANY
    UA_TypeConvert(&UA_TYPES[UA_TYPES_BOOLEAN], &convertFromIECToOPCUASpecific<CIEC_BOOL, UA_Boolean>, &convertFromOPCUAToIECSpecific<CIEC_BOOL, UA_Boolean>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_SBYTE], &convertFromIECToOPCUASpecific<CIEC_SINT, UA_SByte>, &convertFromOPCUAToIECSpecific<CIEC_SINT, UA_SByte>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_INT16], &convertFromIECToOPCUASpecific<CIEC_INT, UA_Int16>, &convertFromOPCUAToIECSpecific<CIEC_INT, UA_Int16>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_INT32], &convertFromIECToOPCUASpecific<CIEC_DINT, UA_Int32>, &convertFromOPCUAToIECSpecific<CIEC_DINT, UA_Int32>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_INT64], &convertFromIECToOPCUASpecific<CIEC_LINT, UA_Int64>, &convertFromOPCUAToIECSpecific<CIEC_LINT, UA_Int64>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_BYTE], &convertFromIECToOPCUASpecific<CIEC_USINT, UA_Byte>, &convertFromOPCUAToIECSpecific<CIEC_USINT, UA_Byte>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT16], &convertFromIECToOPCUASpecific<CIEC_UINT, UA_UInt16>, &convertFromOPCUAToIECSpecific<CIEC_UINT, UA_UInt16>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT32], &convertFromIECToOPCUASpecific<CIEC_UDINT, UA_UInt32>, &convertFromOPCUAToIECSpecific<CIEC_UDINT, UA_UInt32>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT64], &convertFromIECToOPCUASpecific<CIEC_ULINT, UA_UInt64>, &convertFromOPCUAToIECSpecific<CIEC_ULINT, UA_UInt64>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_BYTE], &convertFromIECToOPCUASpecific<CIEC_BYTE, UA_Byte>, &convertFromOPCUAToIECSpecific<CIEC_ULINT, UA_Byte>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT16], &convertFromIECToOPCUASpecific<CIEC_WORD, UA_UInt16>, &convertFromOPCUAToIECSpecific<CIEC_WORD, UA_UInt16>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT32], &convertFromIECToOPCUASpecific<CIEC_DWORD, UA_UInt32>, &convertFromOPCUAToIECSpecific<CIEC_DWORD, UA_UInt32>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT64], &convertFromIECToOPCUASpecific<CIEC_LWORD, UA_UInt64>, &convertFromOPCUAToIECSpecific<CIEC_LWORD, UA_UInt64>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_DATE, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_DATE, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_TIME_OF_DAY, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_TIME_OF_DAY, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_DATE_AND_TIME, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_DATE_AND_TIME, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_INT64], &convertFromIECToOPCUASpecific<CIEC_TIME, UA_Int64>, &convertFromOPCUAToIECSpecific<CIEC_TIME, UA_Int64>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_BYTE], &convertFromIECToOPCUASpecific<CIEC_CHAR, UA_Byte>, &convertFromOPCUAToIECSpecific<CIEC_CHAR, UA_Byte>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_UINT16], &convertFromIECToOPCUASpecific<CIEC_WCHAR, UA_UInt16>, &convertFromOPCUAToIECSpecific<CIEC_WCHAR, UA_UInt16>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_LDATE, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_LDATE, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_LTIME_OF_DAY, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_LTIME_OF_DAY, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DATETIME], &convertFromIECToOPCUASpecific<CIEC_LDATE_AND_TIME, UA_DateTime>, &convertFromOPCUAToIECSpecific<CIEC_LDATE_AND_TIME, UA_DateTime>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_INT64], &convertFromIECToOPCUASpecific<CIEC_LTIME, UA_Int64>, &convertFromOPCUAToIECSpecific<CIEC_LTIME, UA_Int64>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_FLOAT], &convertFromIECToOPCUASpecific<CIEC_REAL, UA_Float>, &convertFromOPCUAToIECSpecific<CIEC_REAL, UA_Float>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_DOUBLE], &convertFromIECToOPCUASpecific<CIEC_LREAL, UA_Double>, &convertFromOPCUAToIECSpecific<CIEC_LREAL, UA_Double>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_STRING], &convertFromIECToOPCUASpecific<CIEC_STRING, UA_String>, &convertFromOPCUAToIECSpecific<CIEC_STRING, UA_String>),
    UA_TypeConvert(&UA_TYPES[UA_TYPES_STRING], &convertFromIECToOPCUASpecific<CIEC_WSTRING, UA_String>, &convertFromOPCUAToIECSpecific<CIEC_WSTRING, UA_String>),
};

const UA_DataType* COPC_UA_Helper::getOPCUATypeFromAny(const CIEC_ANY &paAnyType) {
  CIEC_ANY::EDataTypeID typeId = paAnyType.getDataTypeID();
  if(typeId == CIEC_ANY::e_ANY) {
    return getOPCUATypeFromAny(paAnyType.unwrap());
  } else if(typeId <= CIEC_ANY::e_WSTRING) { //basic type
    return scmMapForteTypeIdToOpcUa[typeId].mType;
  } else if(CIEC_ANY::e_ARRAY == typeId) {
    return getOPCUATypeFromAny(static_cast<const CIEC_ARRAY&>(paAnyType)[0]);
  } else {
    return getExternalOPCUATypeFromAny(paAnyType);
  }
}

size_t COPC_UA_Helper::convertToOPCUAType(const CIEC_ANY &paSrcAny, void *paDest) {
  size_t retVal = 0;

  CIEC_ANY::EDataTypeID typeId = paSrcAny.getDataTypeID();
  if(typeId == CIEC_ANY::e_ANY) {
    return convertToOPCUAType(paSrcAny.unwrap(), paDest);
  } else if(typeId <= CIEC_ANY::e_WSTRING) { //basic type
    retVal = scmMapForteTypeIdToOpcUa[typeId].mToOPCUA(paSrcAny, paDest);
  } else if(CIEC_ANY::e_ARRAY == typeId) {
    for(size_t i = 0; i < static_cast<const CIEC_ARRAY&>(paSrcAny).size(); i++) {
      retVal += convertToOPCUAType(static_cast<const CIEC_ARRAY&>(paSrcAny)[static_cast<TForteUInt16>(i)], static_cast<char*>(paDest) + retVal);
    }
  } else if(CIEC_ANY::e_STRUCT == typeId) {
    return convertStructToOPCUAType(static_cast<const CIEC_STRUCT&>(paSrcAny), paDest);
  }
  return retVal;
}

size_t COPC_UA_Helper::convertStructToOPCUAType(const CIEC_STRUCT &paSrcStruct, void *paDest){
  size_t retVal = 0;
  UA_DataTypeMember const * const uaMemberTypes = getOPCUATypeFromAny(paSrcStruct)->members;

  for(size_t i = 0; i < paSrcStruct.getStructSize(); i++){
    if(uaMemberTypes != nullptr){
         retVal += uaMemberTypes[i].padding;
    }
    retVal += convertToOPCUAType(*paSrcStruct.getMember(i), static_cast<char*>(paDest) + retVal);
  }
  return retVal;
}


size_t COPC_UA_Helper::convertFromOPCUAType(void const * const paSrc, CIEC_ANY &paDestAny) {
  size_t retVal = 0;

  CIEC_ANY::EDataTypeID typeId = paDestAny.getDataTypeID();
  if(typeId == CIEC_ANY::e_ANY) {
    return convertFromOPCUAType(paSrc, paDestAny.unwrap());
  } else if(typeId <= CIEC_ANY::e_WSTRING) { //basic type
    retVal = scmMapForteTypeIdToOpcUa[typeId].mFromOPCUA(paSrc, paDestAny);
  } else if(CIEC_ANY::e_ARRAY == typeId) {
    for(size_t i = 0; i < static_cast<CIEC_ARRAY&>(paDestAny).size(); i++) {
      retVal += convertFromOPCUAType(static_cast<const char*>(paSrc) + retVal, static_cast<CIEC_ARRAY&>(paDestAny)[static_cast<TForteUInt16>(i)]);
    }
  } else if(CIEC_ANY::e_STRUCT == typeId) {
    return convertStructFromOPCUAType(paSrc, static_cast<CIEC_STRUCT&>(paDestAny));
  }
  return retVal;
}

size_t COPC_UA_Helper::convertStructFromOPCUAType(void const * const paSrc, CIEC_STRUCT &paDestStruct){
  size_t retVal = 0;
  UA_DataTypeMember const * const uaMemberTypes = getOPCUATypeFromAny(paDestStruct)->members;

  for(size_t i = 0; i < paDestStruct.getStructSize(); i++){
    if(uaMemberTypes != nullptr){
      retVal += uaMemberTypes[i].padding;
    }
    retVal += convertFromOPCUAType(static_cast<const char*>(paSrc) + retVal, *paDestStruct.getMember(i));
  }

  return retVal;
}

void COPC_UA_Helper::fillVariant(UA_Variant &paVariant, const CIEC_ANY &paDataSource) {
  UA_Variant_init(&paVariant);
  paVariant.type = COPC_UA_Helper::getOPCUATypeFromAny(paDataSource);
  void *varValue = UA_new(paVariant.type);
  UA_init(varValue, paVariant.type);
  COPC_UA_Helper::convertToOPCUAType(paDataSource, varValue);
  UA_Variant_setScalarCopy(&paVariant, varValue, paVariant.type);
  paVariant.storageType = UA_VARIANT_DATA;
  UA_delete(varValue, paVariant.type);
}

bool COPC_UA_Helper::isBrowsePathValid(const std::string &paBrowsepath) {
  bool retVal = false;

  if(!paBrowsepath.empty() && '/' == paBrowsepath.front()) {
    size_t lenghtOfBrowsename = paBrowsepath.length();
    if('/' == paBrowsepath.back()) { //check if last character is '/'
      lenghtOfBrowsename--;
    }

    if(1 < lenghtOfBrowsename) {
      retVal = true;
    } else {
      DEVLOG_ERROR("[OPC UA HELPER]: Browsepath %s doesn't provide a valid nodename\n", paBrowsepath.c_str());
    }
  } else {
    DEVLOG_ERROR("[OPC UA HELPER]: Browsepath %s should start with a slash\n", paBrowsepath.c_str());
  }

  return retVal;
}

UA_StatusCode COPC_UA_Helper::getRemoteNodeForPath(UA_Client &paClient, const char *paNodePathConst, UA_NodeId **paParentNodeId, UA_NodeId **paFoundNodeId) {
  *paFoundNodeId = nullptr;
  UA_BrowsePath *browsePaths = nullptr;
  size_t folderCnt = 0;
  UA_StatusCode retVal = prepareBrowseArgument(paNodePathConst, &browsePaths, &folderCnt);
  if(UA_STATUSCODE_GOOD == retVal) {

    UA_TranslateBrowsePathsToNodeIdsRequest request;
    UA_TranslateBrowsePathsToNodeIdsResponse response;

    UA_TranslateBrowsePathsToNodeIdsRequest_init(&request);

    request.browsePaths = browsePaths;
    request.browsePathsSize = folderCnt * 2;

    response = UA_Client_Service_translateBrowsePathsToNodeIds(&paClient, request);

    retVal = response.responseHeader.serviceResult;
    if(UA_STATUSCODE_GOOD == retVal) {
      if(response.resultsSize == folderCnt * 2) {
        int foundFolderOffset = getFolderOffset(response.results, folderCnt);

        if(foundFolderOffset >= 0) {
          // all nodes exist, so just copy the node ids
          copyNodeIds(response.results, folderCnt, foundFolderOffset, paParentNodeId, paFoundNodeId);
        } else {
          DEVLOG_ERROR("[OPC UA HELPER]: Could not translate browse paths for '%s' to node IDs. Not all nodes exist\n", paNodePathConst);
          retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
        }
      } else {
        DEVLOG_ERROR("[OPC UA HELPER]: Could not translate browse paths for '%s' to node IDs. resultSize (%d) != expected count (%d)\n", paNodePathConst,
          response.resultsSize, folderCnt);
        retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
      }
    } else {
      DEVLOG_ERROR("[OPC UA HELPER]: Could not translate browse paths for '%s' to node IDs. Error: %s\n", paNodePathConst, UA_StatusCode_name(retVal));
    }

    //browsePaths is deleted inside
    UA_TranslateBrowsePathsToNodeIdsRequest_clear(&request);
    UA_TranslateBrowsePathsToNodeIdsResponse_clear(&response);
  }

  return retVal;
}

UA_StatusCode COPC_UA_Helper::prepareBrowseArgument(const char *paNodePathConst, UA_BrowsePath **paBrowsePaths, size_t *paFolderCount) {

  if(!isBrowsePathValid(paNodePathConst)) {
    return UA_STATUSCODE_BADINVALIDARGUMENT;
  }

  auto startNodePath = paNodePathConst;
  auto nodePathLength = strlen(paNodePathConst);
  auto endNodePath = startNodePath + (nodePathLength - 1);

  if('/' == *paNodePathConst) { // remove starting slash
    ++startNodePath;
    --nodePathLength;
  }

  if('/' == *endNodePath) { // remove tailing slash
    --nodePathLength;
  }

  std::string copyOfOriginal(startNodePath, nodePathLength);

  CParameterParser folderParser(copyOfOriginal.c_str(), '/');

  (*paFolderCount) = folderParser.parseParameters();

  UA_NodeId startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);

  // for every folder (which is a BrowsePath) we want to get the node id
  *paBrowsePaths = static_cast<UA_BrowsePath*>(UA_Array_new((*paFolderCount) * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]));
  UA_BrowsePath *browsePaths = *paBrowsePaths;

  for(size_t i = 0; i < (*paFolderCount); i++) {
    const char *tok = folderParser[i];
    UA_BrowsePath_init(&browsePaths[i]);
    browsePaths[i].startingNode = startingNode;
    browsePaths[i].relativePath.elementsSize = i + 1;

    browsePaths[i].relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(browsePaths[i].relativePath.elementsSize,
      &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));

    for(size_t j = 0; j <= i; j++) {
      if(j < i) {
        // just copy from before
        UA_RelativePathElement_copy(&browsePaths[i - 1].relativePath.elements[j], &browsePaths[i].relativePath.elements[j]);
        continue;
      }
      // the last element is a new one
      UA_RelativePathElement_init(&browsePaths[i].relativePath.elements[j]);
      browsePaths[i].relativePath.elements[j].isInverse = UA_FALSE;

      if(!getBrowsenameFromNodeName(tok, i ? 1 : 0, browsePaths[i].relativePath.elements[j].targetName)) { //use the fix 0 only for the first one and then take it from the server
        UA_Array_delete(browsePaths, *paFolderCount * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]);
        return UA_STATUSCODE_BADINVALIDARGUMENT;
      }
    }
  }

  // same browse paths, but inverse reference, to check both directions
  for(size_t i = 0; i < (*paFolderCount); i++) {
    UA_BrowsePath_copy(&browsePaths[i], &browsePaths[(*paFolderCount) + i]);
    for(unsigned int j = 0; j < browsePaths[(*paFolderCount) + i].relativePath.elementsSize; j++) {
      browsePaths[(*paFolderCount) + i].relativePath.elements[j].isInverse = !browsePaths[(*paFolderCount) + i].relativePath.elements[j].isInverse;
    }
  }
  return UA_STATUSCODE_GOOD;
}

UA_StatusCode COPC_UA_Helper::releaseBrowseArgument(UA_BrowsePath &paBrowsePaths, size_t paPathLength) {
  UA_Array_delete(&paBrowsePaths, paPathLength * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]);
  return UA_STATUSCODE_GOOD;
}

bool COPC_UA_Helper::getBrowsenameFromNodeName(const char *paNodeName, UA_UInt16 paDefaultNamespace, UA_QualifiedName &paBrowseName) {
  bool retVal = true;

  UA_UInt16 browsenameNamespace = paDefaultNamespace;
  std::string targetName;
  CParameterParser browseNameParser(paNodeName, ':');
  size_t parsingResult = browseNameParser.parseParameters();
  if(scmMaxNoOfParametersInBrowseName == parsingResult) {
    browsenameNamespace = static_cast<UA_UInt16>(forte::core::util::strtol(browseNameParser[0], nullptr, 10));
    targetName = std::string(browseNameParser[1]);
  } else if(1 == parsingResult) {
    targetName = std::string(browseNameParser[0]);
  } else {
    DEVLOG_ERROR("[OPC UA HELPER]: Error by parsing FB browse path %s\n", paNodeName);
    retVal = false;
  }

  if(retVal) {
    paBrowseName = UA_QUALIFIEDNAME_ALLOC(browsenameNamespace, targetName.c_str());
  }
  return retVal;
}

int COPC_UA_Helper::getFolderOffset(const UA_BrowsePathResult *paBrowsePathsResults, size_t paFolderCnt) {
  int foundFolderOffset = -1;
  if(UA_STATUSCODE_GOOD == paBrowsePathsResults[paFolderCnt - 1].statusCode) {
    foundFolderOffset = 0;
  } else if(UA_STATUSCODE_GOOD == paBrowsePathsResults[paFolderCnt * 2 - 1].statusCode) {
    foundFolderOffset = static_cast<int>(paFolderCnt);
  }
  return foundFolderOffset;
}

void COPC_UA_Helper::copyNodeIds(const UA_BrowsePathResult *paBrowsePathsResults, size_t paFolderCnt, int paFoundFolderOffset, UA_NodeId **paParentNodeId,
    UA_NodeId **paFoundNodeId) {
  *paFoundNodeId = UA_NodeId_new();
  UA_NodeId_copy(&paBrowsePathsResults[paFoundFolderOffset + paFolderCnt - 1].targets[0].targetId.nodeId, *paFoundNodeId);
  if(paParentNodeId && paFolderCnt >= 2) {
    *paParentNodeId = UA_NodeId_new();
    UA_NodeId_copy(&paBrowsePathsResults[paFoundFolderOffset + paFolderCnt - 2].targets[0].targetId.nodeId, *paParentNodeId);
  } else if(paParentNodeId) {
    *paParentNodeId = UA_NodeId_new();
    **paParentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
  }
}

