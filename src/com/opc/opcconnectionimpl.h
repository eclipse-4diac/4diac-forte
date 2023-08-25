/*******************************************************************************
 * Copyright (c) 2012, 2015 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef OPCCONNECTIONIMPL_H_
#define OPCCONNECTIONIMPL_H_

#include "fortelist.h"

#include "opcprocessvar.h"

// Includes from OPC library
#include "windows.h"
#include "opcda.h"
#include "OPCClient.h"

class COpcConnection;

class COpcConnectionImpl : public IAsynchDataCallback{
  public:
    COpcConnectionImpl(const char *paHost, const char *paServerName, COpcConnection* paOpcConn);
    ~COpcConnectionImpl();

    void connect();
    void addItem(COpcProcessVar* paNewItem);
    void addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand);

    int sendItemData(COpcProcessVar* paNewItem);
    int receiveData(const char* paRecvBuffer);

    const char* getHost(){
      return mHost;
    }
    const char* getServerName(){
      return mServerName;
    }

    virtual void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes);

  private:
    COPCGroup* getOpcGroup(const char *paGroupName, COpcProcessVar::EOpcProcessVarFunctions paFunction);

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
          mOpcGroupRead(0), mOpcGroupWrite(0), mGroupName(paGroupName), mReqUpdateRate(paReqUpdateRate), mRevisedUpdateRate(0), mDeadBand(paDeadBand), mReadGroupAdded(false), mWriteGroupAdded(false){
        }
    };

    typedef CSinglyLinkedList<SOpcGroupSettings*> TOpcGroupSettingsList;
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
};

#endif // OPCCONNECTIONIMPL_H_
