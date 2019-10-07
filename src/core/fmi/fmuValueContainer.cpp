/*******************************************************************************
 * Copyright (c) 2016 -2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "fmuValueContainer.h"
#include "fmuInstance.h"
#include "../../arch/devlog.h"

fmuValueContainer::fmuValueContainer(fmuValueContainer::valueType pa_valueType, bool pa_needPointerToValue) :
    mErrorOcurred(false), mCallback(0), mCallBackArgument(0), mIsEvent(false){
  mValue = allocateValuePointer(pa_valueType);
  if(pa_needPointerToValue){
    mPointerToValue = new UPointerToValue;
  }else{
    mPointerToValue = 0;
  }
}

fmuValueContainer::fmuValueContainer(fmuValueContainer& pa_Source){
  mValue = allocateValuePointer(getValueFromType(pa_Source.getDataTypeID()));
  if(pa_Source.mPointerToValue){
     mPointerToValue = new UPointerToValue;
     *mPointerToValue = *pa_Source.mPointerToValue;
   }else{
     mPointerToValue = 0;
   }
}

fmuValueContainer::~fmuValueContainer(){
  delete mValue;
  if(mPointerToValue){
    delete mPointerToValue;
  }
}

void fmuValueContainer::setCallback(newValueArrived pa_callback){
  mCallback = pa_callback;
}

void fmuValueContainer::setCallbackArgument(void* pa_callbackArguments){
  mCallBackArgument = pa_callbackArguments;
}

void fmuValueContainer::setValue(const CIEC_ANY& pa_value){
  mValue->setValue(pa_value);
  if(mPointerToValue){
    if(mIsEvent){
      CIEC_LINT toAssign;
      toAssign.setValue(pa_value);
      *mPointerToValue->mEventCounter = static_cast<TForteUInt32>(toAssign);
    }
    else{
      mPointerToValue->mActualValue->setValue(pa_value);
    }
  }
  if(0 != mCallback){
    mCallback(mCallBackArgument);
  }
}

void fmuValueContainer::setEventCounterPointer(TForteUInt32* pa_eventCounter){
  if(mPointerToValue){
    mPointerToValue->mEventCounter = pa_eventCounter;
    mIsEvent = true;
  }else{
    DEVLOG_ERROR("[FMU] Trying to set a event counter pointer to a container that was not constructed for it\n");
  }
}

void fmuValueContainer::setValuePointer(CIEC_ANY* pa_valuePointer){
  if(mPointerToValue){
    mPointerToValue->mActualValue = pa_valuePointer;
    mIsEvent = false;
  }else{
    DEVLOG_ERROR("[FMU] Trying to set an CIEC_ANY pointer to a container that was not constructed for it\n");
  }
}

CIEC_BOOL* fmuValueContainer::getValueAsBool() const{
  if(0 != mPointerToValue && !mIsEvent){
      mValue->setValue(*mPointerToValue->mActualValue);
  }
  return static_cast<CIEC_BOOL*>(mValue);
}

CIEC_LINT* fmuValueContainer::getValueAsInt() const{
  if(0 != mPointerToValue){
    if(mIsEvent){
      CIEC_LINT toAssign = *mPointerToValue->mEventCounter;
      mValue->setValue(toAssign);
    }else{
      mValue->setValue(*mPointerToValue->mActualValue);
    }
  }
  return static_cast<CIEC_LINT*>(mValue);
}

CIEC_LREAL* fmuValueContainer::getValueAsReal() const{
  if(0 != mPointerToValue && !mIsEvent){
    switch(mPointerToValue->mActualValue->getDataTypeID()){
      case CIEC_ANY::e_REAL:
        CIEC_REAL::castRealData(*static_cast<CIEC_REAL*>(mPointerToValue->mActualValue), *static_cast<CIEC_ANY*>(mValue));
        break;
      case CIEC_ANY::e_LREAL:
        CIEC_LREAL::castLRealData(*static_cast<CIEC_LREAL*>(mPointerToValue->mActualValue), *static_cast<CIEC_ANY*>(mValue));
        break;
      default:
        mValue->setValue(*mPointerToValue->mActualValue); //should'nt be here
        break;
    }
  }
  return static_cast<CIEC_LREAL*>(mValue);
}

CIEC_STRING* fmuValueContainer::getValueAsString() const{
  return static_cast<CIEC_STRING*>(mValue);
}

CIEC_ANY* fmuValueContainer::allocateValuePointer(fmuValueContainer::valueType pa_valueType){
  CIEC_ANY* allocatedValue;

  switch(pa_valueType){
      case BOOL:
        allocatedValue = new CIEC_BOOL;
        break;
      case INTEGER:
        allocatedValue = new CIEC_LINT;
        break;
      case REAL:
        allocatedValue = new CIEC_LREAL;
        break;
      case STRING:
        allocatedValue = new CIEC_STRING;
        break;
      default:
        allocatedValue = 0;
        mErrorOcurred = true;
    }

  return allocatedValue;
}

fmuValueContainer::valueType fmuValueContainer::getValueFromType(CIEC_ANY::EDataTypeID pa_type){
  fmuValueContainer::valueType retVal = WRONG;
  switch (pa_type){
    case CIEC_ANY::e_BOOL:
      retVal = BOOL;
      break;
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_LINT:
    case CIEC_ANY::e_USINT:
    case CIEC_ANY::e_UINT:
    case CIEC_ANY::e_UDINT:
    case CIEC_ANY::e_ULINT:
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
     retVal = INTEGER;
      break;
    case CIEC_ANY::e_REAL:
    case CIEC_ANY::e_LREAL:
     retVal = REAL;
      break;
    case CIEC_ANY::e_STRING:
    case CIEC_ANY::e_WSTRING:
    case CIEC_ANY::e_DATE:
    case CIEC_ANY::e_DATE_AND_TIME:
    case CIEC_ANY::e_TIME:
    case CIEC_ANY::e_TIME_OF_DAY:
     retVal = STRING;
      break;
    default:

      break;
  }
  return retVal;
}
