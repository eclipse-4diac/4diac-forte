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

        ~UA_Variables_handle() {
          delete[] mConvert;
        }


        bool mFailed;
        size_t mOffset;
        size_t mSize;
        const struct UA_TypeConvert** mConvert;
      private:
        UA_Variables_handle(const UA_Variables_handle &obj);
    };

    class UA_RecvVariable_handle : public UA_Variables_handle {
      public:
        explicit UA_RecvVariable_handle(size_t paSize) :
            UA_Variables_handle(paSize) {
          mData = new const UA_Variant*[mSize];

        }

        ~UA_RecvVariable_handle() {
          delete[] mData;
        }

        const UA_Variant **mData;
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
    };

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

    static const char* const mActionNames[eActionUnknown];

    struct CNodePairInfo {
        UA_NodeId *mNodeId;
        CIEC_STRING mBrowsePath;

        CNodePairInfo(UA_NodeId * paNodeId, const char* paBrowsePath) :
            mNodeId(paNodeId), mBrowsePath(paBrowsePath) {
        }
    };

    class CActionInfo {
      public:
        explicit CActionInfo(COPC_UA_Layer* paLayer) :
            mAction(eActionUnknown), mLayer(paLayer) {
        }

        ~CActionInfo() {
          for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = mNodePair.begin(); it != mNodePair.end(); ++it) {
            if((*it)->mNodeId) {
              UA_NodeId_deleteMembers((*it)->mNodeId);
              UA_NodeId_delete((*it)->mNodeId);
            }
            delete (*it);
          }
          mNodePair.clearAll();
        }

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

        COPC_UA_HandlerAbstract::UA_ActionType getAction() {
          return mAction;
        }

        size_t getNoOfNodePairs() {
          size_t noOfPairs = 0;
          for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CNodePairInfo*>::Iterator it = mNodePair.begin(); it != mNodePair.end(); ++it, noOfPairs++)
            ;
          return noOfPairs;
        }

        COPC_UA_HandlerAbstract::UA_ActionType mAction;
        COPC_UA_Layer* mLayer;
        CIEC_STRING mEndpoint;
        CSinglyLinkedList<CNodePairInfo*> mNodePair;
    };

    class CLocalMethodInfo : public CActionInfo {
      public:
        explicit CLocalMethodInfo(COPC_UA_Layer* paLayer) :
            CActionInfo(paLayer) {
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

        static COPC_UA_HandlerAbstract::UA_ActionType getActionFromParams(const char * paParams);

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

    static bool checkNodePairInfo(COPC_UA_HandlerAbstract::CActionInfo& paResult);
    static bool checkAction(COPC_UA_HandlerAbstract::CActionInfo& paResult);
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_ */
