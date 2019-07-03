/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
 *               fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *    Jose Cabral:
 *      - refactoring to cleaner architecture
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUALOCALHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUALOCALHANDLER_H_

#include <forte_thread.h>
#include "opcua_handler_abstract.h"
#include <conn.h>
#include <stdio.h>
#include "comlayer.h"
#include <forte_config.h>
#include "opcua_helper.h"
#include "opcua_layer.h"

// cppcheck-suppress noConstructor
class COPC_UA_Local_Handler : public COPC_UA_HandlerAbstract, public CThread {
  DECLARE_HANDLER(COPC_UA_Local_Handler)
  public:

    static UA_StatusCode onServerMethodCall(UA_Server *paServer, const UA_NodeId *paSessionId, void *paSessionContext, const UA_NodeId *paMethodId,
        void *paMethodContext, const UA_NodeId *paObjectId, void *paObjectContext, size_t paInputSize, const UA_Variant *paInput, size_t paOutputSize,
        UA_Variant *paOutput);

    /**
     * Callback when an external client writes to a variable on this server.
     *
     * @param h handle passed to the write method.
     * @param nodeid node id of the changed variable
     * @param data the new data for the variable
     * @param range range indicating the size of the data variant.
     */
    static void onWrite(UA_Server *paServer, const UA_NodeId *paSessionId, void *paSessionContext, const UA_NodeId *paNodeId, void *paNodeContext,
        const UA_NumericRange *paRange, const UA_DataValue *paData);

    /* functions needed for the external event handler interface */
    void enableHandler(void);

    void disableHandler(void);

  protected:

    virtual UA_StatusCode initializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    virtual UA_StatusCode executeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    virtual UA_StatusCode uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

  private:

