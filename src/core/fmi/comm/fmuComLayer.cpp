/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/


#include "../../cominfra/commfb.h"
#include "fmuComLayer.h"
#include "fmuHandler.h"
#include "../fmuInstance.h" //just for logging?

using namespace forte::com_infra;

fmuComLayer::fmuComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB) : CComLayer(paUpperLayer, paFB),
    m_someValueChanged(false), mInterruptResp(e_Nothing) {

  m_outputs = getExtEvHandler<fmuHandler>().getOutputMap()->at(paFB);
  m_inputs =  getExtEvHandler<fmuHandler>().getInputMap()->at(paFB);

  if(m_outputs){
    for(unsigned int i = 0; i < m_outputs->size(); i++){
      bool toAdd = false;
      m_inputsArrived.push_back(toAdd);
    }
  }

}

fmuComLayer::~fmuComLayer() {
}

EComResponse fmuComLayer::sendData(void* paData, unsigned int paSize) {
  CIEC_ANY* SDs(static_cast<CIEC_ANY *>(paData));
  for(unsigned int i = 0; i < paSize; i++){
    m_inputs->at(i)->setValue(SDs[i]);
  }
  return e_ProcessDataOk;
}

EComResponse fmuComLayer::recvData(const void* paData, unsigned int){

  mInterruptResp = e_Nothing;
  const fmuValueContainer* l_value = static_cast<const fmuValueContainer*>(paData);
  unsigned int counter = 0;
  for(std::vector<fmuValueContainer*>::iterator itRunner =  m_outputs->begin(); itRunner != m_outputs->end(); ++itRunner){
    if(*itRunner == l_value){
      switch (l_value->getDataTypeID()){
        case CIEC_ANY::e_BOOL:
          if(*static_cast<CIEC_BOOL*>(&getCommFB()->getRDs()[counter]) != *l_value->getValueAsBool()){
            getCommFB()->getRDs()[counter].setValue(*l_value->getValueAsBool());
            m_someValueChanged = true;
          }
          break;
        case CIEC_ANY::e_LINT:
          if(*static_cast<CIEC_LINT*>(&getCommFB()->getRDs()[counter]) != *l_value->getValueAsInt()){
            getCommFB()->getRDs()[counter].setValue(*l_value->getValueAsInt());
            m_someValueChanged = true;
          }
          break;
        case CIEC_ANY::e_LREAL:{
          CIEC_LREAL newValue;
          switch (getCommFB()->getRDs()[counter].getDataTypeID()){
            case CIEC_ANY::e_REAL:
              CIEC_REAL::castRealData(*static_cast<CIEC_REAL*>(&getCommFB()->getRDs()[counter]), newValue);
              break;
            case CIEC_ANY::e_LREAL:
              CIEC_LREAL::castLRealData(*static_cast<CIEC_LREAL*>(&getCommFB()->getRDs()[counter]), newValue);
              break;
          }

          if(newValue != *l_value->getValueAsReal()){
            getCommFB()->getRDs()[counter].setValue(*l_value->getValueAsReal());
            m_someValueChanged = true;
          }
          break;
        }

        case CIEC_ANY::e_STRING:
          if(*static_cast<CIEC_STRING*>(&getCommFB()->getRDs()[counter]) != *l_value->getValueAsString()){
            getCommFB()->getRDs()[counter].setValue(*l_value->getValueAsString());
            m_someValueChanged = true;
          }
          break;
        default:
          //TODO: Can't be here, right?
          break;
      }
      m_inputsArrived.at(counter) = true;
      break;
    }
    counter++;
  }

  bool someFalse = false;
  for(std::vector<bool>::iterator itRunner = m_inputsArrived.begin(); itRunner != m_inputsArrived.end(); ++itRunner){
    if(false == *itRunner){
      someFalse = true;
      break;
    }
  }

  if(!someFalse){
    if(m_someValueChanged){
      mInterruptResp = e_ProcessDataOk;
      mFb->interruptCommFB(this);
    }else{
    }
    for(std::vector<bool>::iterator itRunner = m_inputsArrived.begin(); itRunner != m_inputsArrived.end(); ++itRunner){
      *itRunner = false;
    }
    m_someValueChanged = false;
  }
  return mInterruptResp;
}

EComResponse fmuComLayer::processInterrupt() {
  return mInterruptResp;
}

void fmuComLayer::closeConnection() {
  getExtEvHandler<fmuHandler>().unregisterLayer(this);
}

EComResponse fmuComLayer::openConnection(char* ) {
  EComResponse retVal = e_InitOk;

  if(this->getCommFB()->getNumRD() == 0 && this->getCommFB()->getNumSD() == 0){
    FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_COMM_LAYER() ,MODEL_GUID << " Cannot create a communication IO wihtout inputs and outputs\n" << "--------------\n");
  }else{

    if(fmuHandler::eRegisterLayerSucceeded == getExtEvHandler<fmuHandler>().registerLayer(this)){
      retVal = e_InitOk;
    }
    else{
      retVal = e_InitInvalidId;
    }
  }
  return retVal;
}

