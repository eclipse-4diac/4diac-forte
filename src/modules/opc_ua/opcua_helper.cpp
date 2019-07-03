/*******************************************************************************
  * Copyright (c) 2016 fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Stefan Profanter
  *      - initial implementation
  *******************************************************************************/

#include "opcua_helper.h"
#include "convert_functions.h"
#include "../../core/utils/parameterParser.h"
#include "../../arch/devlog.h"

#define UA_String_to_char_alloc(str, chars) { \
        chars = static_cast<char*>(forte_malloc(str->length+1)); \
        memcpy(chars, str->data, str->length); \
        chars[str->length] = 0; \
    }

template<CIEC_ANY::EDataTypeID T_FORTE_E, typename T_FORTE_T, typename T_C>
bool map_convert_get(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != T_FORTE_E)
    return false;
  T_C *dstType = static_cast<T_C *>(dst);
  *dstType = *(T_FORTE_T *) (src);
  return true;
};

template<CIEC_ANY::EDataTypeID T_FORTE_E, typename T_FORTE_T, typename T_C>
bool map_convert_set(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != T_FORTE_E)
    return false;
  T_FORTE_T *dstType = static_cast<T_FORTE_T *>(dst);
  *dstType = *(T_C *) (src);
  return true;
};

#define MAP_INSERT_CONVERT(T_FORTE_E, T_FORTE_T, T_UA, T_C) \
    { \
        T_UA, \
        &map_convert_get<T_FORTE_E, T_FORTE_T, T_C>, \
        &map_convert_set<T_FORTE_E, T_FORTE_T, T_C> \
    } \

#define MAP_INSERT_CONVERT_SPECIFIC(T_FORTE_T, T_UA) \
    { \
        T_UA, \
        &map_convert_get_##T_FORTE_T, \
        &map_convert_set_##T_FORTE_T \
    } \



bool map_convert_get_CIEC_DATE(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_DATE)
    return false;
  UA_DateTime *dstType = static_cast<UA_DateTime *>(dst);
  *dstType = DATE_TO_DT(*reinterpret_cast<const CIEC_DATE*>(src));
  return true;
}

bool map_convert_set_CIEC_DATE(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_DATE)
    return false;
  CIEC_DATE *dstType = static_cast<CIEC_DATE *>(dst);
  *dstType = DT_TO_DATE(*reinterpret_cast<const TForteUInt64*>(reinterpret_cast<const UA_DateTime*>(src)));
  return true;
}

bool map_convert_get_CIEC_TIME_OF_DAY(const CIEC_ANY *src, void *) {
  if (src->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
    return false;
  //TODO how convert TOD to DT?
  return false;
}

bool map_convert_set_CIEC_TIME_OF_DAY(const void *, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
    return false;
  //TODO how convert UA_DateTime to TOD?
  return false;
}


bool map_convert_get_CIEC_STRING(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_STRING)
    return false;
  UA_String *dstType = static_cast<UA_String *>(dst);
  *dstType = UA_String_fromChars(static_cast<const CIEC_STRING *>(src)->getValue());
  return true;
}

bool map_convert_set_CIEC_STRING(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_STRING)
    return false;
  CIEC_STRING *dstType = static_cast<CIEC_STRING *>(dst);

  const UA_String *str = static_cast<const UA_String *>(src);
  char *chars;
  UA_String_to_char_alloc(str, chars);
  dstType->fromString(chars);
  forte_free(chars);
  return true;
}

bool map_convert_get_CIEC_WSTRING(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_WSTRING)
    return false;
  COPC_UA_Helper::UA_TypeConvert conv = COPC_UA_Helper::mapForteTypeIdToOpcUa[CIEC_ANY::e_STRING];
  CIEC_STRING str = WSTRING_TO_STRING(*static_cast<const CIEC_WSTRING *>(src));
  return conv.get(&str, dst);
}

bool map_convert_set_CIEC_WSTRING(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_WSTRING)
    return false;
  CIEC_WSTRING *dstType = static_cast<CIEC_WSTRING *>(dst);

  const UA_String *str = static_cast<const UA_String *>(src);
  char *chars;
  UA_String_to_char_alloc(str, chars);
  dstType->fromString(chars);
  forte_free(chars);

  return true;
}

/**
 * ATTENTION:
 * This array has to match the enum CIEC_ANY::EDataTypeID.
 * Current maximum index is WSTRING
 */
