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
#include "slave.h"
#include <handler/bus.h>
#include <io/mapper.h>
#include <devlog.h>
namespace EmBrick {

SlaveHandle::SlaveHandle(IOMapper::Direction direction, uint8_t offset,
    Slave *slave) :
    IOHandle(direction), offset(offset), slave(slave) {
  if (direction == IOMapper::In)
    buffer = slave->updateReceiveImage;
  else if (direction == IOMapper::Out)
    buffer = slave->updateSendImage;
}

SlaveHandle::~SlaveHandle() {
}

void SlaveHandle::set(const CIEC_ANY &) {
  slave->forceUpdate();
}

void SlaveHandle::onObserver(IOObserver *observer) {
  reset();

  IOHandle::onObserver(observer);
}

void SlaveHandle::dropObserver() {
  IOHandle::dropObserver();

  reset();
}

BitSlaveHandle::BitSlaveHandle(IOMapper::Direction direction, uint8_t offset,
    uint8_t position, Slave *slave) :
    SlaveHandle(direction, offset, slave), mask((uint8_t) (1 << position)) {
  type = CIEC_ANY::e_BOOL;
}

void BitSlaveHandle::set(const CIEC_ANY &state) {
  slave->syncMutex.lock();

  if (static_cast<const CIEC_BOOL&>(state))
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  slave->syncMutex.unlock();

  SlaveHandle::set(state);
}

void BitSlaveHandle::get(CIEC_ANY &state) {
  slave->syncMutex.lock();

  static_cast<CIEC_BOOL&>(state) = (*(buffer + offset) & mask) != 0;

  slave->syncMutex.unlock();
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

Analog10SlaveHandle::Analog10SlaveHandle(IOMapper::Direction direction,
    uint8_t offset, Slave *slave) :
    SlaveHandle(direction, offset, slave) {
  type = CIEC_ANY::e_DWORD;
}

void Analog10SlaveHandle::set(const CIEC_ANY &value) {
  slave->syncMutex.lock();

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (static_cast<const CIEC_DWORD&>(value) / 256) & 0x03;

  slave->syncMutex.unlock();

  SlaveHandle::set(value);
}

void Analog10SlaveHandle::get(CIEC_ANY &value) {
  slave->syncMutex.lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  slave->syncMutex.unlock();
}

bool Analog10SlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD Analog10SlaveHandle::getValue(const unsigned char* buffer) {
  return (*(buffer + offset) & 0x03) * 256 + *(buffer + offset + 1);
}

AnalogSlaveHandle::AnalogSlaveHandle(IOMapper::Direction direction,
    uint8_t offset, Slave *slave) :
    SlaveHandle(direction, offset, slave) {
  type = CIEC_ANY::e_DWORD;
}

void AnalogSlaveHandle::set(const CIEC_ANY &value) {
  slave->syncMutex.lock();

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (unsigned char) (static_cast<const CIEC_DWORD&>(value)
      / 256);

  slave->syncMutex.unlock();

  SlaveHandle::set(value);
}

void AnalogSlaveHandle::get(CIEC_ANY &value) {
  slave->syncMutex.lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  slave->syncMutex.unlock();
}

bool AnalogSlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD AnalogSlaveHandle::getValue(const unsigned char* buffer) {
  return *(buffer + offset) * 256 + *(buffer + offset + 1);
}

} /* namespace EmBrick */
