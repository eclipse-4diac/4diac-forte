/*******************************************************************************
  * Copyright (c) 2013-2015 TU Wien ACIN and fortiss.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Martin Melik Merkumians, Monika Wenger 
  *     - initial implementation
  *******************************************************************************/
#include "rawdatacomlayer.h"
#include <forte_any.h>
#include <forte_string.h>

namespace forte {
  namespace com_infra {

    CRawDataComLayer::CRawDataComLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poFB) : CComLayer(pa_poUpperLayer, pa_poFB){
      m_cTerminatorSybmol = '\n';
    }

    CRawDataComLayer::~CRawDataComLayer(){
    }

    void CRawDataComLayer::closeConnection(){
      if(0 != m_poBottomLayer){
        m_poBottomLayer->closeConnection();
      }
    }

    EComResponse CRawDataComLayer::sendData( void *pa_pvData, unsigned int){
      temp.clear();
      TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(pa_pvData);
      temp.reserve(static_cast<TForteUInt16>(static_cast<const CIEC_STRING&>(apoSDs[0]).length() + 1));
      temp = static_cast<const CIEC_STRING&>(apoSDs[0]);
      temp.append(const_cast<char*>(&m_cTerminatorSybmol),1);
      m_poBottomLayer->sendData(reinterpret_cast<void*>(temp.getValue()), temp.length());
      return e_ProcessDataOk;
    }

    EComResponse CRawDataComLayer::recvData( const void *pa_pvData, unsigned int pa_unSize){
      if (0 == m_poTopLayer && m_poFb->getNumRD() == 1){
        m_poFb->getRDs()[0].saveAssign(CIEC_STRING(static_cast<const char*>(pa_pvData)));
      }
      return e_ProcessDataOk;
    }

    EComResponse CRawDataComLayer::openConnection(char *){
          switch (m_poFb->getComServiceType()){
            case e_Client:
            case e_Publisher:
              if(m_poFb->getNumSD() != 1 || (m_poFb->getNumSD() > 0 && CIEC_ANY::e_STRING  != m_poFb->getSDs()[0].getDataTypeID())){
                return e_InitTerminated;
              }
              break;
            case e_Server:
            case e_Subscriber:
              if(m_poFb->getNumRD() != 1 || (m_poFb->getNumRD() > 0 && CIEC_ANY::e_STRING  != m_poFb->getRDs()[0].getDataTypeID())){
                return e_InitTerminated;
              }
              break;
          }
          return e_InitOk;
        }
  } /* namespace com_infra */
} /* namespace forte */
