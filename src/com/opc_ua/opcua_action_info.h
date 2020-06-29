/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_
#define SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_

#include <forte_sem.h>
#include "opcua_layer.h"

/**
 * Each Communication FB (PUBLISH, SUBSCRIBE, CLIENT, SERVER) is considered an action to be executed in the OPC UA library, either locally or remotely.
 * This class encapsulates all information needed to perform the action. It has a factory to create a new action from the parameters set in the ID
 * of the FB
 */
class CActionInfo {
  public:

    /**
     * Allowed type of actions. If a new action is to be added, it should go before eActionUnknown
     */
    enum UA_ActionType {
      eRead, //!< eRead Read a data variable
      eWrite, //!< eWrite Write to a data variable
      eCreateMethod, //!< eCreateMethod Create a method
      eCallMethod, //!< eCallMethod Call a method
      eSubscribe, //!< eSubscribe Subscribe to changes of a variable
      eCreateObject, //!< eCreateObject Create an object
      eCreateVariable, //!< eCreateObject Create a variable
      eDeleteObject, //!< eDeleteObject Delete an object
      eDeleteVariable, //!< eDeleteObject Delete a variable
      eActionUnknown, //!< eActionUnknown The provided action is unknown. This is used also to set the length of known actions
    };

    /**
     * 2-Tuple to reference a specific node in an OPC UA server composed of the node ID and browsepath of the node
     */
    struct CNodePairInfo {
        CNodePairInfo(UA_NodeId *paNodeId, const char *paBrowsePath) :
            mNodeId(paNodeId), mBrowsePath(paBrowsePath) {
        }
        UA_NodeId *mNodeId;
        CIEC_STRING mBrowsePath;
    };

    /**
     * Constructor of the class.
     * @param paLayer The layer that creates and executes the action
     * @param paAction The action to be executed
     * @param paEndpoint The endpoint of a remote OPC UA in case the action is to be executed remotely. An empty endpoint means that the action is to be executed locally
     */
    explicit CActionInfo(COPC_UA_Layer &paLayer, UA_ActionType paAction, const CIEC_STRING &paEndpoint);

    /**
     * Destructor of the class
     */
    virtual ~CActionInfo();

    /**
     * Getter of the action type
     * @return Action type
     */
    UA_ActionType getAction() const {
      return mAction;
    }

    /**
     * Getter of the layer
     * @return Layer
     */
    COPC_UA_Layer& getLayer() {
      return mLayer;
    }

    /**
     * Getter of the enpoint
     * @return A constant reference of the endpoint
     */
    const CIEC_STRING& getEndpoint() const {
      return mEndpoint;
    }

    /**
     * Getter of the list of node pair information
     * @return List of node pair information
     */
    CSinglyLinkedList<CNodePairInfo*>& getNodePairInfo() {
      return mNodePair;
    }

