/*************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "wagoHandle.h"

WagoHandle::WagoHandle(WagoDeviceController *paController, CIEC_ANY::EDataTypeID paType, forte::core::io::IOMapper::Direction paDirection,
    tApplicationDeviceInterface * paAppDevInterface, uint32_t paTaskId, tDeviceId paKBusDeviceId, TForteUInt32 paOutputOffset, TForteUInt32 paInputOffset) :
    forte::core::io::IOHandle(paController, paDirection, paType), mAppDevInterface(paAppDevInterface), mTaskId(paTaskId), mKBusDeviceId(paKBusDeviceId),
        mOutputOffset(paOutputOffset), mInputOffset(paInputOffset) {
  switch(mType){
    case CIEC_ANY::e_BOOL:
      mLastValue = new CIEC_BOOL;
      break;
    case CIEC_ANY::e_WORD:
      mLastValue = new CIEC_WORD;
      break;
    default:
      mLastValue = 0;
  }
}

WagoHandle::~WagoHandle() {
  if(0 != mLastValue) {
    delete mLastValue;
  }
}

void WagoHandle::set(const CIEC_ANY &paState) {
  mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);

  switch(mType){
    case CIEC_ANY::e_BOOL:
      setBoolean(static_cast<const CIEC_BOOL&>(paState));
      break;
    case CIEC_ANY::e_WORD:
      setWord(static_cast<const CIEC_WORD&>(paState));
      break;
    default:
      break;
  }

  mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
}

void WagoHandle::get(CIEC_ANY &paState) {
  mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId);

  switch(mType){
    case CIEC_ANY::e_BOOL:
      getBoolean(static_cast<CIEC_BOOL&>(paState));
      break;
    case CIEC_ANY::e_WORD:
      getWord(static_cast<CIEC_WORD&>(paState));
      break;
    default:
      break;
  }

  mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId);
}

bool WagoHandle::check() {
  bool changed = false;
  switch(mType){
    case CIEC_ANY::e_BOOL:
      changed = checkBoolean();
      break;
    case CIEC_ANY::e_WORD:
      changed = checkWord();
      break;
    default:
      break;
  }
  return changed;
}

void WagoHandle::dropObserver() {
  CIEC_WORD state = 0; //should work for boolean too
  set(state);
  forte::core::io::IOHandle::dropObserver();
}

void WagoHandle::getBoolean(CIEC_BOOL &paState) {
  bool inDataBool = false;
  mAppDevInterface->ReadBool(mKBusDeviceId, mTaskId, mInputOffset, &inDataBool);
  paState = inDataBool;
}

void WagoHandle::getWord(CIEC_WORD &paState) {
  TForteByte inDataWord[2];
  mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, mInputOffset, 2, inDataWord);
  paState = static_cast<TForteWord>((static_cast<TForteWord>(inDataWord[1]) << 8) + (static_cast<TForteWord>(inDataWord[0])));
}

void WagoHandle::setBoolean(const CIEC_BOOL &paState) {
  bool outDataBool = paState;
  mAppDevInterface->WriteBool(mKBusDeviceId, mTaskId, mOutputOffset, outDataBool);
}

void WagoHandle::setWord(const CIEC_WORD &paState) {
  TForteWord dataWord = paState;
  TForteByte outData[2];
  outData[0] = static_cast<TForteByte>(dataWord & 0x00FF);
  outData[1] = static_cast<TForteByte>(dataWord >> 8);
  mAppDevInterface->WriteBytes(mKBusDeviceId, mTaskId, mOutputOffset, 2, outData);
}

bool WagoHandle::checkBoolean() {
  bool retVal;
  CIEC_BOOL value;
  get(value);
  retVal = (value != *static_cast<CIEC_BOOL*>(mLastValue));
  mLastValue->setValue(value);
  return retVal;
}
bool WagoHandle::checkWord() {
  bool retVal;
  CIEC_WORD value;
  get(value);
  retVal = (value != *static_cast<CIEC_WORD*>(mLastValue));
  mLastValue->setValue(value);
  return retVal;
}
