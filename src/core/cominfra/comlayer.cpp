/*******************************************************************************
  * Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation and rework communication infrastructure
  *    Michael Hofmann - rework communication infrastructure
  *******************************************************************************/
#include "comlayer.h"
#include "comlayersmanager.h"
#include "commfb.h"

using namespace forte::com_infra;

char *CComLayer::extractLayerIdAndParams(char *pa_acID, char **pa_pacParameter){
  char *acRunner = strchr(pa_acID, '[');
  if(0 != acRunner){
    *acRunner = '\0';
    ++acRunner;
    *pa_pacParameter = acRunner;
    acRunner = strchr(acRunner, ']');
    if(0 != acRunner){
      *acRunner = '\0';
      ++acRunner;

      if('\0' != *acRunner){
        //this is not the end of the string
        ++acRunner; //move the pointer beyond the separating '.'
      }
    }
  }
  return acRunner;
}

CComLayer::CComLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poComFB) :
  m_eConnectionState(forte::com_infra::e_Disconnected), m_poTopLayer(pa_poUpperLayer), m_poBottomLayer(0), m_poFb(pa_poComFB){

}

CComLayer::~CComLayer(){
  delete m_poBottomLayer;
}

EComResponse CComLayer::openConnection(char *pa_acConnectionParams, char *pa_acRemainingConnectionID){
  EComResponse eRetVal = openConnection(pa_acConnectionParams);
  if((!(scg_unComNegative & eRetVal)) && (0 != pa_acRemainingConnectionID) && ('\0' != *pa_acRemainingConnectionID)){
    //There is a connection ID left
    char *acRemainingConnectionID = extractLayerIdAndParams(pa_acRemainingConnectionID, &pa_acConnectionParams);
    if('\0' != *pa_acRemainingConnectionID){
      if(m_poBottomLayer == 0){
        m_poBottomLayer = CComLayersManager::createCommunicationLayer(pa_acRemainingConnectionID, this, m_poFb); // create bottom layer
        if(m_poBottomLayer == 0){
          eRetVal = e_InitInvalidId;
          return eRetVal;
        }
      }
      eRetVal = m_poBottomLayer->openConnection(pa_acConnectionParams, acRemainingConnectionID);
    }
    else{
      eRetVal = e_InitInvalidId;
    }
  }

  return eRetVal;
}

void CComLayer::rCloseConnection(){
  if(m_poBottomLayer != 0){
    m_poBottomLayer->rCloseConnection();
    delete m_poBottomLayer; // this will close the whole communication stack
    m_poBottomLayer = 0;
  }
  closeConnection();
}

EComResponse CComLayer::processInterrupt(){
  return e_Nothing;
}
