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
#include <conn.h>
#include <forte_sem.h>
#include <forte_sync.h>
#include "../../core/fortelist.h"
#include "opcua_handler_abstract.h"
#include "opcua_helper.h"

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

    virtual UA_StatusCode initializeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode executeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode uninitializeAction(CActionInfo& paActionInfo);

  private:

    UA_StatusCode executeWrite(CActionInfo& paActionInfo);

    UA_StatusCode initializeCreateMethod(CActionInfo& paActionInfo);

    UA_StatusCode executeCreateMethod(CActionInfo& paActionInfo);

    UA_StatusCode initializeCreateObject(CActionInfo& paActionInfo);

    UA_StatusCode executeCreateObject(CActionInfo& paActionInfo);

    UA_StatusCode initializeDeleteObject(CActionInfo& paActionInfo);

    UA_StatusCode executeDeleteObject(CActionInfo& paActionInfo);

    UA_StatusCode splitAndCreateFolders(CIEC_STRING& paBrowsePath, CIEC_STRING& paNodeName,
        CSinglyLinkedList<UA_NodeId*>& paRreferencedNodes);

    void handlePresentNodes(CSinglyLinkedList<UA_NodeId*>& paPresentNodes, CSinglyLinkedList<UA_NodeId*>& paReferencedNodes, bool paFailed);

    bool isTypeOfObjectPresent(CActionInfo::CNodePairInfo& paNodePairInfo);

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

    struct nodesReferencedByActions {
        const UA_NodeId *mNodeId;
        CSinglyLinkedList<CActionInfo*> mActionsReferencingIt;
    };

    /* a map to know which layers are using the Node */
    CSinglyLinkedList<nodesReferencedByActions*> mNodesReferences;
    CSyncObject mNodeLayerMutex;
    CSemaphore mServerStarted;

    class UA_ParentNodeHandler {
      public:
        UA_ParentNodeHandler(UA_NodeId *paParentNodeId, UA_NodeId *paMethodNodeId, CLocalMethodInfo& paActionInfo) :
            mParentNodeId(paParentNodeId), mMethodNodeId(paMethodNodeId), mActionInfo(paActionInfo) {
        }
        ~UA_ParentNodeHandler() {
          UA_NodeId_delete(mParentNodeId);
        }

        UA_ParentNodeHandler(const UA_ParentNodeHandler& other) :
            mParentNodeId(UA_NodeId_new()), mMethodNodeId(other.mMethodNodeId), mActionInfo(other.mActionInfo) {
          UA_NodeId_copy(other.mParentNodeId, mParentNodeId);
        }

        UA_NodeId *mParentNodeId;
        UA_NodeId *mMethodNodeId;
        CLocalMethodInfo& mActionInfo;

      private:
        UA_ParentNodeHandler& operator=(const UA_ParentNodeHandler& other);
    };

    static CSinglyLinkedList<UA_ParentNodeHandler> methodsWithoutContext;

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
        CCreateInfo(const CCreateInfo& other);
        CCreateInfo& operator=(const CCreateInfo& other);
    };

    class CCreateObjectInfo : public CCreateInfo {
      public:
        CCreateObjectInfo() :
            CCreateInfo(), mTypeNodeId(0) {
        }

        ~CCreateObjectInfo() {
        }

        UA_NodeId* mTypeNodeId;
      private:
        CCreateObjectInfo(const CCreateObjectInfo &paObj);
        CCreateObjectInfo& operator=(const CCreateObjectInfo& other);
    };

    class CCreateVariableInfo : public CCreateInfo {
      public:
        CCreateVariableInfo() :
            CCreateInfo(), mTypeConvert(0), mInitData(0), mAllowWrite(false) {

        }

        ~CCreateVariableInfo() {
        }

        const COPC_UA_Helper::UA_TypeConvert* mTypeConvert;
        const CIEC_ANY *mInitData;
        bool mAllowWrite;
      private:
        CCreateVariableInfo(const CCreateVariableInfo &paObj);
        CCreateVariableInfo& operator=(const CCreateVariableInfo& other);
    };

    class CCreateMethodInfo : public CCreateInfo {
      public:
        explicit CCreateMethodInfo(CLocalMethodInfo& paCallack) :
            CCreateInfo(), mInputArguments(0), mOutputArguments(0), mLocalMethodInfo(paCallack), mInputSize(0), mOutputSize(0) {

        }
        ~CCreateMethodInfo() {
          UA_Array_delete(mInputArguments, mInputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
          UA_Array_delete(mOutputArguments, mOutputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
        }

        UA_Argument *mInputArguments;
        UA_Argument *mOutputArguments;
        CLocalMethodInfo& mLocalMethodInfo;
        size_t mInputSize;
        size_t mOutputSize;
      private:
        CCreateMethodInfo(CCreateMethodInfo& other);
        CCreateMethodInfo& operator=(const CCreateMethodInfo& other);
    };

    struct UA_VariableContext_Handle {
        UA_VariableContext_Handle(CActionInfo& paActionInfo, const COPC_UA_Helper::UA_TypeConvert *paConvert, size_t paPortIndex) :
            mActionInfo(paActionInfo), mConvert(paConvert), mPortIndex(paPortIndex) {
        }

        //default copy constructor should be enough

        bool operator==(UA_VariableContext_Handle const& paRightObject) const {
          return (&mActionInfo == &paRightObject.mActionInfo && mConvert == paRightObject.mConvert && mPortIndex == paRightObject.mPortIndex);
        }

        CActionInfo& mActionInfo;
        const COPC_UA_Helper::UA_TypeConvert *mConvert;
        size_t mPortIndex;
    };

    /**
     * Collector list for callback handles to be able to clean them up on destroy.
     */
    CSinglyLinkedList<UA_VariableContext_Handle> mNodeCallbackHandles;

    CSyncObject mCreateNodesMutex;

    UA_StatusCode handleExistingVariable(CActionInfo& paActionInfo, CActionInfo::CNodePairInfo& paNodePairInfo, COPC_UA_Helper::UA_TypeConvert* paTypeConvert,
        size_t paIndexOfNodePair, bool paWrite);

    UA_StatusCode handleNonExistingVariable(CActionInfo& paActionInfo, CActionInfo::CNodePairInfo& paNodePairInfo,
        COPC_UA_Helper::UA_TypeConvert* paTypeConvert, size_t paIndexOfNodePair, CSinglyLinkedList<UA_NodeId*>& paReferencedNodes, bool paWrite);

    bool initializeNodesets(UA_Server* paUaServer);

    void initializeCreateInfo(CIEC_STRING& paNodeName, CActionInfo::CNodePairInfo& paNodePairInfo, const UA_NodeId* paParentNodeId, CCreateInfo& paResult);

    UA_StatusCode initializeReadWrite(CActionInfo& paActionInfo, bool isWrite);

    UA_StatusCode getNode(CActionInfo::CNodePairInfo& paNodeInfo, CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds, bool* paIsPresent);

    UA_StatusCode createVariableNode(CCreateVariableInfo& paCreateVariableInfo);

    UA_StatusCode storeAlreadyExistingNodes(UA_BrowsePathResult* mNodesReferences, size_t paFolderCnt, size_t* paFirstNonExistingNode,
        CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds);

    UA_StatusCode createObjectNode(CCreateObjectInfo& paCreateObjectInfo);

    UA_StatusCode translateBrowseNameAndStore(const char* paBrowsePath, UA_BrowsePath **paBrowsePaths, size_t *paFoldercount, size_t *paFirstNonExistingNode,
        CSinglyLinkedList<UA_NodeId*>& paFoundNodeIds);

    UA_StatusCode createFolders(const char* paFolders, CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds);

    bool splitFoldersFromNode(const CIEC_STRING& paOriginal, CIEC_STRING& paFolder, CIEC_STRING& paNodeName);

    static const size_t scmMaxServerNameLength = 255;

    void cleanNodeReferences();

    void createMethodArguments(CCreateMethodInfo& paCreateMethodInfo, CActionInfo & paActionInfo);

    void referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *>& paNodes, CActionInfo& paLayer);

    void referencedNodesDecrement(const CActionInfo& paLayer);

    void getNodesReferencedByAction(const CActionInfo& paActionInfo, CSinglyLinkedList<const UA_NodeId *>& paNodes);

    /**
     * Sets the user access level attribute of the node to the given new value.
     * @param nodeId the node for which to update the access level
     * @param newAccessLevel new access level of the node
     * @return UA_STATUSCODE_GOOD on succes, the error code otherwise
     */
    UA_StatusCode updateNodeUserAccessLevel(const UA_NodeId& paNodeId, UA_Byte paNewAccessLevel);

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
    UA_StatusCode createMethodNode(CCreateMethodInfo& paMethodInfo);

    /**
     * Update UA Address Space node value given by the data pointer to an IEC61499 data object.
     *
     * @param nodeId node id of the variable to be updated
     * @param data new value for the variable
     * @param mTypeConvert converter object to mTypeConvert IEC61499 to OPC UA data value
     * @return UA_STATUSCODE_GOOD on success. Error otherwise
     */
    UA_StatusCode updateNodeValue(const UA_NodeId *paNodeId, const CIEC_ANY *paData, const COPC_UA_Helper::UA_TypeConvert *paConvert);

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
    registerVariableCallBack(const UA_NodeId& paNodeId, CActionInfo& paActionInfo, const COPC_UA_Helper::UA_TypeConvert *paConvert,
        size_t paPortIndex);

    UA_StatusCode handleExistingMethod(CActionInfo& paActionInfo, UA_NodeId *paParentNode);

    static const size_t scmMethodCallTimeout = 4;

    struct CLocalMethodCall {
        CLocalMethodCall(CLocalMethodInfo& paActionInfo, COPC_UA_Helper::UA_SendVariable_handle& paSendHandle) :
            mActionInfo(paActionInfo), mSendHandle(paSendHandle) {
        }

        //default copy constructor should be enough

        bool operator==(CLocalMethodCall const& rhs) const {
          return this == &rhs;
        }

        CLocalMethodInfo& mActionInfo;
        COPC_UA_Helper::UA_SendVariable_handle& mSendHandle;

    };

    CSinglyLinkedList<CLocalMethodCall> mMethodCalls;

    CSyncObject mMethodCallsMutex;

    CLocalMethodCall& addMethodCall(CLocalMethodInfo& paActionInfo, COPC_UA_Helper::UA_SendVariable_handle& paHandleRecv);

    void removeMethodCall(CLocalMethodCall& toDelete);

    CLocalMethodCall* getLocalMethodCall(CLocalMethodInfo& paActionInfo);

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
