/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger, Matthias Plasch
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef ANYELEMHELPER_H_
#define ANYELEMHELPER_H_

#include "../datatypes/forte_real.h"
#include "../datatypes/forte_lreal.h"
#include "../datatypes/forte_lint.h"
#include "../datatypes/forte_dint.h"
#include "../datatypes/forte_bool.h"
#include "../datatypes/forte_byte.h"
#include "../datatypes/forte_word.h"
#include "../datatypes/forte_lword.h"
#include "../datatypes/forte_dword.h"
#include "../datatypes/forte_sint.h"
#include "../datatypes/forte_ulint.h"
#include "../datatypes/forte_uint.h"
#include "../datatypes/forte_usint.h"
#include "../datatypes/forte_udint.h"
#include "../datatypes/forte_int.h"
#include <assert.h>
#include <devlog.h>

template<class T>
void anyBitFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  switch (pa_eDataTypeId){
    case CIEC_ANY::e_BOOL:
      pa_roFB.template calculateValue<CIEC_BOOL>();
      break;
    case CIEC_ANY::e_BYTE:
      pa_roFB.template calculateValue<CIEC_BYTE>();
      break;
    case CIEC_ANY::e_WORD:
      pa_roFB.template calculateValue<CIEC_WORD>();
      break;
    case  CIEC_ANY::e_LWORD:
      pa_roFB.template calculateValue<CIEC_LWORD>();
      break;
    default:
      pa_roFB.template calculateValue<CIEC_DWORD>();
      break;
  }
}

template<class T>
void anyBitFBHelperWithoutBool(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  switch (pa_eDataTypeId){
    case CIEC_ANY::e_BYTE:
      pa_roFB.template calculateValue<CIEC_BYTE>();
      break;
    case CIEC_ANY::e_WORD:
      pa_roFB.template calculateValue<CIEC_WORD>();
      break;

    case  CIEC_ANY::e_LWORD:
      pa_roFB.template calculateValue<CIEC_LWORD>();
      break;
    case CIEC_ANY::e_DWORD:
      pa_roFB.template calculateValue<CIEC_DWORD>();
      break;
    default:
      //if there is an invalid input we will not do anything here, could help to find this more easily when debugging the application
      DEVLOG_ERROR("[anyBitFBHelperWithoutBool]: Wrong type usage, this function can be used only with sub-types of ANY_BIT except CIEC_BOOL");
      break;
  }
}

template<class T>
void anyMagnitudeFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  switch (pa_eDataTypeId){
    case CIEC_ANY::e_REAL:
      pa_roFB.template calculateValue<CIEC_REAL>();
      break;
    case CIEC_ANY::e_LREAL:
      pa_roFB.template calculateValue<CIEC_LREAL>();
      break;
    default:
      if(pa_eDataTypeId <= CIEC_ANY::e_WCHAR){
          pa_roFB.template calculateValue<CIEC_LINT>();
      }
      break;
  }
}

template<class T>
void anyIntFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  switch (pa_eDataTypeId){
    case CIEC_ANY::e_SINT:
      pa_roFB.template calculateValue<CIEC_SINT>();
      break;
    case CIEC_ANY::e_DINT:
      pa_roFB.template calculateValue<CIEC_DINT>();
      break;
    case CIEC_ANY::e_LINT:
      pa_roFB.template calculateValue<CIEC_LINT>();
      break;
    case CIEC_ANY::e_ULINT:
      pa_roFB.template calculateValue<CIEC_ULINT>();
      break;
    case CIEC_ANY::e_UINT:
      pa_roFB.template calculateValue<CIEC_UINT>();
      break;
    case CIEC_ANY::e_USINT:
      pa_roFB.template calculateValue<CIEC_USINT>();
      break;
    case CIEC_ANY::e_UDINT:
      pa_roFB.template calculateValue<CIEC_UDINT>();
      break;
    default:
      pa_roFB.template calculateValue<CIEC_INT>();
      break;
  }
}

