/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
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
#include "forte/io/mapper/io_mapper.h"

namespace forte::eclipse4diac::io::embrick {

  EmbrickSlaveHandle::EmbrickSlaveHandle(forte::io::IODeviceController *paController,
                                         forte::io::IOMapper::Direction paDirection,
                                         CIEC_ANY::EDataTypeID paType,
                                         uint8_t paOffset,
                                         EmbrickSlaveHandler *paSlave) :
      IOHandle(paController, paDirection, paType),
      mOffset(paOffset),
      mSlave(paSlave),
      mUpdateMutex(&mSlave->mUpdateMutex) {
    if (paDirection == forte::io::IOMapper::In) {
      mBuffer = mSlave->mUpdateReceiveImage;
    } else if (paDirection == forte::io::IOMapper::Out) {
      mBuffer = mSlave->mUpdateSendImage;
    }
  }

  void EmbrickSlaveHandle::set(const CIEC_ANY &) {
    mSlave->forceUpdate();
  }

  void EmbrickSlaveHandle::onObserver(forte::io::IOObserver *paObserver) {
    reset();

    IOHandle::onObserver(paObserver);
  }

  void EmbrickSlaveHandle::dropObserver() {
    IOHandle::dropObserver();

    reset();
  }

} // namespace forte::eclipse4diac::io::embrick
