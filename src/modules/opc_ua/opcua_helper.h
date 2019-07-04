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

class COPC_UA_Helper {
  public:

    struct UA_TypeConvert {
        const UA_DataType *type;

        bool (*get)(const CIEC_ANY *src, void *dst);

        bool (*set)(const void *src, CIEC_ANY *dst);
    };

    class UA_Variables_handle {
      public:
        explicit UA_Variables_handle(size_t paSize) :
            mFailed(false), mOffset(0), mSize(paSize), mConvert(0) {
          mConvert = new const UA_TypeConvert*[mSize];
        }

        virtual ~UA_Variables_handle() {
          delete[] mConvert;
        }

        bool mFailed;
        size_t mOffset;
        size_t mSize;
        const UA_TypeConvert** mConvert;
      private:
        UA_Variables_handle(const UA_Variables_handle &paObj);
        UA_Variables_handle& operator=(const UA_Variables_handle& other);

    };

    class UA_RecvVariable_handle : public UA_Variables_handle {
      public:
        explicit UA_RecvVariable_handle(size_t paSize) :
            UA_Variables_handle(paSize) {
          mData = new const UA_Variant*[mSize];
        }

        virtual ~UA_RecvVariable_handle() {
          delete[] mData;
        }

        const UA_Variant **mData;
      private:
        UA_RecvVariable_handle(const UA_RecvVariable_handle &paObj);
        UA_RecvVariable_handle& operator=(const UA_RecvVariable_handle& other);
    };

    class UA_SendVariable_handle : public UA_Variables_handle {
      public:
        explicit UA_SendVariable_handle(size_t paSize) :
            UA_Variables_handle(paSize) {
          mData = new UA_Variant*[mSize];
        }

        ~UA_SendVariable_handle() {
          delete[] mData;
        }

        UA_Variant **mData;
      private:
        UA_SendVariable_handle(const UA_SendVariable_handle &paObj);
        UA_SendVariable_handle& operator=(const UA_SendVariable_handle& other);
    };

    /**
     * Maps EDataTypeID to OPC UA data types
     */

    static const UA_TypeConvert* geTypeConvertFromAny(CIEC_ANY* paAny);
    static const UA_TypeConvert mapForteTypeIdToOpcUa[];

    /**
     * Check if given type ID is valid an within the range of the array.
     * @param typeId the type ID to check
     * @return true if valid
     */
    static bool isTypeValid(CIEC_ANY *typeId);

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

    static UA_StatusCode getRemoteNodeForPath(UA_Client *paClient, UA_NodeId **paFoundNodeId, const char *paNodePathConst, UA_NodeId **paParentNodeId);

    static UA_StatusCode releaseBrowseArgument(UA_BrowsePath* paBrowsePaths, size_t paPathLength);

    static UA_StatusCode prepareBrowseArgument(const char *paNodePathConst, UA_BrowsePath** paBrowsePaths, size_t* paFolderCount);

    static bool checkBrowsePath(const char *paBrowsepath);

    static bool getBrowsenameFromNodeName(const char* paNodeName, UA_QualifiedName& paBrowseName, UA_UInt16 paDefaultNamespace);

    static const size_t scmMaxNoOfParametersInBrowseName = 2;

  private:

    static int getFolderOffset(UA_BrowsePathResult* browsePathsResults, size_t folderCnt);

    static void copyNodeIds(UA_NodeId **paFoundNodeId, UA_BrowsePathResult* browsePathsResults, int foundFolderOffset, UA_NodeId **paParentNodeId,
        size_t folderCnt);

};

#endif //FORTE_OPCUA_HELPER_H