template<class T>
void anyRealFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  switch (pa_eDataTypeId){
    case CIEC_ANY::e_REAL:
      pa_roFB.template calculateValue<CIEC_REAL>();
      break;
    case CIEC_ANY::e_LREAL:
      pa_roFB.template calculateValue<CIEC_LREAL>();
      break;
    default:
      break;
  }
}

template<class T>
void anyNumFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
    if(CIEC_ANY::e_LREAL == pa_eDataTypeId || CIEC_ANY::e_REAL == pa_eDataTypeId){
      anyRealFBHelper(pa_eDataTypeId, pa_roFB);
    }else{
      anyIntFBHelper(pa_eDataTypeId, pa_roFB);
    }
}

template<class T>
void anyElementaryFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  if(CIEC_STRING::e_STRING == pa_eDataTypeId){
     pa_roFB.template calculateValueString<CIEC_STRING>();
  }
#ifdef FORTE_USE_WSTRING_DATATYPE
  else  if(CIEC_WSTRING::e_WSTRING == pa_eDataTypeId){
      pa_roFB.template calculateValueString<CIEC_WSTRING>();
  }
#endif
  else{
    anyMagnitudeFBHelper(pa_eDataTypeId, pa_roFB);
  }
}

template<class T>
void anyStringFBHelper(CIEC_ANY::EDataTypeID pa_eDataTypeId, T &pa_roFB){
  if(CIEC_STRING::e_STRING == pa_eDataTypeId){
     pa_roFB.template calculateValueString<CIEC_STRING>();
  }
#ifdef FORTE_USE_WSTRING_DATATYPE
  else  if(CIEC_WSTRING::e_WSTRING == pa_eDataTypeId){
      pa_roFB.template calculateValueString<CIEC_WSTRING>();
  }
#endif
}

template <typename T>
void anyRealToAnyIntInOutHelper(CIEC_ANY::EDataTypeID paDataTypeIdIn, CIEC_ANY::EDataTypeID paDataTypeIdOut, T &paFB) {
  switch (paDataTypeIdIn) {
  case CIEC_ANY::e_REAL:
    switch (paDataTypeIdOut) {
      case CIEC_ANY::e_SINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_SINT>();
        break;
      case CIEC_ANY::e_INT:
        paFB.template calculateValue<CIEC_REAL, CIEC_INT>();
        break;
      case CIEC_ANY::e_DINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_DINT>();
        break;
      case CIEC_ANY::e_LINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_LINT>();
        break;
      case CIEC_ANY::e_USINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_USINT>();
        break;
      case CIEC_ANY::e_UINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_UINT>();
        break;
      case CIEC_ANY::e_UDINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_UDINT>();
        break;
      case CIEC_ANY::e_ULINT:
        paFB.template calculateValue<CIEC_REAL, CIEC_ULINT>();
        break;
      default:
        DEVLOG_ERROR("Invalid type used in anyTruncInOutHelper! No calculation performed!");
    }
    break;
  case CIEC_ANY::e_LREAL:
    switch (paDataTypeIdOut) {
      case CIEC_ANY::e_SINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_SINT>();
        break;
      case CIEC_ANY::e_INT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_INT>();
        break;
      case CIEC_ANY::e_DINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_DINT>();
        break;
      case CIEC_ANY::e_LINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_LINT>();
        break;
      case CIEC_ANY::e_USINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_USINT>();
        break;
      case CIEC_ANY::e_UINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_UINT>();
        break;
      case CIEC_ANY::e_UDINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_UDINT>();
        break;
      case CIEC_ANY::e_ULINT:
        paFB.template calculateValue<CIEC_LREAL, CIEC_ULINT>();
        break;
      default:
        DEVLOG_ERROR("Invalid type used in anyTruncInOutHelper! No calculation performed!");
    }
  default:
    DEVLOG_ERROR("Invalid type used in anyTruncInOutHelper! No calculation performed!");
  }
}

#endif /* ANYELEMHELPER_H_ */