    /**
     * Gets the amount of node pair in the action
     * @return Amount of node pair in the action
     */
    size_t getNoOfNodePairs() const {
      size_t noOfPairs = 0;
      for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = mNodePair.begin(); it != mNodePair.end(); ++it, noOfPairs++)
        ;
      return noOfPairs;
    }

    /**
     * Indicates if the action is to be executed locally or remotely. This function is used by the layer to decide which handler to use (local or remote)
     * @return True if the action is to be executed remotely, false otherwise
     */
    bool isRemote() const;

    /**
     * Factory to retrieve an action type from the parameters defined in the ID data input of the FB. The ID has the format ACTION;[ENDPOINT#];BROWSENAME,NODEID;[BROSWENAME,NODEID];...
     * @param paParams Parameters set in the ID of the FB (string contained in the square brackets of opc_ua[...])
     * @param paLayer The layer that creates and executes the action
     * @param paTypes A list of type converters of the connections of the FB of the action (SDs/RDs)
     * @return
     */
    static CActionInfo* getActionInfoFromParams(const char *paParams, COPC_UA_Layer &paLayer);

    /**
     * Retrieves the array of CIEC_ANY to be sent
     * @return the array of CIEC_ANY to be sent
     */
    const CIEC_ANY* getDataToSend() const;

    /**
     * Retrieves the array of CIEC_ANY where to receive the data
     * @return array of CIEC_ANY where to receive the data
     */
    CIEC_ANY* getDataToReceive() const;

    /**
     * Retrieves the size of the array to send
     * @return size of the array to send
     */
    size_t getSendSize() const;

    /**
     * Retrieves the size of the array to receive
     * @return size of the array to receive
     */
    size_t getReceiveSize() const;

    /**
     * String representations of the actions and which should be provided as the first part of the ID
     */
    static const char *const mActionNames[eActionUnknown];

  private:

    /**
     * Copy constructor is private and not defined to avoid its usage
     */
    CActionInfo(const CActionInfo &paObj);

    /**
     * Assignment operator is private and not defined to avoid its usage
     */
    CActionInfo& operator=(const CActionInfo &paOther);

    /**
     * Checks if the action is valid regarding requirements for the amount of node pairs, endpoint value and the type of action
     * @return True if the action is valid, false otherwise
     */
    bool checkAction() const;

    /**
     * Checks if the provided node pair information is valid
     * @return True if the node pair information is valid, false otherwise
     */
    bool checkNodePairInfo() const;

    /**
     * Specific check for read action
     * @param paFbType The type of FB that wants to executed the read action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the read action is valid, false otherwise
     */
    bool checkReadAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for write action
     * @param paFbType The type of FB that wants to executed the write action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the write action is valid, false otherwise
     */
    bool checkWriteAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for create method action
     * @param paFbType The type of FB that wants to executed the create method action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the create method action is valid, false otherwise
     */
    bool checkCreateMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for call method action
     * @param paFbType The type of FB that wants to executed the call method action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the call method action is valid, false otherwise
     */
    bool checkCallMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for subscribe action
     * @param paFbType The type of FB that wants to executed the subscribe action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the subscribe action is valid, false otherwise
     */
    bool checkSubscribeAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for create object action
     * @param paFbType The type of FB that wants to executed the create object action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the create object action is valid, false otherwise
     */
    bool checkCreateObjectAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for create variable action
     * @param paFbType The type of FB that wants to executed the create variable action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the create variable action is valid, false otherwise
     */
    bool checkCreateVariableAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * Specific check for delete node action
     * @param paFbType The type of FB that wants to executed the delete node action
     * @param paNoOfRDs Number of RDs present in the FB
     * @param paNoOfSDs Number of SDs present in the FB
     * @return True if the delete node action is valid, false otherwise
     */
    bool checkDeleteNodeAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs) const;

    /**
     * The type of action to execute
     */
    UA_ActionType mAction;

    /**
     * The layer that created the action
     */
    COPC_UA_Layer &mLayer;

    /**
     * Empty if the action is executed locally, other value otherwise
     */
    CIEC_STRING mEndpoint;

    /**
     * List of the node pair information about the nodes the action is accessing
     */
    CSinglyLinkedList<CNodePairInfo*> mNodePair;

    static const size_t scmMinimumAmounOfParameters = 2; //at least two are needed

    /**
     * Internal class to parse the parameters that are passed to the factory
     */
    class CActionParser {
      public:

        enum IDPositions {
          eActionType = 0,
          eEndpoint,
          eNodePairs
        };

        /**
         * Retrieves the action type from a string defined in mActionNames
         * @param paParams The string source
         * @return Type according to the string parameter. eActionUnknown is returned if paParams doesn't match any value in mActionNames
         */
        static UA_ActionType getActionEnum(const char *paParams);

        /**
         * Retrieves the endpoint from a string. The endpoint is signalized by a # character at the end of the string
         * @param paEndpoint Source where the endpoint is present
         * @param paResult Place to store the endpoint
         * @return True if an endpoint was found, false otherwise
         */
        static bool getEndpoint(const char *paEndpoint, CIEC_STRING &paResult);

        /**
         * Checks if a pair in a string form is valid and stores it in the result list. The node pair has format BROWSENAME,NODEID where BROWSENAME is a path string. @see parseNodeId for the format of NODEID
         * @param paPair Source string containing the node pair information
         * @param paResult Place to store a new allocated node pair info if the source was valid
         * @return True if paPair contained a valid node pair information, false otherwise
         */
        static bool handlePair(const char *paPair, CSinglyLinkedList<CNodePairInfo*> &paResult);

      private:

        /**
         * Parse a node ID in a string format and returns a new allocated UA_NodeId pointer. The format of the node ID is as follow: <namespaceIndex>:<identifiertype>=<identifier> where namespaceIndex is a number, identifier depends on identifiertype. @see parseIdentifier for the format of identifiertype
         * @param paNodeIdString The string source of the node ID
         * @return 0 if the source string is invalid, a new allocated UA_NodeId with the information from the source otherwise
         */
        static UA_NodeId* parseNodeId(const char *paNodeIdString);

        /**
         * Parse the namespace of the node ID from the string source
         * @param paNamespace Source string containing the namespace
         * @param paResult Place to store the result
         * @return True if the source string has a valid namespace, false otherwise
         */
        static bool parseNamespace(const char *paNamespace, UA_NodeId &paResult);

        /**
         * Parse the identifier of the node ID from the string source. Allowed values for identifier are [i, s, g] (numeric, string and bytestring node ID type respectively). GUID type is not supported
         * @param paIdentifier Source string containing the identifier
         * @param paResult Place to store the result
         * @return True if the source string has a valid identifier, false otherwise
         */
        static bool parseIdentifier(const char *paIdentifier, UA_NodeId &paResult);

        enum NodePairPositions {
          eBrowseName = 0,
          eNodeId,
          eMaxNumberOfPositions
        };

        enum NodeIdPositions {
          eNamespace = 0,
          eIdenfier,
          eMaxNumberOfNodeIdPositions
        };

        enum NodeIdItenfierPositions {
          eIdenfierType = 0,
          eIdenfierValue,
          eMaxNumberOfNodeIdIdenfiertPositions
        };
    };
};

/**
 * Special case of an action because more information is needed
 */
class CLocalMethodInfo : public CActionInfo {
  public:

    /**
     * Constructor of the class
     * @param paLayer The layer that creates and executes the action
     * @param paEndpoint The endpoint of a remote OPC UA in case the action is to be executed remotely. An empty endpoint means that the action is to be executed locally
     * @param paTypes A list of type converters of the connections of the FB of the action (SDs/RDs)
     */
    explicit CLocalMethodInfo(COPC_UA_Layer &paLayer, const CIEC_STRING &paEndpoint);

    /**
     * Destructor of the class
     */
    ~CLocalMethodInfo();

    /**
     * Getter for the semaphore of the action
     * @return the semaphore of the action
     */
    forte::arch::CSemaphore& getResultReady();

  private:

    /**
     * Copy constructor is private and not defined to avoid its usage
     */
    CLocalMethodInfo(const CLocalMethodInfo &paObj);

    /**
     * Assignment operator is private and not defined to avoid its usage
     */
    CLocalMethodInfo& operator=(const CLocalMethodInfo &other);

    /**
     * When a method is called, it waits with this semaphore until the response comes back to the FB, when this semaphore is increased indicating the method has finished
     */
    forte::arch::CSemaphore mResultIsReady;
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_ */