    UA_StatusCode executeWrite(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode initializeCreateMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode executeCreateMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode initializeCreateObject(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode executeCreateObject(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode initializeDeleteObject(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode executeDeleteObject(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode splitAndCreateFolders(CIEC_STRING& paBrowsePath, CIEC_STRING& paNodeName,
        CSinglyLinkedList<UA_NodeId*>& paRreferencedNodes);

    void handlePresentNodes(CSinglyLinkedList<UA_NodeId*>& paPresentNodes, CSinglyLinkedList<UA_NodeId*>& paReferencedNodes, bool paFailed);

    bool isTypeOfObjectPresent(CNodePairInfo* paNodePairInfo);

    /**
     * Starts the OPC UA server, if it is not already running
     */
    void startServer();

    /**
     * Sets the uaServerRunningFlag to false which causes the UA Server to stop.
     */
    void stopServer();

    /**
     * Creates the configuration for the OPC UA Server.
     * @param UAServerPort port where the OPC UA Server should listen.
     */
    void configureUAServer(TForteUInt16 paUAServerPort, UA_ServerConfig **paUaServerConfig);

    /**
     * Overridden run() from CThread which loops the UA Server.
     */
    virtual void run();

    /**
     * Collector list for callback handles to be able to clean them up on destroy.
     */
    CSinglyLinkedList<struct UA_NodeCallback_Handle *> nodeCallbackHandles;

    struct nodesReferencedByActions {
        const UA_NodeId *mNodeId;
        CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*> mActionsReferencingIt;
    };

    /* a map to know which layers are using the Node */
    CSinglyLinkedList<struct nodesReferencedByActions*> mNodesReferences;
    CSyncObject mNodeLayerMutex;
    CSemaphore mServerStarted;

    struct UA_ParentNodeHandler {
        UA_NodeId *mParentNodeId;
        UA_NodeId *mMethodNodeId;
        COPC_UA_HandlerAbstract::CLocalMethodInfo* mActionInfo;
    };

    static CSinglyLinkedList<UA_ParentNodeHandler *> methodsWithoutContext;

    class CCreateInfo {
      public:
        CCreateInfo() :
            mRequestedNodeId(0), mParentNodeId(0), mBrowseName(UA_QualifiedName_new()), mReturnedNodeId(UA_NodeId_new()) {
        }

        virtual ~CCreateInfo() {
          UA_NodeId_delete(mReturnedNodeId);
          UA_QualifiedName_delete(mBrowseName);
        }

        UA_NodeId* mRequestedNodeId;
        const UA_NodeId* mParentNodeId;
        UA_QualifiedName* mBrowseName;
        UA_NodeId* mReturnedNodeId;

      private:
        CCreateInfo(CCreateInfo& other);
        CCreateInfo& operator=(const CCreateInfo& other);
    };

    class CCreateObjectInfo : public CCreateInfo {
      public:
        CCreateObjectInfo() :
            CCreateInfo(), mTypeNodeId(0) {

        }
        UA_NodeId* mTypeNodeId;
    };

    class CCreateVariableInfo : public CCreateInfo {
      public:
        CCreateVariableInfo() :
            CCreateInfo(), mTypeConvert(0), mInitData(0), mAllowWrite(false) {

        }
        const UA_TypeConvert* mTypeConvert;
        const CIEC_ANY *mInitData;
        bool mAllowWrite;
    };

    class CCreateMethodInfo : public CCreateInfo {
      public:
        CCreateMethodInfo() :
            CCreateInfo(), mInputArguments(0), mOutputArguments(0), mCallback(0), mInputSize(0), mOutputSize(0) {

        }
        ~CCreateMethodInfo() {
          UA_Array_delete(mInputArguments, mInputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
          UA_Array_delete(mOutputArguments, mOutputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
        }

        UA_Argument *mInputArguments;
        UA_Argument *mOutputArguments;
        COPC_UA_HandlerAbstract::CLocalMethodInfo* mCallback;
        size_t mInputSize;
        size_t mOutputSize;
      private:
        CCreateMethodInfo(CCreateMethodInfo& other);
        CCreateMethodInfo& operator=(const CCreateMethodInfo& other);
    };

    CSyncObject getNodeForPathMutex;

    UA_StatusCode handleExistingVariable(COPC_UA_HandlerAbstract::CActionInfo& paInfo, CNodePairInfo* paNodePairInfo, UA_TypeConvert* paTypeConvert,
        size_t paIndexOfNodePair, bool paWrite);

    bool getBroswenameFromNodeName(CIEC_STRING& paNodeName, UA_QualifiedName& paBrowseName);

    bool initializeNodesets(UA_Server* paUaServer);

    bool initializeCreateInfo(CIEC_STRING& paNodeName, CNodePairInfo* paNodePair, const UA_NodeId* paParentNodeId, CCreateInfo& paResult);

    UA_StatusCode initializeReadWrite(COPC_UA_HandlerAbstract::CActionInfo& paInfo, bool isWrite);

    UA_StatusCode getNode(const UA_NodeId* paParentNode, CNodePairInfo* paNodeInfo, CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds, bool* paIsPresent);

    UA_StatusCode createVariableNode(CCreateVariableInfo* paNodeInformation);

    UA_StatusCode storeAlreadyExistingNodes(UA_BrowsePathResult* mNodesReferences, size_t paFolderCnt, size_t* paFirstNonExistingNode,
        CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds);

    UA_StatusCode createObjectNode(CCreateObjectInfo* paNodeInformation);

    UA_StatusCode translateBrowseNameAndStore(const char* paBrowsePath, UA_BrowsePath **paBrowsePaths, size_t *paFoldercount, size_t *paFirstNonExistingNode,
        CSinglyLinkedList<UA_NodeId*>& paFoundNodeIds);

    UA_StatusCode createFolders(const char* paFolders, CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds);

    bool splitFoldersFromNode(const CIEC_STRING& paOriginal, CIEC_STRING& paFolder, CIEC_STRING& paNodeName);

    static const size_t scmMaxServerNameLength = 255;

    void cleanNodeReferences();

    void createMethodArguments(CCreateMethodInfo * paMethodInformation, COPC_UA_HandlerAbstract::CActionInfo & paInfo);

    void referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *> *paNodes, COPC_UA_HandlerAbstract::CActionInfo* const paLayer);

    void referencedNodesDecrement(const COPC_UA_HandlerAbstract::CActionInfo* paLayer);

    void getNodesReferencedByAction(const COPC_UA_HandlerAbstract::CActionInfo *paActionInfo, CSinglyLinkedList<const UA_NodeId *>& paNodes);

    /**
     * Sets the user access level attribute of the node to the given new value.
     * @param nodeId the node for which to update the access level
     * @param newAccessLevel new access level of the node
     * @return UA_STATUSCODE_GOOD on succes, the error code otherwise
     */
    UA_StatusCode updateNodeUserAccessLevel(const UA_NodeId *paNodeId, UA_Byte paNewAccessLevel);

    /**
     * Create a new method node in the OPC UA information model.
     *
     * @param parentNode parent node where the method should be created
     * @param methodName browse name and display name of the method
     * @param callback Callback which is called when the method is invoked
     * @param callbackHandle handle passed to the callback
     * @param inputArgumentsSize size of the inputArguments array
     * @param inputArguments list of input arguments for the method. These are normally the RD ports
     * @param outputArgumentsSize size of outputArguments array
     * @param outputArguments list of output arguments for the method. These are normally the SD ports
     * @param returnNodeId node id of the newly created method
     * @return UA_STATUSCODE_GOOD on success. Error otherwise
     */
    UA_StatusCode createMethodNode(CCreateMethodInfo* paMethodInfo);

    /**
     * Update UA Address Space node value given by the data pointer to an IEC61499 data object.
     *
     * @param nodeId node id of the variable to be updated
     * @param data new value for the variable
     * @param mTypeConvert converter object to mTypeConvert IEC61499 to OPC UA data value
     * @return UA_STATUSCODE_GOOD on success. Error otherwise
     */
    UA_StatusCode updateNodeValue(const UA_NodeId *paNodeId, const CIEC_ANY *paData, const UA_TypeConvert *paConvert);

    /**
     * Register a callback routine to a Node in the Address Space that is executed
     * on either write or read access on the node. A handle to the caller communication layer
     * is passed too. This alleviates for the process of searching the
     * originating layer of the external event.
     *
     * @param nodeId
     * @param mComLayer
     * @param mTypeConvert
     * @return
     */
    UA_StatusCode
    registerVariableCallBack(const UA_NodeId *paNodeId, COPC_UA_Layer *paComLayer, const struct UA_TypeConvert *paConvert, size_t paPortIndex);

    UA_StatusCode handleExistingMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo, UA_NodeId *paParentNode);

    static const size_t scmMethodCallTimeout = 4;

    struct CLocalMethodCall {
        COPC_UA_HandlerAbstract::CLocalMethodInfo *mActionInfo;
        UA_SendVariable_handle* mSendHandle;
    };

    CSinglyLinkedList<CLocalMethodCall*> mMethodCalls;

    CSyncObject mMethodCallsMutex;

    CLocalMethodCall* addMethodCall(COPC_UA_HandlerAbstract::CLocalMethodInfo *paActionInfo, UA_SendVariable_handle* paHandleRecv);

    void removeMethodCall(CLocalMethodCall* toDelete);

    void cleanMethodCalls();

    CLocalMethodCall* getLocalMethodCall(COPC_UA_HandlerAbstract::CLocalMethodInfo *paActionInfo);

    // OPC UA Server and configuration
    UA_Server *mUaServer;

    // OPC UA Client and configuration
    volatile UA_Boolean mUaServerRunningFlag;

    static const UA_UInt16 scmDefaultBrowsenameNameSpace = 1;

    static const char* const mEnglishLocaleForNodes;
    static const char* const mDescriptionForVariableNodes;

#ifdef FORTE_COM_OPC_UA_MULTICAST
# ifndef UA_ENABLE_DISCOVERY_MULTICAST
#  error open62541 needs to be built with UA_ENABLE_DISCOVERY_MULTICAST=ON
# else // UA_ENABLE_DISCOVERY_MULTICAST
#  ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    //nothing here
#   else
    UA_ServerConfig* mServerConfig;
#   endif
    /**
     * List of LDS servers where this instance is already registered.
     */
    CSinglyLinkedList<UA_String*> mRegisteredWithLds;

    const UA_String* getDiscoveryUrl() const;
    void registerWithLds(const UA_String *paDiscoveryUrl);
    void removeLdsRegister(const UA_String *paDiscoveryUrl);
    static void serverOnNetworkCallback(const UA_ServerOnNetwork *paServerOnNetwork, UA_Boolean paIsServerAnnounce, UA_Boolean paIsTxtReceived, void* paData);
# endif //UA_ENABLE_DISCOVERY_MULTICAST
#endif //FORTE_COM_OPC_UA_MULTICAST

};

#endif /* SRC_MODULES_OPC_UA_OPCUALOCALHANDLER_H_ */
