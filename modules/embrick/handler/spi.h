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

#include <fcntl.h>
#include <unistd.h>

namespace forte::eclipse4diac::io::embrick {

  class EmbrickSPIHandler {
      friend class EmbrickBusHandler;

    protected:
      explicit EmbrickSPIHandler(unsigned int paInterface);
      virtual ~EmbrickSPIHandler();

      bool transfer(unsigned char *paSendBuffer, unsigned char *paRreceiveBuffer, int paLength);

      void init(const char *paSpidev);
      void deInit();
      template<typename T>
      bool config(unsigned int paConfig, unsigned int paConfigVerify, T paValue);

      bool hasError() {
        return mError != nullptr;
      }

      static unsigned long const scmDefaultSpiSpeed;
      static unsigned long const scmMaxSpiSpeed;

      const char *mError;

      void setSpeed(const unsigned long paSpeed);

    private:
      int mFd;
      unsigned long mSpiSpeed;

      void fail(const char *paReason);
  };

} // namespace forte::eclipse4diac::io::embrick
