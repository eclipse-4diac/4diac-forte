/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "handle.h"
#include "slave.h"
#include <handler/bus.h>
#include <io/mapper/io_mapper.h>
#include <devlog.h>

EmbrickSlaveHandle::EmbrickSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID paType, uint8_t paOffset, EmbrickSlaveHandler *paSlave) :
    forte::core::io::IOHandle(paController, paDirection, paType), mOffset(paOffset), mSlave(paSlave), mUpdateMutex(&mSlave->mUpdateMutex) {
  if(paDirection == forte::core::io::IOMapper::In) {
    mBuffer = mSlave->mUpdateReceiveImage;
  } else if(paDirection == forte::core::io::IOMapper::Out) {
    mBuffer = mSlave->mUpdateSendImage;
  }
}

EmbrickSlaveHandle::~EmbrickSlaveHandle() {
}

void EmbrickSlaveHandle::set(const CIEC_ANY &) {
  mSlave->forceUpdate();
}

void EmbrickSlaveHandle::onObserver(forte::core::io::IOObserver *paObserver) {
  reset();

  forte::core::io::IOHandle::onObserver(paObserver);
}

void EmbrickSlaveHandle::dropObserver() {
  forte::core::io::IOHandle::dropObserver();

  reset();
}

