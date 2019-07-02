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
#ifndef SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_
#define SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_

#include <forte_sem.h>
#include <forte_sync.h>
#include <extevhan.h>
#include <open62541.h>
#include <forte_string.h>
#include <fortelist.h>
#include <forte_string.h>
#include <basecommfb.h>

class COPC_UA_Layer;

class COPC_UA_HandlerAbstract : public CExternalEventHandler {
  public:
    explicit COPC_UA_HandlerAbstract(CDeviceExecution& paDeviceExecution);

    ~COPC_UA_HandlerAbstract();

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
        const struct UA_TypeConvert** mConvert;
      private:
        UA_Variables_handle(const UA_Variables_handle &obj);
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
        UA_RecvVariable_handle(const UA_RecvVariable_handle &obj);
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
        UA_SendVariable_handle(const UA_SendVariable_handle &obj);
        UA_SendVariable_handle& operator=(const UA_SendVariable_handle& other);
    };

    struct CNodePairInfo {
        UA_NodeId *mNodeId;
        CIEC_STRING mBrowsePath;

        CNodePairInfo(UA_NodeId * paNodeId, const char* paBrowsePath) :
            mNodeId(paNodeId), mBrowsePath(paBrowsePath) {
        }
    };

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

        explicit CActionInfo(COPC_UA_Layer* paLayer, UA_ActionType paAction, CIEC_STRING& paEndpoint);

        virtual ~CActionInfo();

        bool isRemote() {
          return ("" != mEndpoint);
        }

        CIEC_STRING& getEndpoint() {
          return mEndpoint;
        }

        COPC_UA_Layer* getLayer() {
          return mLayer;
        }

        CSinglyLinkedList<CNodePairInfo*>& getNodePairInfo() {
          return mNodePair;
        }

        UA_ActionType getAction() {
          return mAction;
        }

        size_t getNoOfNodePairs() {
          size_t noOfPairs = 0;
          for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CNodePairInfo*>::Iterator it = mNodePair.begin(); it != mNodePair.end(); ++it, noOfPairs++)
            ;
          return noOfPairs;
        }

        bool checkAction();

        static UA_ActionType getActionFromParams(const char * paParams);

        static const char* const mActionNames[eActionUnknown];

      private:

        UA_ActionType mAction;
        COPC_UA_Layer* mLayer;
        CIEC_STRING mEndpoint;
        CSinglyLinkedList<CNodePairInfo*> mNodePair;

        bool checkNodePairInfo();

        bool checkReadAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkWriteAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkCreateMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkCallMethodAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkSubscribeAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkCreateObjectAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
        bool checkDeleteObjectAction(forte::com_infra::EComServiceType paFbType, unsigned int paNoOfRDs, unsigned int paNoOfSDs);
    };

    class CLocalMethodInfo : public CActionInfo {
      public:
        explicit CLocalMethodInfo(COPC_UA_Layer* paLayer, CIEC_STRING& paEndpoint) :
            CActionInfo(paLayer, eCreateMethod, paEndpoint) {
        }

        CSyncObject& getMutex() {
          return mMutex;
        }

        forte::arch::CSemaphore& getResultReady() {
          return mResultIsReady;
        }

      private:
        CSyncObject mMutex;
        forte::arch::CSemaphore mResultIsReady;
    };

    virtual UA_StatusCode initializeAction(CActionInfo& paInfo) = 0;

    virtual UA_StatusCode executeAction(CActionInfo& paInfo) = 0;

    virtual UA_StatusCode uninitializeAction(CActionInfo& paInfo) = 0;

    static CActionInfo* getInfoFromParams(const char* paParams, COPC_UA_Layer* paLayer);

    class COPC_UA_Params {
      public:

        static CActionInfo* getInfoFromParams(const char* paParams, COPC_UA_Layer* paLayer);

      private:

        static bool handlePair(const char * paPair, CSinglyLinkedList<CNodePairInfo*>& paResult);

        static UA_NodeId* parseNodeId(const char* paNodeIdString);
        static bool parseNamespace(const char* paNamespace, UA_NodeId& paResult);
        static bool parseIdentifier(const char* paIdentifier, UA_NodeId& paResult);
    };

    void setPriority(int paPriority);

    int getPriority() const;

  protected:

    struct UA_NodeCallback_Handle {
        COPC_UA_Layer *mLayer;
    };

    struct UA_VariableCallback_Handle : UA_NodeCallback_Handle {
        const struct UA_TypeConvert *mConvert;
        size_t mPortIndex;
    };

    UA_Logger getLogger();

  private:

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    static void UA_Log_Forte_clear(void *paLogContext);
    static const UA_Logger UA_Forte_logger;
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

    static const size_t mMaxLogLenght = 400;

    static void UA_Log_Forte(
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        void*,
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
        UA_LogLevel paLevel, UA_LogCategory paCategory, const char *paMsg, va_list paArgs);
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_ */
