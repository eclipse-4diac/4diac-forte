/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef OPCCONNECTION_H_
#define OPCCONNECTION_H_

#include "comlayer.h"
#include "fortelist.h"
#include <forte_sync.h>
#include "windows.h"
#include "Variant.h"

class COpcConnectionImpl;
class COpcProcessVar;
class COpcEventHandler;

struct SOpcItemData{
    const char* mItemName;
    Variant mItemData;

    explicit SOpcItemData(const char* paItemName) :
        mItemName(paItemName){
      mItemData.set<SHORT>(0);
    }
    SOpcItemData(const char* paItemName, Variant paItemData) :
        mItemName(paItemName), mItemData(paItemData){
    }
};
typedef CSinglyLinkedList<SOpcItemData*> TItemDataList;

class COpcConnection{
  public:

    COpcConnection(const char *paHost, const char *paServerName, COpcEventHandler* pa_eventHandler);
    ~COpcConnection();

    /*** Functions for OpcConnectionHandler ****************************************/
    void addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand, forte::com_infra::CComLayer* paComCallback);
    void removeGroup(const char* paGroupName);
    /*** END ***********************************************************************/

    /*** Functions for OpcComLayer *************************************************/
    int send_connect();
    int send_connect(bool paBlocking);
    int send_addItem(COpcProcessVar* paNewItem);
    int send_sendItemData(COpcProcessVar* paItem);

    typedef CSinglyLinkedList<COpcProcessVar*> TOpcProcessVarList;
    int receiveData(const char* paGroupName, TOpcProcessVarList * paOpcProcessVarList);

    bool isConnected() const {
      return mIsConnected;
    }
    /*** END ***********************************************************************/

    /*** Functions for OpcConnectionImpl *******************************************/
    void response_connect(bool paConnectionState);
    void response_dataReceived(const char *paGroupName, TItemDataList & paItemDataList);
    void response_itemAdded(COpcProcessVar* paOpcItem);
    /*** END ***********************************************************************/

    /*** Common Functions **********************************************************/
    enum EOpcConnectionEvents{
      e_Disconnected, e_Connecting, e_ConnectionFailed, e_Connected, e_ItemAddedOk, e_ItemAddedFailed, e_DataReceived
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

    EOpcConnectionEvents getConnectionState() const {
      return mConnectionEvent;
    }

    /*** END ***********************************************************************/

  private:
    struct SOpcGroupMap{
        const char* mGroupName;
        int mCallbackDesc;
        TItemDataList mReadItemsList;
        TItemDataList mWriteItemsList;

        SOpcGroupMap(const char* paGroupName, int paCallbackDesc) :
            mGroupName(paGroupName), mCallbackDesc(paCallbackDesc){
        }
    };

    typedef CSinglyLinkedList<SOpcGroupMap*> TOpcGroupMapList;
    TOpcGroupMapList mOpcGroupMapList;

    unsigned int mGroupCount;

    COpcConnectionImpl *mOpcConnectionImpl;

    EOpcConnectionEvents mConnectionEvent;

    const char* mHost;
    const char* mServerName;
    const char* mGroupName;
    unsigned long mReqUpdateRate;
    unsigned long mRealUpdateRate;
    float mDeadBand;

    bool mIsConnected;
    bool mBlockingConnect;

    CSyncObject mSync;

    COpcEventHandler* m_eventHandler;

    //we don't want COpcConnection to be copy and assignable
    COpcConnection(const COpcConnection&);
    COpcConnection& operator = (const COpcConnection &);
};

#endif // OPCCONNECTION_H_
