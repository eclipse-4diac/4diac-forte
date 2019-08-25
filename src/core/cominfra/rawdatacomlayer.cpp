/*******************************************************************************
 * Copyright (c) 2013-2015 TU Wien ACIN and fortiss.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

    CRawDataComLayer::CRawDataComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB) :
            CComLayer(pa_poUpperLayer, pa_poFB){
    }

    CRawDataComLayer::~CRawDataComLayer(){
    }

    void CRawDataComLayer::closeConnection(){
      //We don't need to do anything specific on closing
    }

    EComResponse CRawDataComLayer::sendData( void *paData, unsigned int){
      TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
      const CIEC_STRING &val(static_cast<const CIEC_STRING&>(apoSDs[0]));
      m_poBottomLayer->sendData((void*)val.getValue(), val.length());
      return e_ProcessDataOk;
    }

    EComResponse CRawDataComLayer::recvData( const void *paData, unsigned int paSize){
      if (0 == m_poTopLayer && m_poFb->getNumRD() == 1){
        TIEC_ANYPtr apoRDs = static_cast<TIEC_ANYPtr>(m_poFb->getRDs());
        CIEC_STRING &val(static_cast<CIEC_STRING&>(apoRDs[0]));
        val.assign(static_cast<const char *>(paData), static_cast<TForteUInt16>(paSize));
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
