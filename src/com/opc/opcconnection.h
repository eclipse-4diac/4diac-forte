/*******************************************************************************
 * Copyright (c) 2012, 2024 AIT, ACIN,fortiss GmbH, Hit robot group, Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - Guard the opc connection from requests from FBs,filter non-sense requests 
 *   Ketut Kumajaya - switch to OPCClientToolKit with 64bit support
 *******************************************************************************/
#ifndef OPCCONNECTION_H_
#define OPCCONNECTION_H_

#include "comlayer.h"
#include "forte_sync.h"
#include "windows.h"
#include "Variant.h"
#include <vector>

class COpcConnectionImpl;
class COpcProcessVar;
class COpcEventHandler;

struct SOpcItemData{
    const std::string mItemName;
    Variant mItemData;

    explicit SOpcItemData(const std::string paItemName) :
        mItemName(paItemName){
      mItemData.set<SHORT>(0);
    }
    SOpcItemData(const std::string paItemName, Variant paItemData) :
        mItemName(paItemName), mItemData(paItemData){
    }
};
typedef std::vector<SOpcItemData*> TItemDataList;


class COpcConnection{
  public:

    COpcConnection(const char *paHost, const char *paServerName, COpcEventHandler* paEventHandler);
    ~COpcConnection();

    /*** Functions for OpcConnectionHandler ****************************************/
    void addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand, forte::com_infra::CComLayer* paComCallback);
    void removeGroup(const char* paGroupName);
    /*** END ***********************************************************************/


    /*** Functions for OpcComLayer *************************************************/
    typedef std::vector<COpcProcessVar*> TOpcProcessVarList;
    int send_connect(const char *acGroupName, unsigned long nReqUpdateRate, float nDeadBand, forte::com_infra::CComLayer *paComCallback,
                     TOpcProcessVarList paNewItems);
    int send_sendItemData(COpcProcessVar *paItem);
    int send_disconnect(const char *paGroupName);

    int receiveData(const char *paGroupName, TOpcProcessVarList * paOpcProcessVarList);

    bool isConnected() {
      return mConnected;
    }
    /*** END ***********************************************************************/

    /*** Functions for OpcConnectionImpl *******************************************/
    void response_connect(bool paConnectionState, const char* paGroupName);
    void response_dataReceived(const char *paGroupName, TItemDataList & paItemDataList);
    void response_dataSent(const char* paGroupName,const char* paItemName, bool paSuccess);
    /*** END ***********************************************************************/

    /*** Common Functions **********************************************************/
    enum EOpcConnectionEvents{
      e_Idle, e_Disconnect, e_Connect, e_DataReceive, e_ItemSend
    };

    enum EOpcConnectionState{
      e_Connected, e_Disconnected, e_Connecting, e_Disconnecting
    };

    struct SLastHappenedEvent{
      EOpcConnectionEvents mLastHappenedEvent;
      bool mSuccess;
    };

    const char* getHost() const {
      return mHost;
    }

    const char* getServerName() const {
      return mServerName;
    }

    unsigned int getGroupCount() const {
      return mGroupCount;
    }

    EOpcConnectionState getConnectionState() const {
      return mConnectionState;
    }

    SLastHappenedEvent getLastEvent() const {
      return mLastEvent;
    }

    /*** END ***********************************************************************/

  private:
    int maintainGroupMapInfo(COpcProcessVar* paNewItem);

    struct SOpcGroupMap{
        const char* mGroupName;
        int mCallbackDesc;
        TItemDataList mReadItemsList;
        TItemDataList mWriteItemsList;

        SOpcGroupMap(const char* paGroupName, int paCallbackDesc) :
            mGroupName(paGroupName), mCallbackDesc(paCallbackDesc){
        }
    };

    int getGroupCallbackDesc(const char * paGroupName){
      for(unsigned int i = 0; i < mOpcGroupMapList.size(); i++){
        if(mOpcGroupMapList[i]->mGroupName == paGroupName){
          return mOpcGroupMapList[i]->mCallbackDesc;
        }
      }
      return -1;
    }

    int removeGroupCallbackDesc(const char * paGroupName);
    //to see if could send command to opc event handler
    void maintainStateinEventResponse(EOpcConnectionEvents paEvent, bool paSuccess){
      //let's transfer ongoing state to steady state(such as connecting => connected)
      if(e_Connect == paEvent){
        mLastEvent.mLastHappenedEvent = e_Connect;
        mLastEvent.mSuccess = paSuccess;

        if(paSuccess){
          mConnectionState = e_Connected;
        }
        else{
          mConnectionState = e_Disconnected;
        }

      }
      else if(e_Disconnect == paEvent){
        mLastEvent.mLastHappenedEvent = e_Disconnect;
        mLastEvent.mSuccess = paSuccess;
        if(paSuccess){
          mConnectionState = e_Disconnected;
        }
        else{
          mConnectionState = e_Connected;
        }
      }
      else if(e_DataReceive == paEvent){
        mLastEvent.mLastHappenedEvent = e_DataReceive;
        mLastEvent.mSuccess = paSuccess;
        mConnectionState = e_Connected;
      }
      else if(e_ItemSend == paEvent){
        mLastEvent.mLastHappenedEvent = e_ItemSend;
        mLastEvent.mSuccess = paSuccess;
        mConnectionState = e_Connected;
      }
    }

    bool ifInGroupList(const char* paGroupName);

    bool ifLetEventPass(EOpcConnectionEvents paEvent, const char* paGroupName);

    typedef std::vector<SOpcGroupMap*> TOpcGroupMapList;
    TOpcGroupMapList mOpcGroupMapList;

    unsigned int mGroupCount;

    COpcConnectionImpl *mOpcConnectionImpl;

    EOpcConnectionState mConnectionState;

    SLastHappenedEvent mLastEvent;

    const char* mHost;
    const char* mServerName;
    const char* mGroupName;
    unsigned long mReqUpdateRate;
    unsigned long mRealUpdateRate;
    float mDeadBand;

    bool mConnected;
    bool mBlockingConnect;

    CSyncObject mSync;

    COpcEventHandler* mEventHandler;

    //we don't want COpcConnection to be copy and assignable
    COpcConnection(const COpcConnection&);
    COpcConnection& operator = (const COpcConnection &);
};

#endif // OPCCONNECTION_H_