const COPC_UA_Helper::UA_TypeConvert COPC_UA_Helper::mapForteTypeIdToOpcUa[CIEC_ANY::e_WSTRING + 1] = {
    { // dummy for e_ANY
      NULL,
      NULL,
      NULL
    },
    MAP_INSERT_CONVERT(CIEC_ANY::e_BOOL, CIEC_BOOL, &UA_TYPES[UA_TYPES_BOOLEAN], UA_Boolean),
    MAP_INSERT_CONVERT(CIEC_ANY::e_SINT, CIEC_SINT, &UA_TYPES[UA_TYPES_SBYTE], UA_SByte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_INT, CIEC_INT, &UA_TYPES[UA_TYPES_INT16], UA_Int16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DINT, CIEC_DINT, &UA_TYPES[UA_TYPES_INT32], UA_Int32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LINT, CIEC_LINT, &UA_TYPES[UA_TYPES_INT64], UA_Int64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_USINT, CIEC_USINT, &UA_TYPES[UA_TYPES_BYTE], UA_Byte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_UINT, CIEC_UINT, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_UDINT, CIEC_UDINT, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_ULINT, CIEC_ULINT, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_BYTE, CIEC_BYTE, &UA_TYPES[UA_TYPES_BYTE], UA_Byte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_WORD, CIEC_WORD, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DWORD, CIEC_DWORD, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LWORD, CIEC_LWORD, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_DATE, &UA_TYPES[UA_TYPES_DATETIME]),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_TIME_OF_DAY, &UA_TYPES[UA_TYPES_DATETIME]),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DATE_AND_TIME, CIEC_DATE_AND_TIME, &UA_TYPES[UA_TYPES_DATETIME], UA_DateTime),
    MAP_INSERT_CONVERT(CIEC_ANY::e_TIME, CIEC_TIME, &UA_TYPES[UA_TYPES_DOUBLE], UA_Int64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_REAL, CIEC_REAL, &UA_TYPES[UA_TYPES_FLOAT], UA_Float),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LREAL, CIEC_LREAL, &UA_TYPES[UA_TYPES_DOUBLE], UA_Double),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_STRING, &UA_TYPES[UA_TYPES_STRING]),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_WSTRING, &UA_TYPES[UA_TYPES_STRING])
};

bool COPC_UA_Helper::isTypeIdValid(CIEC_ANY::EDataTypeID paTypeId) {
  return paTypeId >= CIEC_ANY::e_BOOL && paTypeId <= CIEC_ANY::e_WSTRING;
}

UA_StatusCode COPC_UA_Helper::releaseBrowseArgument(UA_BrowsePath* paBrowsePaths, size_t paPathLength) {
  UA_Array_delete(paBrowsePaths, paPathLength * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]);
  return UA_STATUSCODE_GOOD;
}

bool COPC_UA_Helper::checkBrowsePath(const char *paBrowsepath) {
  bool retVal = false;

  if('/' == *paBrowsepath) {
    size_t lenghtOfBrowsename = strlen(paBrowsepath);
    if('/' == *(paBrowsepath + lenghtOfBrowsename - 1)) { //remove trailing slash
      lenghtOfBrowsename--;
    }

    if(1 < lenghtOfBrowsename) {
      retVal = true;
    } else {
      DEVLOG_ERROR("[OPC UA HELPER]: Browsepath %s doesn't provide  a valid nodename\n", paBrowsepath);
    }
  } else {
    DEVLOG_ERROR("[OPC UA HELPER]: Browsepath %s should start with a slash\n", paBrowsepath);
  }

  return retVal;
}

