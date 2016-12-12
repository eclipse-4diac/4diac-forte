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
    buffer(buffer), offset(offset), syncMutex(syncMutex) {
}

SlaveHandle::~SlaveHandle() {
}

BitSlaveHandle::BitSlaveHandle(unsigned char* buffer, uint8_t offset,
    uint8_t position, CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex), mask((uint8_t) (1 << position)) {
  type = CIEC_ANY::e_BOOL;
}

void BitSlaveHandle::set(const CIEC_ANY &state) {
  syncMutex->lock();

  if (static_cast<const CIEC_BOOL&>(state))
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  syncMutex->unlock();
}

void BitSlaveHandle::get(CIEC_ANY &state) {
  syncMutex->lock();

  static_cast<CIEC_BOOL&>(state) = (*(buffer + offset) & mask) != 0;

  syncMutex->unlock();
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

Analog10SlaveHandle::Analog10SlaveHandle(unsigned char* buffer, uint8_t offset,
CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex) {
  type = CIEC_ANY::e_DWORD;
}

void Analog10SlaveHandle::set(const CIEC_ANY &value) {
  syncMutex->lock();

  *(buffer + offset) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset + 1) = (static_cast<const CIEC_DWORD&>(value) / 256) & 0x03;

  syncMutex->unlock();
}

void Analog10SlaveHandle::get(CIEC_ANY &value) {
  syncMutex->lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  syncMutex->unlock();
}

bool Analog10SlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD Analog10SlaveHandle::getValue(const unsigned char* buffer) {
  return (*(buffer + offset + 1) & 0x03) * 256 + *(buffer + offset);
}

AnalogSlaveHandle::AnalogSlaveHandle(unsigned char* buffer, uint8_t offset,
CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex) {
  type = CIEC_ANY::e_DWORD;
}

void AnalogSlaveHandle::set(const CIEC_ANY &value) {
  syncMutex->lock();

  *(buffer + offset) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) / 256;

  syncMutex->unlock();
}

void AnalogSlaveHandle::get(CIEC_ANY &value) {
  syncMutex->lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  syncMutex->unlock();
}

bool AnalogSlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD AnalogSlaveHandle::getValue(const unsigned char* buffer) {
  return *(buffer + offset + 1) * 256 + *(buffer + offset);
}

} /* namespace EmBrick */
