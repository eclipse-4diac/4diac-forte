/*******************************************************************************
 * Copyright (c) 2013-2015 TU Wien ACIN and fortiss.
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#include "rawdatacomlayer.h"
#include <forte_any.h>
#include <forte_string.h>

namespace forte {
  namespace com_infra {

    CRawDataComLayer::CRawDataComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB) :
            CComLayer(paUpperLayer, paFB){
    }

    CRawDataComLayer::~CRawDataComLayer() = default;

    void CRawDataComLayer::closeConnection(){
      //We don't need to do anything specific on closing
    }

    EComResponse CRawDataComLayer::sendData( void *paData, unsigned int){
      TConstIEC_ANYPtr *apoSDs = static_cast<TConstIEC_ANYPtr *>(paData);
      const CIEC_STRING &val(static_cast<const CIEC_STRING&>(*apoSDs[0]));
      mBottomLayer->sendData((void*)val.getStorage().c_str(), val.length());
      return e_ProcessDataOk;
    }

    EComResponse CRawDataComLayer::recvData( const void *paData, unsigned int paSize){
      if (nullptr == mTopLayer && mFb->getNumRD() == 1){
        TIEC_ANYPtr *apoRDs = static_cast<TIEC_ANYPtr *>(mFb->getRDs());
        CIEC_STRING &val(static_cast<CIEC_STRING&>(*apoRDs[0]));
        val.assign(static_cast<const char *>(paData), static_cast<TForteUInt16>(paSize));
      }
      return e_ProcessDataOk;
    }

    EComResponse CRawDataComLayer::openConnection(char *){
          switch (mFb->getComServiceType()){
            case e_Client:
            case e_Publisher:
              if(mFb->getNumSD() != 1 || (mFb->getNumSD() > 0 && CIEC_ANY::e_STRING  != mFb->getSDs()[0]->getDataTypeID())){
                return e_InitTerminated;
              }
              break;
            case e_Server:
            case e_Subscriber:
              if(mFb->getNumRD() != 1 || (mFb->getNumRD() > 0 && CIEC_ANY::e_STRING  != mFb->getRDs()[0]->getDataTypeID())){
                return e_InitTerminated;
              }
              break;
          }
          return e_InitOk;
        }
  } /* namespace com_infra */
} /* namespace forte */
