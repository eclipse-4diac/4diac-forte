/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          OFFIS e.V.
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
 *   Jörg Walter - more efficient pin handling
 *******************************************************************************/

#pragma once

namespace forte::eclipse4diac::io::embrick {

  class EmbrickPinHandler {
      friend class EmbrickBusHandler;

    protected:
      explicit EmbrickPinHandler(unsigned int paPin);
      virtual ~EmbrickPinHandler();

      bool set(bool paState);
      bool enable() {
        return set(true);
      }
      bool disable() {
        return set(false);
      }

      void init();
      void deInit();

      bool hasError() {
        return mError != nullptr;
      }
      const char *mError;

    private:
      unsigned int mPinNumber = 0;
      int mPinFd = -1;
      bool didExport = false;

      void fail(const char *paReason);
  };
} // namespace forte::eclipse4diac::io::embrick
