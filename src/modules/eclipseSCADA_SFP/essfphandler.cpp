/*******************************************************************************
 * Copyright (c) 2014, 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "essfphandler.h"
#include <forte_any.h>
#include <forte_bool.h>
#include <forte_dint.h>
#include <forte_lint.h>
#include <forte_dint.h>
#include <forte_real.h>
#include <forte_lreal.h>
#include <forte_string.h>
#include <devexec.h>
#include <commfb.h>

using namespace forte::com_infra;


DEFINE_SINGLETON(CEclipseSCADASFPHandler)


const uint16_t CEclipseSCADASFPHandler::scm_unServerPort;

CEclipseSCADASFPHandler::CEclipseSCADASFPHandler(){
  mItemRegistry = sfp_registry_new();
  mEclipseSCADAServer = sfp_server_new(mItemRegistry);
}

CEclipseSCADASFPHandler::~CEclipseSCADASFPHandler(){
  disableHandler();
  sfp_server_free(mEclipseSCADAServer);
  sfp_registry_free(mItemRegistry);
}

struct sfp_item *CEclipseSCADASFPHandler::registerDataPoint(const char *paDataPointId, const char* paDataPointComment){
    struct sfp_item *retVal;
    if(!isAlive()){
      //thread is not running start it
      start();
    }
    retVal = sfp_registry_register_item(mItemRegistry, paDataPointId, paDataPointComment, 0); //TODO extract unit from pa_acDataPointComment
    return retVal;
}

void CEclipseSCADASFPHandler::unregisterDataPoint(struct sfp_item *patItem){
  sfp_registry_unregister_item(patItem);
}

void CEclipseSCADASFPHandler::registerWriteCallBack(struct sfp_item *patItem, CComLayer *paLayer){
  sfp_item_set_write_handler(patItem, CEclipseSCADASFPHandler::handleCellWrite, static_cast<void *>(paLayer));
}

void CEclipseSCADASFPHandler::updateDataPoint(struct sfp_item *patItem, const CIEC_ANY &paDataPoint){
  switch (paDataPoint.getDataTypeID()){
    case CIEC_ANY::e_BOOL:
      sfp_item_update_data_bool_now(patItem, ((CIEC_BOOL &) paDataPoint).operator bool());
      break;
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_USINT:
    case CIEC_ANY::e_UINT:
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
      //Everything smaller or equal 32Bit will be handled here
      sfp_item_update_data_int32_now(patItem, ((CIEC_DINT &) paDataPoint).operator TForteInt32());
      break;
    case CIEC_ANY::e_LINT:
    case CIEC_ANY::e_UDINT:
    case CIEC_ANY::e_ULINT:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      //Everything needing more than 32Bit signed integer will be handled here
      sfp_item_update_data_int64_now(patItem, ((CIEC_LINT &) paDataPoint).operator TForteInt64());
    case CIEC_ANY::e_REAL:
      sfp_item_update_data_double_now(patItem, (double)((CIEC_REAL &) paDataPoint).operator TForteFloat());
      break;
    case CIEC_ANY::e_LREAL:
      sfp_item_update_data_double_now(patItem, (double)((CIEC_LREAL &) paDataPoint).operator TForteDFloat());
      break;
    case CIEC_ANY::e_STRING:
    case CIEC_ANY::e_WSTRING:
      //TODO think about string support in the new interface
      //sfp_variant_set_string(&pa_rstDataPointItem.m_stValueBuf, ((CIEC_ANY_STRING &) paDataPoint).getValue());
      break;
      //TODO think on how to handle these e_DATE, e_TIME_OF_DAY, e_DATE_AND_TIME, e_TIME
    default:
      break;
  }
}

bool CEclipseSCADASFPHandler::readBackDataPoint(const struct sfp_variant *paValue, CIEC_ANY &paDataPoint){
  bool retVal = true;
  switch (paDataPoint.getDataTypeID()){
    case CIEC_ANY::e_BOOL:
      if(VT_BOOLEAN == paValue->type){
        ((CIEC_BOOL &) paDataPoint) = (paValue->payload.val_boolean != 0 );
      }else{
        retVal = false;
      }
      break;
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_USINT:
    case CIEC_ANY::e_UINT:
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
      //Everything smaller or equal 32Bit will be handled here
      if(VT_INT32 == paValue->type){
        ((CIEC_DINT &) paDataPoint) = paValue->payload.val_int;
      }else{
        retVal = false;
      }
      break;
    case CIEC_ANY::e_LINT:
    case CIEC_ANY::e_UDINT:
    case CIEC_ANY::e_ULINT:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      //Everything needing more than 32Bit signed integer will be handled here
      if(VT_INT64 == paValue->type){
        ((CIEC_LINT &) paDataPoint) = paValue->payload.val_long;
      }else{
        retVal = false;
      }
    case CIEC_ANY::e_REAL:
      if(VT_DOUBLE == paValue->type){
        ((CIEC_REAL &) paDataPoint) = static_cast<CIEC_REAL::TValueType>(paValue->payload.val_double);
      }else{
        retVal = false;
      }
      break;
    case CIEC_ANY::e_LREAL:
      if(VT_DOUBLE == paValue->type){
        ((CIEC_LREAL &) paDataPoint) = paValue->payload.val_double;
      }else{
        retVal = false;
      }
      break;
    case CIEC_ANY::e_STRING:
    case CIEC_ANY::e_WSTRING:
      if(VT_BOOLEAN == paValue->type){
        ((CIEC_ANY_STRING &) paDataPoint) = paValue->payload.val_string;
      }else{
        retVal = false;
      }
      break;
    default:
      //TODO think on how to handle these e_DATE, e_TIME_OF_DAY, e_DATE_AND_TIME, e_TIME
      retVal = false;
      break;
  }
  return retVal;
}

void CEclipseSCADASFPHandler::enableHandler(void){
  start();
}

void CEclipseSCADASFPHandler::disableHandler(void){
  sfp_server_exit_loop(mEclipseSCADAServer);
  end();
}

void CEclipseSCADASFPHandler::setPriority(int){
  //currently we are doing nothing here.
  //TODO We should adjust the thread priority.
}

int CEclipseSCADASFPHandler::getPriority(void) const{
  //the same as for setPriority
  return 0;
}

void CEclipseSCADASFPHandler::run(){
  sfp_server_run(mEclipseSCADAServer, scm_unServerPort);
}

void CEclipseSCADASFPHandler::handleCellWrite(struct sfp_item * pa_pstItem, struct sfp_variant *pa_pstValue, int32_t pa_nOperationID,
    struct sfp_strategy * pa_pstStrategy,
    void (*handle_result)(struct sfp_strategy* strategy, int32_t operation_id, struct sfp_error_information* error),
    void * pa_pvCtx){
  CComLayer *layer = static_cast<CComLayer *>(pa_pvCtx);

  EComResponse retVal = layer->recvData(static_cast<void *>(pa_pstValue), 0);

  if(e_ProcessDataOk == retVal){
    //only update data in item if data could be read
    sfp_item_update_data(pa_pstItem, pa_pstValue, sfp_time_in_millis());
  }

  if(e_Nothing != retVal){
    getInstance().startNewEventChain(layer->getCommFB());
  }

//  sfp_error_information_new(
//
  handle_result(pa_pstStrategy, pa_nOperationID, NULL );
}

