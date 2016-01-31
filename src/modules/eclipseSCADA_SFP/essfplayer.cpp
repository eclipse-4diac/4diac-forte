/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "essfplayer.h"
#include "essfphandler.h"
#include <commfb.h>

using namespace forte::com_infra;

const char CES_SFP_Layer::scmParameterSeperator;

CES_SFP_Layer::CES_SFP_Layer(CComLayer* paUpperLayer, CCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mSFPItem(0){
}

CES_SFP_Layer::~CES_SFP_Layer(){
  closeConnection();
}


void CES_SFP_Layer::closeConnection(){
  if(0 != mSFPItem){
    int numData = (e_Subscriber == getCommFB()->getComServiceType()) ?
                    getCommFB()->getNumRD() : getCommFB()->getNumSD();

    if(0 == numData){
      numData = 1;  //remove the item used for the event transmition
    }

    for(int i = 0; i < numData; i++){
      if(0 != mSFPItem[i]){
        CEclipseSCADASFPHandler::getInstance().unregisterDataPoint(mSFPItem[i]);
      }
    }

    delete[] mSFPItem;
    mSFPItem = 0;
  }
}

EComResponse CES_SFP_Layer::sendData(void *paData, unsigned int paSize){
  EComResponse retVal = e_ProcessDataOk;

  if(0 == paSize){
    //TODO change to an update now with out the need for a new allocation
    sfp_item_update_data_allocated(*mSFPItem, sfp_variant_new_null(), sfp_time_in_millis ());
  }else {
    CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(paData));
    for(unsigned int i = 0; i < paSize; i++){
      CEclipseSCADASFPHandler::updateDataPoint(mSFPItem[i], SDs[i]);  //TODO use common timestamp for all
    }
  }
  return retVal;
}

EComResponse CES_SFP_Layer::recvData(const void *paData, unsigned int ){
  mInterruptResp = e_ProcessDataOk;
  const struct sfp_variant *value = static_cast<const struct sfp_variant *>(paData);

  if(0 == getCommFB()->getNumRD()){
    //we are a subscribe 0
    if(VT_NULL != value->type){
      mInterruptResp = e_ProcessDataRecvFaild;
    }
  }else{
    CIEC_ANY &RD1(*getCommFB()->getRDs());
    if(!CEclipseSCADASFPHandler::readBackDataPoint(value, RD1)){
      mInterruptResp = e_ProcessDataRecvFaild;
    }
  }

  getCommFB()->interruptCommFB(this);
  return mInterruptResp;
}

EComResponse CES_SFP_Layer::processInterrupt(){
  //we don't need to do anything here
  return mInterruptResp;
}

EComResponse CES_SFP_Layer::openConnection(char *paLayerParameter){
  int numData;
  CIEC_ANY *dataArray;

  if(e_Subscriber == getCommFB()->getComServiceType()){
    numData = getCommFB()->getNumRD();
    dataArray = getCommFB()->getRDs();
  }else{
    numData = getCommFB()->getNumSD();
    dataArray = getCommFB()->getSDs();
  }

  EComResponse retVal = createItems(dataArray, numData, paLayerParameter);

  if(e_InitOk == retVal){
    if(e_Subscriber == getCommFB()->getComServiceType()){
      if(0 == numData){
        numData = 1;  //register for the item used for the event transmition
      }

      for(int i = 0; i < numData; i++){
        CEclipseSCADASFPHandler::getInstance().registerWriteCallBack(mSFPItem[i], this);
      }
    }
  }
  return retVal;
}

EComResponse CES_SFP_Layer::createItems(CIEC_ANY *paDataArray, int paNumData, char *paLayerParameter){
  EComResponse retVal = e_InitOk;

  if(0 == paNumData){
    //handle pure event messages
    mSFPItem = new sfp_item *[1];
    *mSFPItem = CEclipseSCADASFPHandler::getInstance().registerDataPoint(paLayerParameter, "Coment");
    if(0 != *mSFPItem){
      sfp_item_update_data_allocated(*mSFPItem, sfp_variant_new_null(), sfp_time_in_millis ());
    }else{
      retVal = e_InitInvalidId;
    }
  }else{
    mSFPItem = new sfp_item *[paNumData];
    memset(mSFPItem, 0, sizeof(sfp_item *) * paNumData);
    char *nextParam;

    for(int i = 0; i < paNumData; i++){
      if(0 != paLayerParameter){
        nextParam = strchr(paLayerParameter, scmParameterSeperator);
        if(0 != nextParam){
          *nextParam = '\0';
          nextParam++;
        }
        mSFPItem[i] = CEclipseSCADASFPHandler::getInstance().registerDataPoint(paLayerParameter, "Coment");

        if(0 != mSFPItem[i]){
          //write the initial value to the SFP server so that the data type of the item gets set
          CEclipseSCADASFPHandler::updateDataPoint(mSFPItem[i], paDataArray[i]);
        }else {
          retVal = e_InitInvalidId;
          break;
        }
      }else{
        retVal = e_InitInvalidId;
        break;
      }
      paLayerParameter = nextParam;
    }
  }

  return retVal;
}

