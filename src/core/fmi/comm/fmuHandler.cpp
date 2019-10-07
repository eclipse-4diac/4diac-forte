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


#include <string.h>
#include <criticalregion.h>
#include <commfb.h>
#include "fmuHandler.h"
#include "../fmuInstance.h"

DEFINE_HANDLER(fmuHandler);

CSinglyLinkedList<fmuComLayer*> fmuHandler::mlayers;


fmuHandler::fmuHandler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution)  {
}

fmuHandler::~fmuHandler(){
}

void fmuHandler::fmuMessageArrived(void *pa_value){

  if(0 != pa_value){
    for(CSinglyLinkedList<fmuComLayer*>::Iterator it = mlayers.begin(); it != mlayers.end(); ++it){
      bool found = false;
      for(std::vector<fmuValueContainer*>::iterator itInputs = (*(*it)->getOutputs()).begin(); itInputs != (*(*it)->getOutputs()).end(); ++itInputs){
        if(pa_value == (*itInputs)){
          found = true;
          if(forte::com_infra::e_Nothing != (*it)->recvData(pa_value, 0)){
            ::getExtEvHandler<fmuHandler>(*(*it)->getCommFB()).startNewEventChain((*it)->getCommFB());
          }
          break;
        }
      }
      if(found){
        break;
      }
    }
  }
}

int fmuHandler::registerLayer(fmuComLayer* paLayer){
  if(paLayer != 0){
    mlayers.pushBack(paLayer);
    return eRegisterLayerSucceeded;
  }
  return eWrongLayer;
}

void fmuHandler::unregisterLayer(fmuComLayer* paLayer){

  CSinglyLinkedList<fmuComLayer*>::Iterator itRunner(mlayers.begin());
  CSinglyLinkedList<fmuComLayer*>::Iterator itRefNode(mlayers.end());
  CSinglyLinkedList<fmuComLayer*>::Iterator itEnd(mlayers.end());

  while(itRunner != itEnd){
    if(*itRunner == paLayer){
      if(itRefNode == itEnd){
        mlayers.popFront();
      }
      else{
        mlayers.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}

void fmuHandler::enableHandler(void){
  //TODO: Should also work empty
}

void fmuHandler::disableHandler(void){
  //TODO: Should also work empty
}

void fmuHandler::setPriority(int ){
  //TODO: Should also work empty
}

int fmuHandler::getPriority(void) const{
  return 0;
}
