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

#include "forte/io/mapper/io_handle.h"
#include "../boards/MCC128Board.h"
#include "../boards/MCC152Board.h"

#include <cstddef>

namespace forte::eclipse4diac::io::mccdaqhat {

  //! Common base: remembers the owning device so that dropSlaveHandles() can remove exactly its handles.
  class MCCHandle : public forte::io::IOHandle {
    public:
      MCCHandle(forte::io::IODeviceController *paController,
                forte::io::IOMapper::Direction paDirection,
                CIEC_ANY::EDataTypeID paType,
                size_t paDeviceIndex);

      size_t getDeviceIndex() const {
        return mDeviceIndex;
      }

      //! True if the cached input changed since the previous call. Outputs never report a change.
      virtual bool checkChanged() {
        return false;
      }

      //! True while a failed hardware write has been logged and no write succeeded since
      bool isWriteErrorReported() const {
        return mWriteErrorReported;
      }

    protected:
      //! Logs a failed output write once per failure episode; the next successful write ends the episode.
      void reportWriteResult(int paResult, MCCBoard &paBoard, const char *paPort, uint8_t paChannel);

    private:
      const size_t mDeviceIndex;
      bool mWriteErrorReported = false;
  };

  class MCCAnalogInHandle final : public MCCHandle {
    public:
      MCCAnalogInHandle(forte::io::IODeviceController *paController,
                        size_t paDeviceIndex,
                        MCC128Board &paBoard,
                        uint8_t paChannel);
      void set(const CIEC_ANY &) override;
      void get(CIEC_ANY &paValue) override;
      bool checkChanged() override;

    private:
      MCC128Board &mBoard;
      const uint8_t mChannel;
      uint16_t mLastCode = 0;
  };

  class MCCDigitalInHandle final : public MCCHandle {
    public:
      MCCDigitalInHandle(forte::io::IODeviceController *paController,
                         size_t paDeviceIndex,
                         MCC152Board &paBoard,
                         uint8_t paChannel);
      void set(const CIEC_ANY &) override;
      void get(CIEC_ANY &paValue) override;
      bool checkChanged() override;

    private:
      MCC152Board &mBoard;
      const uint8_t mChannel;
      bool mLastValue = false;
  };

  class MCCDigitalOutHandle final : public MCCHandle {
    public:
      MCCDigitalOutHandle(forte::io::IODeviceController *paController,
                          size_t paDeviceIndex,
                          MCC152Board &paBoard,
                          uint8_t paChannel);
      void set(const CIEC_ANY &paValue) override;
      void get(CIEC_ANY &paValue) override;

    private:
      MCC152Board &mBoard;
      const uint8_t mChannel;
      bool mValue = false;
  };

  class MCCAnalogOutHandle final : public MCCHandle {
    public:
      MCCAnalogOutHandle(forte::io::IODeviceController *paController,
                         size_t paDeviceIndex,
                         MCC152Board &paBoard,
                         uint8_t paChannel);
      void set(const CIEC_ANY &paValue) override;
      void get(CIEC_ANY &paValue) override;

    private:
      MCC152Board &mBoard;
      const uint8_t mChannel;
      uint16_t mCode = 0;
      bool mClampReported = false;
  };

} // namespace forte::eclipse4diac::io::mccdaqhat
