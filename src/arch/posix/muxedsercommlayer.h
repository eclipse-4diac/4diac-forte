/*******************************************************************************
 * Copyright (c) 2012, 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _MUXEDSERCOMMLAYER_H_
#define _MUXEDSERCOMMLAYER_H_

#include "comlayer.h"
#include "fdselecthand.h"

class CMuxedSerCommLayer : public forte::com_infra::CComLayer{
  public:
    CMuxedSerCommLayer(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    virtual ~CMuxedSerCommLayer();

    virtual forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize);
    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);

    virtual forte::com_infra::EComResponse processInterrupt();

    TForteUInt8 getSerMuxId() const{
      return mSerMuxId;
    }

  protected:
  private:
    virtual forte::com_infra::EComResponse openConnection(char *paLayerParameter);
    virtual void closeConnection();

    forte::com_infra::EComResponse mInterruptResp;
    char mRecvBuffer[cg_unIPLayerRecvBufferSize];
    unsigned int mBufFillSize;
    TForteUInt8 mSerMuxId;
    CFDSelectHandler::TFileDescriptor mFD; //!< file descriptor for accessing the serial device

    class CMuxedSerPortsManager{

      public:

        CFDSelectHandler::TFileDescriptor addMuxedSerLayer(char* paSerPort, CMuxedSerCommLayer *paComCallBack);
        void removeMuxedSerLayer(CFDSelectHandler::TFileDescriptor paFD, CMuxedSerCommLayer *paComCallBack);

      private:

        typedef CSinglyLinkedList<CMuxedSerCommLayer *> TConnectionContainer;
        class SSerPortEntry : public forte::com_infra::CComLayer{
          public:
            SSerPortEntry(): forte::com_infra::CComLayer(0, 0), mSerPort(0), mFD(scm_nInvalidFileDescriptor){
            }

            char* mSerPort;
            CFDSelectHandler::TFileDescriptor mFD;
            TConnectionContainer mConnectionsList;

            virtual forte::com_infra::EComResponse sendData(void *, unsigned int ){
              return forte::com_infra::e_Nothing;
            }

            virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);
            virtual forte::com_infra::EComResponse processInterrupt();

          private:
            virtual forte::com_infra::EComResponse openConnection(char *){
              return forte::com_infra::e_Nothing;
            }

            virtual void closeConnection(){}
        };

        typedef CSinglyLinkedList<SSerPortEntry> TSerPortList;

        CMuxedSerPortsManager();
        SSerPortEntry *getSerPortEntry(char* paSerPort);
        SSerPortEntry *getOpendSerPortEntry(CFDSelectHandler::TFileDescriptor paFD);
        void openPort(char* paSerPort, SSerPortEntry *paPortEntry);
        void closePort(SSerPortEntry *paSerPortEntry);

        TSerPortList mPortList;

        CSyncObject mSync;

        //needed so that CLMSUSBLayer can have a static member variable holding the manager.
        friend class CMuxedSerCommLayer;

        //not to be implemented
        CMuxedSerPortsManager(const CMuxedSerPortsManager &);
        CMuxedSerPortsManager &operator =(const CMuxedSerPortsManager&);
    };

    static CMuxedSerPortsManager smMuxedSerPortsManager;
};

#endif /* MUXEDSERCOMMLAYER_H_ */
