/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_
#define SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_

#include "opcua_layer.h"
#include <forte_sem.h>

class CActionInfo {
  public:
    enum UA_ActionType {
      eRead,
      eWrite,
      eCreateMethod,
      eCallMethod,
      eSubscribe,
      eCreateObject,
      eDeleteObject,
      eActionUnknown,
    };

    struct CNodePairInfo {
        CNodePairInfo(UA_NodeId * paNodeId, const char* paBrowsePath) :
            mNodeId(paNodeId), mBrowsePath(paBrowsePath) {
        }
        UA_NodeId *mNodeId;
        CIEC_STRING mBrowsePath;
    };

    explicit CActionInfo(COPC_UA_Layer &paLayer, UA_ActionType paAction, CIEC_STRING& paEndpoint);

    virtual ~CActionInfo();

    UA_ActionType getAction() {
      return mAction;
    }

    COPC_UA_Layer& getLayer() {
      return mLayer;
    }

    CIEC_STRING& getEndpoint() {
      return mEndpoint;
    }

    CSinglyLinkedList<CNodePairInfo*>& getNodePairInfo() {
      return mNodePair;
    }

    bool isRemote() {
      return ("" != mEndpoint);
    }

    size_t getNoOfNodePairs() {
      size_t noOfPairs = 0;
      for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = mNodePair.begin(); it != mNodePair.end(); ++it, noOfPairs++)
        ;
      return noOfPairs;
    }

    static CActionInfo* getActionInfoFromParams(char* paParams, COPC_UA_Layer& paLayer);

    static const char* const mActionNames[eActionUnknown];

  private:
    CActionInfo(const CActionInfo &paObj);
    CActionInfo& operator=(const CActionInfo& other);

    bool checkAction();

    bool checkNodePairInfo();

    bool checkReadAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkWriteAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkCreateMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkCallMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkSubscribeAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkCreateObjectAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    bool checkDeleteObjectAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);

    UA_ActionType mAction;
    COPC_UA_Layer& mLayer;
    CIEC_STRING mEndpoint;
    CSinglyLinkedList<CNodePairInfo*> mNodePair;

    class CActionParser {
      public:
        static UA_ActionType getActionEnum(const char *paParams);

        static bool getEndpoint(const char* paEndpoint, CIEC_STRING& paResult);

        static bool handlePair(const char * paPair, CSinglyLinkedList<CNodePairInfo*>& paResult);

      private:

        static UA_NodeId* parseNodeId(const char* paNodeIdString);
        static bool parseNamespace(const char* paNamespace, UA_NodeId& paResult);
        static bool parseIdentifier(const char* paIdentifier, UA_NodeId& paResult);
    };
};

class CLocalMethodInfo : public CActionInfo {
  public:
    explicit CLocalMethodInfo(COPC_UA_Layer& paLayer, CIEC_STRING& paEndpoint);

    ~CLocalMethodInfo();

    CSyncObject& getMutex();

    forte::arch::CSemaphore& getResultReady();

  private:
    CLocalMethodInfo(const CLocalMethodInfo &paObj);
    CLocalMethodInfo& operator=(const CLocalMethodInfo& other);

    CSyncObject mMutex;
    forte::arch::CSemaphore mResultIsReady;
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_ACTION_INFO_H_ */
