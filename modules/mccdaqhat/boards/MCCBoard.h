/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#pragma once

#include "../mccdaqhat_api.h"
#include "forte/arch/forte_sync.h"

#include <string>

namespace forte::eclipse4diac::io::mccdaqhat {

  /*! @brief State of one HAT: open flag, input cache and error counter.
   *
   * The controller thread calls poll(); output handles write from the FB thread. mMutex serialises both, because
   * the board objects share their cache and the vendor library between the two threads.
   */
  class MCCBoard {
    public:
      //! A board keeps running through transient SPI errors; only a persistent fault stops the controller.
      static constexpr unsigned int scmMaxConsecutiveErrors = 10;

      MCCBoard(IMCCDaqHatApi &paApi, uint8_t paAddress);
      virtual ~MCCBoard() = default;
      MCCBoard(const MCCBoard &) = delete;
      MCCBoard &operator=(const MCCBoard &) = delete;

      uint8_t getAddress() const {
        return mAddress;
      }

      virtual HatType getType() const = 0;

      //! Reads all inputs into the cache. Returns false once scmMaxConsecutiveErrors is reached.
      bool poll();

      //! Closes the board; safe to call more than once
      virtual void close() = 0;

      std::string getLastError();

    protected:
      //! Board specific input read, called with mMutex held
      virtual int readInputs() = 0;

      //! Counts the result of a hardware call (mMutex held) and passes it through
      int track(int paResult);

      std::string formatError(int paResult) const;

      IMCCDaqHatApi &mApi;
      const uint8_t mAddress;
      arch::CSyncObject mMutex;
      bool mOpen = false;

    private:
      unsigned int mConsecutiveErrors = 0;
      std::string mLastError;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
