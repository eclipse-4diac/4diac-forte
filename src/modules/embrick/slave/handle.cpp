/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "handle.h"

namespace EmBrick {

SlaveHandle::SlaveHandle(unsigned char* buffer, uint8_t offset,
CSyncObject *syncMutex) :
    IOHandle(), buffer(buffer), offset(offset), syncMutex(syncMutex) {
}

SlaveHandle::~SlaveHandle() {
}

BitSlaveHandle::BitSlaveHandle(unsigned char* buffer, uint8_t offset,
    uint8_t position, CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex), mask((uint8_t) (1 << position)) {
  type = X;
}

void BitSlaveHandle::set(CIEC_BOOL state) {
  syncMutex->lock();

  if (state)
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  syncMutex->unlock();
}

CIEC_BOOL BitSlaveHandle::get() {
  syncMutex->lock();

  CIEC_BOOL res = (*(buffer + offset) & mask) != 0;

  syncMutex->unlock();
  return res;
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

Analog10SlaveHandle::Analog10SlaveHandle(unsigned char* buffer, uint8_t offset,
CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex) {
  type = D;
}

void Analog10SlaveHandle::set(CIEC_DWORD value) {
  syncMutex->lock();

  *(buffer + offset) = value % 256;
  *(buffer + offset + 1) = (value / 256) & 0x03;

  syncMutex->unlock();
}

CIEC_DWORD Analog10SlaveHandle::get() {
  syncMutex->lock();

  CIEC_DWORD res = getValue(buffer);

  syncMutex->unlock();
  return res;
}

bool Analog10SlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

CIEC_DWORD Analog10SlaveHandle::getValue(unsigned char* buffer) {
  return (*(buffer + offset + 1) & 0x03) * 256 + *(buffer + offset);
}

AnalogSlaveHandle::AnalogSlaveHandle(unsigned char* buffer, uint8_t offset,
CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex) {
  type = D;
}

void AnalogSlaveHandle::set(CIEC_DWORD value) {
  syncMutex->lock();

  *(buffer + offset) = value % 256;
  *(buffer + offset + 1) = (value / 256);

  syncMutex->unlock();
}

CIEC_DWORD AnalogSlaveHandle::get() {
  syncMutex->lock();

  CIEC_DWORD res = getValue(buffer);

  syncMutex->unlock();
  return res;
}

bool AnalogSlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

CIEC_DWORD AnalogSlaveHandle::getValue(unsigned char* buffer) {
  return *(buffer + offset + 1) * 256 + *(buffer + offset);
}

} /* namespace EmBrick */
