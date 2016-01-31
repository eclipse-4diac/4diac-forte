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
    CMuxedSerCommLayer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CCommFB * pa_poFB);
    virtual ~CMuxedSerCommLayer();

    virtual void closeConnection();
    virtual forte::com_infra::EComResponse sendData(void *pa_pvData, unsigned int pa_unSize);
    virtual forte::com_infra::EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

    virtual forte::com_infra::EComResponse processInterrupt();

    TForteUInt8 getSerMuxId() const{
      return m_unSerMuxId;
    }

  protected:
  private:
    virtual forte::com_infra::EComResponse openConnection(char *pa_acLayerParameter);

    forte::com_infra::EComResponse m_eInterruptResp;
    char m_acRecvBuffer[cg_unIPLayerRecvBufferSize];
    unsigned int m_unBufFillSize;
    TForteUInt8 m_unSerMuxId;
    CFDSelectHandler::TFileDescriptor m_nFD; //!< file descriptor for accessing the serial device

    class CMuxedSerPortsManager{

      public:

        CFDSelectHandler::TFileDescriptor addMuxedSerLayer(char* pa_acSerPort, CMuxedSerCommLayer *pa_poComCallBack);
        void removeMuxedSerLayer(CFDSelectHandler::TFileDescriptor m_nFD, CMuxedSerCommLayer *pa_poComCallBack);

      private:

        typedef CSinglyLinkedList<CMuxedSerCommLayer *> TConnectionContainer;
        class SSerPortEntry : public forte::com_infra::CComLayer{
          public:
            SSerPortEntry(): forte::com_infra::CComLayer(0, 0){
            }

            char* m_acSerPort;
            CFDSelectHandler::TFileDescriptor m_nFD;
            TConnectionContainer m_lstConnectionsList;

            virtual void closeConnection(){}

            virtual forte::com_infra::EComResponse sendData(void *, unsigned int ){
              return forte::com_infra::e_Nothing;
            }

            virtual forte::com_infra::EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);
            virtual forte::com_infra::EComResponse processInterrupt();

          private:
            virtual forte::com_infra::EComResponse openConnection(char *){
              return forte::com_infra::e_Nothing;
            }
        };

        typedef CSinglyLinkedList<SSerPortEntry> TSerPortList;

        CMuxedSerPortsManager();
        SSerPortEntry *getSerPortEntry(char* pa_acSerPort);
        SSerPortEntry *getOpendSerPortEntry(CFDSelectHandler::TFileDescriptor pa_nFD);
        void openPort(char* pa_acSerPort, SSerPortEntry *pa_pstPortEntry);
        void closePort(SSerPortEntry *pa_pstSerPortEntry);

        TSerPortList m_lstPortList;

        CSyncObject m_oSync;

        //needed so that CLMSUSBLayer can have a static member variable holding the manager.
        friend class CMuxedSerCommLayer;

        //not to be implemented
        CMuxedSerPortsManager(const CMuxedSerPortsManager &);
        CMuxedSerPortsManager &operator =(const CMuxedSerPortsManager&);
    };

    static CMuxedSerPortsManager sm_oMuxedSerPortsManager;
};

#endif /* MUXEDSERCOMMLAYER_H_ */
