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
 *                  - Code refactoring from char* to std::string
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
    COpcConnectionImpl(const std::string& paHost, const std::string& paServerName, COpcConnection* paOpcConn);
    ~COpcConnectionImpl();

    bool connect(const std::string& paGroupName);
    void disconnect();
    void addItemList(const std::string& paGroupName, std::vector<std::string>& paReadItems, std::vector<std::string>& paWriteItems);
    bool addGroup(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand);
    void removeGroup(const std::string& paGroupName);
    void removeItems(const std::string& paGroupName);


    int sendItemData(const std::string& paGroupName, const std::string& paItemName, Variant paVar);

    bool isConnected();

    virtual void COpcConnectionImpl::OnDataChange(COPCGroup &paGroup, COPCItemDataMap &paChanges);

  private:

    COPCGroup* getOpcGroup(const std::string& paGroupName, bool paIfRead);
    void clearGroup();

    typedef std::map<std::string, std::vector<COPCItem*>>::iterator TOpcItemsIt;
    std::map<std::string, std::vector<COPCItem*>> mOpcItems;

    struct SOpcGroupSettings{
        COPCGroup* mOpcGroupRead;
        COPCGroup* mOpcGroupWrite;
        const std::string mGroupName;
        unsigned long mReqUpdateRate;
        unsigned long mRevisedUpdateRate;
        float mDeadBand;
        bool mReadGroupAdded;
        bool mWriteGroupAdded;

        SOpcGroupSettings(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand) :
          mOpcGroupRead(0), mOpcGroupWrite(0), mGroupName(paGroupName), mReqUpdateRate(paReqUpdateRate), mRevisedUpdateRate(0),
          mDeadBand(paDeadBand), mReadGroupAdded(false), mWriteGroupAdded(false) {
        }
    };

    typedef std::vector<SOpcGroupSettings*> TOpcGroupSettingsList;
    TOpcGroupSettingsList mOpcGroupSettingsList;

    COpcConnection* mOpcConn;

    COPCHost* mOpcHost;
    COPCServer *mOpcServer;

    const std::string mHost;
    const std::string mServerName;
    bool mConnected;
};

#endif // OPCCONNECTIONIMPL_H_
