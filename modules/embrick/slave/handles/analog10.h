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

#pragma once
#include "../slave.h"
#include "../handle.h"

namespace forte::eclipse4diac::io::embrick {

  class EmbrickAnalog10SlaveHandle : public EmbrickSlaveHandle {
    public:
      EmbrickAnalog10SlaveHandle(forte::io::IODeviceController *paController,
                                 forte::io::IOMapper::Direction paDirection,
                                 uint8_t paOffset,
                                 EmbrickSlaveHandler *paSlave);

      virtual void set(const CIEC_ANY &);
      void get(CIEC_ANY &);

      bool equal(unsigned char *paOldBuffer);

    protected:
      const CIEC_DWORD getValue(const unsigned char *paBuffer);
  };

} // namespace forte::eclipse4diac::io::embrick
