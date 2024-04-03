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
 *                  - Code refactoring from char* to std::string
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

    explicit SOpcItemData(const std::string& paItemName) :
        mItemName(paItemName){
      mItemData.set<SHORT>(0);
    }
    SOpcItemData(const std::string& paItemName, Variant paItemData) :
        mItemName(paItemName), mItemData(paItemData){
    }
};
typedef std::vector<SOpcItemData*> TItemDataList;


class COpcConnection{
  public:

    COpcConnection(const std::string& paHost, const std::string& paServerName, COpcEventHandler* paEventHandler);
    ~COpcConnection();

    /*** Functions for OpcConnectionHandler ****************************************/
    void addGroup(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand, forte::com_infra::CComLayer* paComCallback);
    void removeGroup(const std::string& paGroupName);
    /*** END ***********************************************************************/


    /*** Functions for OpcComLayer *************************************************/
    typedef std::vector<COpcProcessVar*> TOpcProcessVarList;
    int send_connect(const std::string& acGroupName, unsigned long nReqUpdateRate, float nDeadBand, forte::com_infra::CComLayer *paComCallback,
                     TOpcProcessVarList paNewItems);
    int send_sendItemData(COpcProcessVar *paItem);
    int send_disconnect(const std::string& paGroupName);

    int receiveData(const std::string& paGroupName, TOpcProcessVarList * paOpcProcessVarList);

    bool isConnected() {
      return mConnected;
    }
    /*** END ***********************************************************************/

    /*** Functions for OpcConnectionImpl *******************************************/
    void response_connect(bool paConnectionState, const std::string& paGroupName);
    void response_dataReceived(const std::string& paGroupName, TItemDataList & paItemDataList);
    void response_dataSent(const std::string& paGroupName,const std::string& paItemName, bool paSuccess);
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

    const std::string getHost() const {
      return mHost;
    }

    const std::string getServerName() const {
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
        const std::string mGroupName;
        int mCallbackDesc;
        TItemDataList mReadItemsList;
        TItemDataList mWriteItemsList;

        SOpcGroupMap(const std::string& paGroupName, int paCallbackDesc) :
            mGroupName(paGroupName), mCallbackDesc(paCallbackDesc){
        }
    };

    int getGroupCallbackDesc(const std::string& paGroupName){
      for(unsigned int i = 0; i < mOpcGroupMapList.size(); i++){
        if(mOpcGroupMapList[i]->mGroupName == paGroupName){
          return mOpcGroupMapList[i]->mCallbackDesc;
        }
      }
      return -1;
    }

    int removeGroupCallbackDesc(const std::string& paGroupName);
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

    bool ifInGroupList(const std::string& paGroupName);

    bool ifLetEventPass(EOpcConnectionEvents paEvent, const std::string& paGroupName);

    typedef std::vector<SOpcGroupMap*> TOpcGroupMapList;
    TOpcGroupMapList mOpcGroupMapList;

    unsigned int mGroupCount;

    COpcConnectionImpl *mOpcConnectionImpl;

    EOpcConnectionState mConnectionState;

    SLastHappenedEvent mLastEvent;

    const std::string mHost;
    const std::string mServerName;

    bool mConnected;
    bool mBlockingConnect;

    CSyncObject mSync;

    COpcEventHandler* mEventHandler;

    //we don't want COpcConnection to be copy and assignable
    COpcConnection(const COpcConnection&);
    COpcConnection& operator = (const COpcConnection &);
};

#endif // OPCCONNECTION_H_