UA_StatusCode COPC_UA_Helper::prepareBrowseArgument(const char *paNodePathConst, UA_BrowsePath** paBrowsePaths, size_t* paFolderCount) {

  if(!checkBrowsePath(paNodePathConst)) {
    return UA_STATUSCODE_BADINVALIDARGUMENT;
  }

  CIEC_STRING copyOfOriginal(paNodePathConst);

  char* nodePath = copyOfOriginal.getValue();

  if('/' == nodePath[copyOfOriginal.length() - 1]) { // remove tailing slash
    nodePath[copyOfOriginal.length() - 1] = '\0';
  }

  CParameterParser folderParser(nodePath + 1, '/'); // + 1 to skip the first slash

  (*paFolderCount) = folderParser.parseParameters();

  UA_NodeId startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);

  // for every folder (which is a BrowsePath) we want to get the node id
  *paBrowsePaths = static_cast<UA_BrowsePath *>(UA_Array_new((*paFolderCount) * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]));
  UA_BrowsePath *browsePaths = *paBrowsePaths;

  for(size_t i = 0; i < (*paFolderCount); i++) {
    const char *tok = folderParser[i];
    UA_BrowsePath_init(&browsePaths[i]);
    browsePaths[i].startingNode = startingNode;
    browsePaths[i].relativePath.elementsSize = i + 1;

    browsePaths[i].relativePath.elements = static_cast<UA_RelativePathElement *>(UA_Array_new(browsePaths[i].relativePath.elementsSize,
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

      UA_UInt16 browsenameNamespace = i ? 1 : 0; //TODO: use the fix 0 only for the first one and then take it from the server
      CIEC_STRING targetName;
      CParameterParser browseNameParser(tok, ':');
      size_t parsingResult = browseNameParser.parseParameters();
      if(scmMaxNoOfParametersInBrowseName == parsingResult) {
        browsenameNamespace = static_cast<UA_UInt16>(forte::core::util::strtol(browseNameParser[0], 0, 10));
        targetName = browseNameParser[1];
      } else if(1 == parsingResult) {
        targetName = browseNameParser[0];
      } else {
        DEVLOG_ERROR("[OPC UA HELPER]: Error by parsing FB browse path %s\n", paNodePathConst);
        UA_Array_delete(browsePaths, *paFolderCount * 2, &UA_TYPES[UA_TYPES_BROWSEPATH]);
        return UA_STATUSCODE_BADINVALIDARGUMENT;
      }
      browsePaths[i].relativePath.elements[j].targetName = UA_QUALIFIEDNAME_ALLOC(browsenameNamespace, targetName.getValue());
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

UA_StatusCode COPC_UA_Helper::getRemoteNodeForPath(UA_Client *paClient, UA_NodeId **paFoundNodeId, const char *paNodePathConst, UA_NodeId **paParentNodeId) {
  *paFoundNodeId = NULL;
  UA_BrowsePath *browsePaths = 0;
  size_t folderCnt = 0;
  UA_StatusCode retVal = prepareBrowseArgument(paNodePathConst, &browsePaths, &folderCnt);
  if(UA_STATUSCODE_GOOD == retVal) {

    UA_TranslateBrowsePathsToNodeIdsRequest request;
    UA_TranslateBrowsePathsToNodeIdsResponse response;

    UA_TranslateBrowsePathsToNodeIdsRequest_init(&request);

    request.browsePaths = browsePaths;
    request.browsePathsSize = folderCnt * 2;

    response = UA_Client_Service_translateBrowsePathsToNodeIds(paClient, request);

    retVal = response.responseHeader.serviceResult;
    if(retVal == UA_STATUSCODE_GOOD) {
      if(response.resultsSize == folderCnt * 2) {
        int foundFolderOffset = getFolderOffset(response.results, folderCnt);

        if(foundFolderOffset >= 0) {
          // all nodes exist, so just copy the node ids
          copyNodeIds(paFoundNodeId, response.results, foundFolderOffset, paParentNodeId, folderCnt);
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
      DEVLOG_ERROR("[OPC UA HELPER]: Could not translate browse paths for '%s' to node IDs. Error: %s\n", paNodePathConst,
        UA_StatusCode_name(retVal));
    }

    //browsePaths is deleted inside
    UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
    UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
  }

  return retVal;
}

int COPC_UA_Helper::getFolderOffset(UA_BrowsePathResult* browsePathsResults, size_t folderCnt) {
  int foundFolderOffset = -1;
  if(browsePathsResults[folderCnt - 1].statusCode == UA_STATUSCODE_GOOD) {
    foundFolderOffset = 0;
  } else if(browsePathsResults[folderCnt * 2 - 1].statusCode == UA_STATUSCODE_GOOD) {
    foundFolderOffset = static_cast<int>(folderCnt);
  }
  return foundFolderOffset;
}

void COPC_UA_Helper::copyNodeIds(UA_NodeId **paFoundNodeId, UA_BrowsePathResult* browsePathsResults, int foundFolderOffset, UA_NodeId **paParentNodeId,
    size_t folderCnt) {
  *paFoundNodeId = UA_NodeId_new();
  UA_NodeId_copy(&browsePathsResults[foundFolderOffset + folderCnt - 1].targets[0].targetId.nodeId, *paFoundNodeId);
  if(paParentNodeId && folderCnt >= 2) {
    *paParentNodeId = UA_NodeId_new();
    UA_NodeId_copy(&browsePathsResults[foundFolderOffset + folderCnt - 2].targets[0].targetId.nodeId, *paParentNodeId);
  } else if(paParentNodeId) {
    *paParentNodeId = UA_NodeId_new();
    **paParentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
  }
}


