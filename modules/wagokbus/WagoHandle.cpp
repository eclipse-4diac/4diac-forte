/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "WagoHandle.h"

#include "forte/iec61131_functions.h"

WagoHandle::WagoHandle(WagoDeviceController *paController,
                       CIEC_ANY::EDataTypeID paType,
                       forte::io::IOMapper::Direction paDirection,
                       tApplicationDeviceInterface *paAppDevInterface,
                       uint32_t paTaskId,
                       tDeviceId paKBusDeviceId,
                       TForteUInt32 paOutputOffset,
                       TForteUInt32 paInputOffset) :
    IOHandle(paController, paDirection, paType),
    mAppDevInterface(paAppDevInterface),
    mTaskId(paTaskId),
    mKBusDeviceId(paKBusDeviceId),
    mOutputOffset(paOutputOffset),
    mInputOffset(paInputOffset) {
  switch (mType) {
    case CIEC_ANY::e_BOOL: mLastValue = new CIEC_BOOL; break;
    case CIEC_ANY::e_WORD: mLastValue = new CIEC_WORD; break;
    case CIEC_ANY::e_DWORD: mLastValue = new CIEC_DWORD; break;
    default: mLastValue = 0;
  }
}

WagoHandle::~WagoHandle() {
  if (0 != mLastValue) {
    delete mLastValue;
  }
}

void WagoHandle::set(const CIEC_ANY &paState) {
  mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);

  switch (mType) {
    case CIEC_ANY::e_BOOL: setBoolean(static_cast<const CIEC_BOOL &>(paState)); break;
    case CIEC_ANY::e_WORD: setWord(static_cast<const CIEC_WORD &>(paState)); break;
    case CIEC_ANY::e_DWORD: setDWord(static_cast<const CIEC_DWORD &>(paState)); break;
    default: break;
  }

  mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
}

void WagoHandle::get(CIEC_ANY &paState) {
  mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId);

  switch (mType) {
    case CIEC_ANY::e_BOOL: getBoolean(static_cast<CIEC_BOOL &>(paState)); break;
    case CIEC_ANY::e_WORD: getWord(static_cast<CIEC_WORD &>(paState)); break;
    case CIEC_ANY::e_DWORD: getDWord(static_cast<CIEC_DWORD &>(paState)); break;
    default: break;
  }

  mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId);
}

bool WagoHandle::check() {
  bool changed = false;
  switch (mType) {
    case CIEC_ANY::e_BOOL: changed = checkValue<CIEC_BOOL>(); break;
    case CIEC_ANY::e_WORD: changed = checkValue<CIEC_WORD>(); break;
    case CIEC_ANY::e_DWORD: changed = checkValue<CIEC_DWORD>(); break;
    default: break;
  }
  return changed;
}

void WagoHandle::dropObserver() {
  set(CIEC_WORD(0));
  IOHandle::dropObserver();
}

void WagoHandle::getBoolean(CIEC_BOOL &paState) {
  bool inDataBool = false;
  mAppDevInterface->ReadBool(mKBusDeviceId, mTaskId, mInputOffset, &inDataBool);
  paState = CIEC_BOOL(inDataBool);
}

void WagoHandle::getWord(CIEC_WORD &paState) {
  TForteByte inDataWord[2];
  mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, mInputOffset, 2, inDataWord);
  paState = CIEC_WORD((inDataWord[1] << 8) | inDataWord[0]);
}

void WagoHandle::getDWord(CIEC_DWORD &paState) {
  TForteByte inDataDWord[4];
  mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, mInputOffset, 4, inDataDWord);
  paState = CIEC_DWORD((inDataDWord[3] << 24) | (inDataDWord[2] << 16) | (inDataDWord[1] << 8) | inDataDWord[0]);
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

void WagoHandle::setDWord(const CIEC_DWORD &paState) {
  TForteDWord dataDWord = paState;
  TForteByte outData[4];
  outData[0] = static_cast<TForteByte>(dataDWord & 0x00FF);
  outData[1] = static_cast<TForteByte>(dataDWord >> 8);
  outData[2] = static_cast<TForteByte>(dataDWord >> 16);
  outData[3] = static_cast<TForteByte>(dataDWord >> 24);
  mAppDevInterface->WriteBytes(mKBusDeviceId, mTaskId, mOutputOffset, 4, outData);
}

template<typename T>
bool WagoHandle::checkValue() {
  bool retVal;
  T value;
  get(value);
  retVal = func_NOT(func_EQ(value, static_cast<T &>(*mLastValue)));
  mLastValue->setValue(value);
  return retVal;
}
