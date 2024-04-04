/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Markus Meingast - initial implementation
 *******************************************************************************/

#pragma once

#include "opcua_layer.h"
#include <memory>

class COPC_UA_HandlerAbstract;
class CActionInfo;

namespace forte {
  namespace com_infra {
    class CBaseCommFB;
  }
}

class COPC_UA_ObjectStruct_Helper {
  public:

    COPC_UA_ObjectStruct_Helper(COPC_UA_Layer &paLayer, COPC_UA_HandlerAbstract *paHandler);

    ~COPC_UA_ObjectStruct_Helper();

    /**
    * Uninitialize ActionInfos of Struct and Struct members
    */
    void uninitializeStruct();

    /**
     * Check that the Struct Object type of the SDs is valid
     * NOTE: Supports only one connected Struct Type and SDs currently
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     * @return True if Struct Object Type is valid, false otherwise
     */
    bool checkStructTypeConnection(bool paIsPublisher);

    /**
     * Create an OPC UA Object Node from Struct Type, if it is not present
     * @param paActionInfo The ActionInfo to create the Object Node from
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     * @return e_InitOk if Object Node was created successfully, e_InitTerminated otherwise
    */
    forte::com_infra::EComResponse createObjectNode(CActionInfo& paActionInfo, bool paIsPublisher);

    /**
     * Get Object Struct member from ActionInfo browsepath
     * @param paActionInfo ActionInfo to get the browsepath from
     * @param paIsSD True if the local port is an SD, false otherwise
     * @return The pointer to the member variable
    */
    CIEC_ANY const *getStructMember(CActionInfo &paActionInfo, bool paIsSD);

    /**
     * Execute the action for a Object Node Struct
     * @return e_ProcessDataOK if no problem occurred, other value otherwise
     */
    forte::com_infra::EComResponse executeStructAction();

    /**
     * Get index to the corresponding Object Struct RDBuffer entry from the Node ID
     * @param paNodeId The Node ID
     * @return The index to the corresponding RDBuffer entry
    */
    int getRDBufferIndexFromNodeId(const UA_NodeId *paNodeId);

    /**
     * Set values of Object Struct members from the RDBuffer
     *
     * @param paRDs      the array of data pointers to be sent
     * @param paRDBuffer The buffer for the data
    */
    static void setMemberValues(CIEC_ANY** paRDs, CIEC_ANY **paRDBuffer);

    /**
     * Initialize RDBuffer for Object Structs
     * @return The initialized buffer
    */
    CIEC_ANY **initializeRDBuffer();

    /**
     * Delete all entries of the RDBuffer
     *
     * @param paCommFB   The comm fb for which the rdbuffer was created
     * @param paRDBuffer The buffer to be uninitialized
    */
    static void deleteRDBufferEntries(forte::com_infra::CBaseCommFB &paCommFB, CIEC_ANY **paRDBuffer);

    /**
     * Check if Data Connection is a Struct Type
     * @param paLayer The corresponding OPC UA layer
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     * @return True if connected data type is Struct, false otherwise
    */
    static bool isStructType(const COPC_UA_Layer &paLayer, bool paIsPublisher);
    
  private:

    COPC_UA_Layer &mLayer; 
    COPC_UA_HandlerAbstract *mHandler;

    /**
     * OPC UA Object Struct Namespace Index
    */
    static const UA_UInt16 smOpcuaNamespaceIndex;

    /**
     * BrowsePath to folder that contains Object Node Struct Types
     */
     static const std::string smStructTypesBrowsePath;

    /**
     * String representation to the Namespace Index for Object Struct Members
    */
     static const std::string smMemberNamespaceIndex;

     static char smEmptyLocale[];

     std::vector<char*> mStructTypeNames;

    /**
     * Pointer to ActionInfo for created OPC UA Struct Object Node
     */
    std::shared_ptr<CActionInfo> mCreateNodeActionInfo;

    /**
     * ActionInfos of Struct members 
    */
    std::vector<std::shared_ptr<CActionInfo>> mStructMemberActionInfos;

    /**
     * Check if Object Node Struct is present in OPC UA server
     * @param paBrowsePath The BrowsePath to the Object Node
     * @return true, if Object Node already exists, false otherwise
    */
    bool isOPCUAObjectPresent(std::string &paBrowsePath);

    /**
     * Get the ActionInfo to create the OPC UA Object Node for Struct Type.
     * Supports only one connected Struct Type currently
     * @param paActionInfo The ActionInfo to create the Object Node from
     * @param paBrowsePath BrowsePath to the Struct Object Node
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     * @return The ActionInfo for creating OPC UA Object Node
    */
    std::shared_ptr<CActionInfo> getCreateObjectActionInfo(CActionInfo& paActionInfo, std::string &paBrowsePath, bool paIsPublisher);

    bool createOPCUAStructType(const std::string &paStructTypeName, CIEC_STRUCT &paStructType);

    /**
     * Perform initialization for Object Struct Members
     * @param paActionInfo The ActionInfo to create the Object Node from
     * @param paBrowsePath The browsepath to the Object Struct
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     * @return e_InitOk if initialization was successful, e_InitTerminated otherwise
     */
    forte::com_infra::EComResponse initializeMemberAction(CActionInfo& paActionInfo, std::string &paBrowsePath, bool paIsPublisher);
    
    /**
     * Get the BrowsePath to the OPC UA Struct Object Type from the local Struct Type
     * @param paPathPrefix The BrowsePath directory with namespace (e.g. /Objects/1:)
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     */
    std::string getStructBrowsePath(const std::string &paPathPrefix, bool paIsPublisher);

    /**
     * @param paIsPublisher True if the FB is a Publisher, false othewise
    */
    std::string getStructTypeName(bool paIsPublisher);

    /**
     * Get the BrowsePath to the OPC UA Object Struct members from the local Struct Type
     * @param paBrowsePathPrefix BrowsePath to the Struct Object Node
     * @param structMemberNameId Name Id of Object Node Struct member
     */
    static std::string getStructMemberBrowsePath(std::string &paBrowsePathPrefix, const CStringDictionary::TStringId structMemberNameId);

    bool defineOPCUAStructTypeNode(UA_Server *paServer, UA_NodeId &paNodeId, const std::string &paStructTypeName);

    bool addOPCUAStructTypeComponent(UA_Server *paServer, UA_NodeId &paParentNodeId, CIEC_ANY *paStructMember, const std::string &paStructMemberName);

};
