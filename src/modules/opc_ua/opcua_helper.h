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

#ifndef FORTE_OPCUA_HELPER_H
#define FORTE_OPCUA_HELPER_H

#include <open62541.h>
#include "forte_any.h"
#include <forte_sync.h>
#include <fortelist.h>

struct UA_TypeConvert {
    const UA_DataType *type;

    bool (*get)(const CIEC_ANY *src, void *dst);

    bool (*set)(const void *src, CIEC_ANY *dst);
};

class COPC_UA_Helper {
  public:
    /**
     * Maps EDataTypeID to OPC UA data types
     */
    static const UA_TypeConvert mapForteTypeIdToOpcUa[CIEC_ANY::e_WSTRING + 1];

    /**
     * Check if given type ID is valid an within the range of the array.
     * @param typeId the type ID to check
     * @return true if valid
     */
    static bool isTypeIdValid(CIEC_ANY::EDataTypeID typeId);

    /**
     * Convert forte DataType (CIEC_...) to the corresponding OPC UA datatype.
     *
     * @param typeId type id of the forte datatype
     * @param src source data
     * @param dst the value of src will be converted and stored in dst. It has to be initialized before.
     * @return false if type is not supported
     */
    static inline bool convertToOpcUa_typeId(CIEC_ANY::EDataTypeID typeId, const CIEC_ANY *src, void *dst) {
      if(!COPC_UA_Helper::isTypeIdValid(typeId))
        return false;
      return COPC_UA_Helper::mapForteTypeIdToOpcUa[typeId].get(src, dst);
    }

    /**
     * Convert OPC UA DataType to the corresponding forte DataType (CIEC_...).
     *
     * @param typeId type id of the forte datatype
     * @param src source data
     * @param dst the value of src will be converted and stored in dst. It has to be initialized before.
     * @return false if type is not supported
     */
    static inline bool convertFromUa_typeId(CIEC_ANY::EDataTypeID typeId, const void *src, CIEC_ANY *dst) {
      if(!COPC_UA_Helper::isTypeIdValid(typeId))
        return false;
      return COPC_UA_Helper::mapForteTypeIdToOpcUa[typeId].set(src, dst);
    }

    /**
     * Get the node id of the node which is represented by the given path.
     * If createIfNotFound is set to true, all nodes which do not exist yet will be created.
     *
     * @param foundNodeId the node ID which corresponds to the nodePath or null if it is not found and createIfNotFound is false.
     * @param nodePath Full path to the node, e.g. '/Objects/FlipFlop'
     * @param createIfNotFound if true all the missing nodes will be created as FolderType objects.
     * @param startingNode the nodeId from where the given browse path starts. If NULL, the starting Node is Root and the path has to
     *       start with '/Objects'
     * @param newNodeType type of the new node (only used for the last one), if it has to be created.
     *       Default is FolderType which is also used for intermediate nodes, if they don't exist
     * @param clientInitialized optional client which should be used to get the node. The client has to be already connected if given.
     *       If no client is given, a new temporary client will be created and connected to localhost
     * @return UA_STATUSCODE_GOOD on success or the corresponding error code.
     */

    static UA_StatusCode getRemoteNodeForPath(UA_Client *paClient, UA_NodeId **paFoundNodeId, const char *paNodePathConst, const UA_NodeId *paStartingNode =
    NULL, UA_NodeId **paParentNodeId = NULL);

    static UA_StatusCode releaseBrowseArgument(UA_BrowsePath* paBrowsePaths, size_t paPathLength);

    static UA_StatusCode prepareBrowseArgument(const char *paNodePathConst, const UA_NodeId* paParent, UA_BrowsePath** paBrowsePaths, size_t* paFolderCount);

  private:

    static const size_t scmMaxNoOfParametersInBrowseName = 2;

    /**
     * Mutex for making sure that getNodeForPath is only called by one layer at a time.
     * If a race condition would occur, two layers may try to create the same node at the same time.
     */
    static CSyncObject getNodeForPathMutex;

    static int getFolderOffset(UA_BrowsePathResult* browsePathsResults, size_t folderCnt);

    static void copyNodeIds(UA_NodeId **paFoundNodeId, UA_BrowsePathResult* browsePathsResults, int foundFolderOffset, UA_NodeId **paParentNodeId,
        const UA_NodeId *paStartingNode, size_t folderCnt);

};

#endif //FORTE_OPCUA_HELPER_H
