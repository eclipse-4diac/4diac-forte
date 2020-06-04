/*******************************************************************************
 * Copyright (c) 2016, 2019 fortiss GmbH
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
 *******************************************************************************/

#ifndef FORTE_OPCUA_HELPER_H
#define FORTE_OPCUA_HELPER_H

#include <open62541.h>
#include "forte_any.h"

class COPC_UA_Helper {
  public:

    /**
     * Returns the OPC UA data type that correspond to the IEC 61131-3 type
     * @param paAnyType IEC 61131-3 type
     * @return The OPC UA type pointer, or 0 if not found
     */
    static const UA_DataType *getOPCUATypeFromAny(const CIEC_ANY& paAnyType);

    /**
     * Converts any IEC 61131-3 type to its OPC UA Type
     * @param paSrcAny Input to be converted
     * @param paDest Already allocated memory where to store
     * @return The size in the destination it was used
     */
    static size_t convertToOPCUAType(const CIEC_ANY& paSrcAny, void* paDest);

    /**
     * Converts an OPC UA Type into its IEC 61131-3 type
     * @param paSrc Input to be converted
     * @param paDestAny Place to store
     * @return The size of the source that was used
     */
    static size_t convertFromOPCUAType(const void *paSrc, CIEC_ANY &paDestAny);

    /**
     * Fills a variant using the information from a CIEC_ANY type
     * @param paDataValue variant to fill
     * @param paDataSource source of information
     */
    static void fillVariant(UA_Variant &paVariant, const CIEC_ANY &paDataSource);

    /**
     * Parent class with the information for exchanging data between the layer and the handlers.
     * This is needed because the two childs, send and receive differs in the fact that the data is
     * const or no and some ugly cast will be needed. If these is not a problem, the two childs could be
     * deleted and use just the parent
     */
    class UA_Variables_handle {
      public:
        explicit UA_Variables_handle(size_t paSize) :
            mFailed(false), mOffset(0), mSize(paSize) {
        }

        virtual ~UA_Variables_handle() {
        }

        bool mFailed;
        size_t mOffset;
        size_t mSize;
      private:
        UA_Variables_handle(const UA_Variables_handle &paObj);
        UA_Variables_handle& operator=(const UA_Variables_handle& other);

    };

    /**
     * Context given to the layer by the handler including the new data that has arrive
     */
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

    /**
     * Context given to the handler by the layer including the new data to be sent
     */
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
     * Check if a browsepath is valid
     * @param paBrowsepath browsepath to be checked
     * @return True if the browsepath is valid, false otherwise
     */
    static bool isBrowsePathValid(const char *paBrowsepath);

    /**
     * Get the node id of the node which is represented by the given path in a remote server
     * @param paClient Client to use to look for the node ID
     * @param paNodePathConst Browsepath of the remote node
     * @param paParentNodeId Place to store the parent node ID of the requested node
     * @param paFoundNodeId Place to store the node ID of the requested node
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    static UA_StatusCode getRemoteNodeForPath(UA_Client &paClient, const char *paNodePathConst, UA_NodeId **paParentNodeId, UA_NodeId **paFoundNodeId);

    /**
     * Prepare the browse request from a browsepath
     * @param paNodePathConst Browsepath to be looked for
     * @param paBrowsePaths Place to store the browse request
     * @param paFolderCount Place to store the amount of folders in the browse request
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    static UA_StatusCode prepareBrowseArgument(const char *paNodePathConst, UA_BrowsePath **paBrowsePaths, size_t *paFolderCount);

    /**
     * Release all memory alloacted in prepareBrowseArgument. It should be called when paBrowsePaths in prepareBrowseArgument is not used anymore
     * @param paBrowsePaths Browse request returned by prepareBrowseArgument
     * @param paPathLength Amount of folders returned by prepareBrowseArgument
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    static UA_StatusCode releaseBrowseArgument(UA_BrowsePath &paBrowsePaths, size_t paPathLength);

    /**
     * Allocated a UA_QualifiedName from a nodename which can have a semicolon to define the namespace. If no semicolon is found
     * the passed deafult namespace is used
     * @param paNodeName String containing the nodename
     * @param paBrowseName Place to store the qualified name
     * @param paDefaultNamespace Default namespace to be used if not present in paNodeName
     * @return True if no error occurred, false otherwise
     */
    static bool getBrowsenameFromNodeName(const char *paNodeName, UA_UInt16 paDefaultNamespace, UA_QualifiedName &paBrowseName);

    /**
     * Maximum number of parameters in a browsename separated by a semicolon
     */
    static const size_t scmMaxNoOfParametersInBrowseName = 2;

  private:

    /**
     * Function pointer definition to convert a IEC 61131-3 type to an OPC UA Type
     */
    typedef size_t (*convertFromIECToOPCUA)(const CIEC_ANY &, void *);

    /**
     * Function pointer definition to convert an OPC UA type to an IEC 61131-3 Type
     */
    typedef size_t (*convertFromOPCUAToIEC)(const void *, CIEC_ANY &);

    /**
     * Class that encapsulates, for each 61131-3 type, the map to the OPC UA type, and a transform to and from functions
     */
    class UA_TypeConvert {
      public:
        UA_TypeConvert(const UA_DataType *paType, convertFromIECToOPCUA paToOPCUA, convertFromOPCUAToIEC paFromOPCUA) :
            mType(paType), mToOPCUA(paToOPCUA), mFromOPCUA(paFromOPCUA) {
        }
        const UA_DataType *mType;
        convertFromIECToOPCUA mToOPCUA;
        convertFromOPCUAToIEC mFromOPCUA;
    };
    class UA_TypeConvert_external {
      public:
        UA_TypeConvert_external(const UA_DataType *paType, const CStringDictionary::TStringId paStringId) :
            mType(paType), mStringId(paStringId) {
        }
        const UA_DataType *mType;
        const CStringDictionary::TStringId mStringId;
    };

    /**
     * Array of all conversion functions for the basic types
     */
    static const UA_TypeConvert scmMapForteTypeIdToOpcUa[];

    /**
     * Array of conversion functions for added types
     */
    static const UA_TypeConvert_external scmExternalMapForteTypeIdToOpcUa[];

    /**
     * If the type being looked for is not a basic one, it will look in the externally added types
     * @param paAnyType IEC 61131-3 container which is used to look for the corresponding OPC UA type
     * @return The OPC UA type pointer, or 0 if not found
     */
    static const UA_DataType *getExternalOPCUATypeFromAny(const CIEC_ANY& paAnyType);

    /**
     * Look for the first occurrence of an existing node from a browse path result
     * @param paBrowsePathsResults Result of browse path service
     * @param folderCnt Number of folders in the paBrowsePathsResults
     * @return The offset to first existing folder, -1 if no folder exist
     */
    static int getFolderOffset(const UA_BrowsePathResult *paBrowsePathsResults, size_t folderCnt);

    /**
     * Copy the found node id and its parent from a browse result
     * @param paFoundNodeId Place to store the found node ID
     * @param paBrowsePathsResults Browse path results where to look for
     * @param paBoundFolderOffset Offset to use to copy the node IDs
     * @param paParentNodeId Place to store the parent node ID
     * @param paFolderCnt Size of the browse result
     */
    static void copyNodeIds(const UA_BrowsePathResult *paBrowsePathsResults, size_t paFolderCnt, int paFoundFolderOffset, UA_NodeId **paParentNodeId,
        UA_NodeId **paFoundNodeId);
};

#endif //FORTE_OPCUA_HELPER_H
