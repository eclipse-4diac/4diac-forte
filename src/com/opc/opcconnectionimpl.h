/*******************************************************************************
 * Copyright (c) 2012, 2024 AIT, fortiss GmbH, HIT robot group, Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - add the list of items instead of add item one by one
 *   Ketut Kumajaya - switch to OPCClientToolKit with 64bit support
 *******************************************************************************/
#ifndef OPCCONNECTIONIMPL_H_
#define OPCCONNECTIONIMPL_H_

#include "opcprocessvar.h"
// Includes from OPC library
#include "windows.h"
#include "opcda.h"
#include "OPCClient.h"
#include <vector>
#include <map>

class COpcConnection;

class COpcConnectionImpl : public IAsyncDataCallback{
  public:
    COpcConnectionImpl(const char *paHost, const char *paServerName, COpcConnection* paOpcConn);
    ~COpcConnectionImpl();

    bool connect(const char* paGroupName);
    void disconnect();
    void addItemList(const char* paGroupName,  std::vector<std::string> paReadItems,std::vector<std::string> paWriteItems);
    bool addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand);
    void removeGroup(const char* paGroupName);
    void removeItems(const char* paGroupName);


    int sendItemData(const char*paGroupName, const char* paItemName, Variant paVar);
    int receiveData(const char* paRecvBuffer);

    const char* getHost(){
      return mHost;
    }
    const char* getServerName(){
      return mServerName;
    }

    bool isConnected();

    virtual void COpcConnectionImpl::OnDataChange(COPCGroup &paGroup, COPCItemDataMap &paChanges);

  private:

    COPCGroup* getOpcGroup(const char* paGroupName, bool paIfRead);
    void clearGroup();

    typedef std::map<CString, std::vector<COPCItem*>>::iterator TOpcItemsIt;
    std::map<CString, std::vector<COPCItem*>> mOpcItems;

    struct SOpcGroupSettings{
        COPCGroup* mOpcGroupRead;
        COPCGroup* mOpcGroupWrite;
        const char* mGroupName;
        unsigned long mReqUpdateRate;
        unsigned long mRevisedUpdateRate;
        float mDeadBand;
        bool mReadGroupAdded;
        bool mWriteGroupAdded;

        SOpcGroupSettings(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand) :
          mOpcGroupRead(0), mOpcGroupWrite(0), mGroupName(paGroupName), mReqUpdateRate(paReqUpdateRate), mRevisedUpdateRate(0),
          mDeadBand(paDeadBand), mReadGroupAdded(false), mWriteGroupAdded(false) {
        }
    };

    typedef std::vector<SOpcGroupSettings*> TOpcGroupSettingsList;
    TOpcGroupSettingsList mOpcGroupSettingsList;

    COpcConnection* mOpcConn;

    COPCHost* mOpcHost;
    COPCServer *mOpcServer;

    const char* mHost;
    const char* mServerName;
    const char* mGroupName;
    unsigned long mReqUpdateRate;
    unsigned long mRealUpdateRate;
    float mDeadBand;
    bool mConnected;
    std::vector<std::string> mWrongItemList;
};

#endif // OPCCONNECTIONIMPL_H_
