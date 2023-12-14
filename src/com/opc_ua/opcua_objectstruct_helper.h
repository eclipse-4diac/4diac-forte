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

// class COPC_UA_Layer;
class COPC_UA_HandlerAbstract;
class CActionInfo;

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
     * @param paRDBuffer The buffer for the data
    */
    void setMemberValues(CIEC_ANY ***paRDBuffer);

    /**
     * Initialize RDBuffer for Object Structs
     * @param paRDBuffer The buffer to be initialized
    */
    void initializeRDBuffer(CIEC_ANY ***paRDBuffer);

    /**
     * Delete all entries of the RDBuffer
     * @param paRDBuffer The buffer to be uninitialized
    */
    void deleteRDBufferEntries(CIEC_ANY ***paRDBuffer);

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
     * BrowsePath to folder that contains Object Node Struct Types
     */
     static const std::string structTypesBrowsePath;

     static const std::string memberNamespaceIndex;

    /**
     * Pointer to ActionInfo for created OPC UA Struct Object Node
     */
    std::shared_ptr<CActionInfo> mCreatetNodeActionInfo;

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
     * @param paBrowsePath Place to store the BrowsePath to the OPC UA Struct Object Type
     * @param paPathPrefix The BrowsePath directory with namespace (e.g. /Objects/1:)
     * @param paIsPublisher True if the FB is a Publisher, false othewise
     */
    void getStructBrowsePath(std::string &paBrowsePath, const std::string &paPathPrefix, bool paIsPublisher);

    /**
     * Get the BrowsePath to the OPC UA Object Struct members from the local Struct Type
     * @param paMemberBrowsePath Place to store the BrowsePath to the OPC UA Struct Object Type
     * @param paBrowsePathPrefix BrowsePath to the Struct Object Node
     * @param structMemberNameId Name Id of Object Node Struct member
     */
    static void getStructMemberBrowsePath(std::string &paMemberBrowsePath, std::string &paBrowsePathPrefix, const CStringDictionary::TStringId structMemberNameId);


    /**
     * @param paStructTypeName Place to store the name of the Struct Type
     * @param paIsPublisher True if the FB is a Publisher, false othewise
    */
    void getStructTypeName(std::string &paStructTypeName, bool paIsPublisher);

};