/*
 * fmuValueContainer.cpp
 *
 *  Created on: 16.08.2017
 *      Author: cabral
 */

#include "fmuValueContainer.h"
#include "fmuInstance.h"

fmuValueContainer::fmuValueContainer(fmuValueContainer::valueType pa_valueType, bool pa_needPointerToValue) :
    m_errorOcurred(false), m_callback(0), m_callBackArgument(0), m_isEvent(false){
  m_value = allocateValuePointer(pa_valueType);
  if(pa_needPointerToValue){
    m_pPointerToValue = new UPointerToValue;
  }else{
    m_pPointerToValue = 0;
  }
}

fmuValueContainer::fmuValueContainer(fmuValueContainer& pa_Source){
  m_value = allocateValuePointer(getValueFromType(pa_Source.getDataTypeID()));
  if(pa_Source.m_pPointerToValue){
     m_pPointerToValue = new UPointerToValue;
     *m_pPointerToValue = *pa_Source.m_pPointerToValue;
   }else{
     m_pPointerToValue = 0;
   }
}

fmuValueContainer::~fmuValueContainer(){
  delete m_value;
  if(m_pPointerToValue){
    delete m_pPointerToValue;
  }
}

void fmuValueContainer::setCallback(newValueArrived pa_callback){
  m_callback = pa_callback;
}

void fmuValueContainer::setCallbackArgument(void* pa_callbackArguments){
  m_callBackArgument = pa_callbackArguments;
}

void fmuValueContainer::setValue(const CIEC_ANY& pa_value){
  m_value->setValue(pa_value);
  if(m_pPointerToValue){
    if(m_isEvent){
      CIEC_LINT toAssign;
      toAssign.setValue(pa_value);
      *m_pPointerToValue->m_eventCounter = toAssign;
    }
    else{
      m_pPointerToValue->m_actualValue->setValue(pa_value);
    }
  }
  if(0 != m_callback){
    m_callback(m_callBackArgument);
  }
}

void fmuValueContainer::setEventCounterPointer(TForteUInt32* pa_eventCounter){
  if(m_pPointerToValue){
    m_pPointerToValue->m_eventCounter = pa_eventCounter;
    m_isEvent = true;
  }else{
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("ERROR: Trying to set a event counter pointer to a container that was not constructed for it\n")
#endif
  }
}

void fmuValueContainer::setValuePointer(CIEC_ANY* pa_valuePointer){
  if(m_pPointerToValue){
    m_pPointerToValue->m_actualValue = pa_valuePointer;
    m_isEvent = false;
  }else{
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("ERROR: Trying to set an CIEC_ANY pointer to a container that was not constructed for it\n")
#endif
  }
}

CIEC_BOOL* fmuValueContainer::getValueAsBool() const{
  if(0 != m_pPointerToValue && !m_isEvent){
      m_value->setValue(*m_pPointerToValue->m_actualValue);
  }
  return static_cast<CIEC_BOOL*>(m_value);
}

CIEC_LINT* fmuValueContainer::getValueAsInt() const{
  if(0 != m_pPointerToValue){
    if(m_isEvent){
      CIEC_LINT toAssign = *m_pPointerToValue->m_eventCounter;
      m_value->setValue(toAssign);
    }else{
      m_value->setValue(*m_pPointerToValue->m_actualValue);
    }
  }
  return static_cast<CIEC_LINT*>(m_value);
}

CIEC_LREAL* fmuValueContainer::getValueAsReal() const{
  if(0 != m_pPointerToValue && !m_isEvent){
    switch(m_pPointerToValue->m_actualValue->getDataTypeID()){
      case CIEC_ANY::e_REAL:
        CIEC_REAL::castRealData(*static_cast<CIEC_REAL*>(m_pPointerToValue->m_actualValue), *static_cast<CIEC_ANY*>(m_value));
        break;
      case CIEC_ANY::e_LREAL:
        CIEC_LREAL::castLRealData(*static_cast<CIEC_LREAL*>(m_pPointerToValue->m_actualValue), *static_cast<CIEC_ANY*>(m_value));
        break;
      default:
        m_value->setValue(*m_pPointerToValue->m_actualValue); //should'nt be here
        break;
    }
  }
  return static_cast<CIEC_LREAL*>(m_value);
}

CIEC_STRING* fmuValueContainer::getValueAsString() const{
  return static_cast<CIEC_STRING*>(m_value);
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
        m_errorOcurred = true;
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
    case CIEC_ANY::e_DATE:
    case CIEC_ANY::e_DATE_AND_TIME:
    case CIEC_ANY::e_TIME:
    case CIEC_ANY::e_TIME_OF_DAY:
     retVal = INTEGER;
      break;
    case CIEC_ANY::e_REAL:
    case CIEC_ANY::e_LREAL:
     retVal = REAL;
      break;
    case CIEC_ANY::e_STRING:
    case CIEC_ANY::e_WSTRING:
     retVal = STRING;
      break;
    default:

      break;
  }
  return retVal;
}
