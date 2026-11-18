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

#include "MCCHandles.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_word.h"
#include "forte/util/devlog.h"
#include "../mccdaqhat_config.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  MCCHandle::MCCHandle(forte::io::IODeviceController *paController,
                       forte::io::IOMapper::Direction paDirection,
                       CIEC_ANY::EDataTypeID paType,
                       size_t paDeviceIndex) :
      IOHandle(paController, paDirection, paType),
      mDeviceIndex(paDeviceIndex) {
  }

  void MCCHandle::reportWriteResult(int paResult, MCCBoard &paBoard, const char *paPort, uint8_t paChannel) {
    if (paResult == IMCCDaqHatApi::scmSuccess) {
      mWriteErrorReported = false;
      return;
    }
    // One message per failure episode: a failing board is written on every cycle of the application.
    if (!mWriteErrorReported) {
      DEVLOG_ERROR("[MCCDaqHat] %s_%u write failed: %s\n", paPort, paChannel, paBoard.getLastError().c_str());
    }
    mWriteErrorReported = true;
  }

  MCCAnalogInHandle::MCCAnalogInHandle(forte::io::IODeviceController *paController,
                                       size_t paDeviceIndex,
                                       MCC128Board &paBoard,
                                       uint8_t paChannel) :
      MCCHandle(paController, forte::io::IOMapper::In, CIEC_ANY::e_WORD, paDeviceIndex),
      mBoard(paBoard),
      mChannel(paChannel) {
  }

  void MCCAnalogInHandle::set(const CIEC_ANY &) {
    // Inputs are read-only
  }

  void MCCAnalogInHandle::get(CIEC_ANY &paValue) {
    static_cast<CIEC_WORD &>(paValue) = CIEC_WORD(mBoard.getCode(mChannel));
  }

  bool MCCAnalogInHandle::checkChanged() {
    const uint16_t code = mBoard.getCode(mChannel);
    if (code == mLastCode) {
      return false;
    }
    mLastCode = code;
    return true;
  }

  MCCDigitalInHandle::MCCDigitalInHandle(forte::io::IODeviceController *paController,
                                         size_t paDeviceIndex,
                                         MCC152Board &paBoard,
                                         uint8_t paChannel) :
      MCCHandle(paController, forte::io::IOMapper::In, CIEC_ANY::e_BOOL, paDeviceIndex),
      mBoard(paBoard),
      mChannel(paChannel) {
  }

  void MCCDigitalInHandle::set(const CIEC_ANY &) {
    // Inputs are read-only
  }

  void MCCDigitalInHandle::get(CIEC_ANY &paValue) {
    static_cast<CIEC_BOOL &>(paValue) = CIEC_BOOL(mBoard.getInputBit(mChannel));
  }

  bool MCCDigitalInHandle::checkChanged() {
    const bool value = mBoard.getInputBit(mChannel);
    if (value == mLastValue) {
      return false;
    }
    mLastValue = value;
    return true;
  }

  MCCDigitalOutHandle::MCCDigitalOutHandle(forte::io::IODeviceController *paController,
                                           size_t paDeviceIndex,
                                           MCC152Board &paBoard,
                                           uint8_t paChannel) :
      MCCHandle(paController, forte::io::IOMapper::Out, CIEC_ANY::e_BOOL, paDeviceIndex),
      mBoard(paBoard),
      mChannel(paChannel) {
  }

  void MCCDigitalOutHandle::set(const CIEC_ANY &paValue) {
    mValue = static_cast<const CIEC_BOOL &>(paValue);
    reportWriteResult(mBoard.writeOutputBit(mChannel, mValue), mBoard, "DO", mChannel);
  }

  void MCCDigitalOutHandle::get(CIEC_ANY &paValue) {
    static_cast<CIEC_BOOL &>(paValue) = CIEC_BOOL(mValue);
  }

  MCCAnalogOutHandle::MCCAnalogOutHandle(forte::io::IODeviceController *paController,
                                         size_t paDeviceIndex,
                                         MCC152Board &paBoard,
                                         uint8_t paChannel) :
      MCCHandle(paController, forte::io::IOMapper::Out, CIEC_ANY::e_WORD, paDeviceIndex),
      mBoard(paBoard),
      mChannel(paChannel) {
  }

  void MCCAnalogOutHandle::set(const CIEC_ANY &paValue) {
    const auto requested = static_cast<TForteWord>(static_cast<const CIEC_WORD &>(paValue));
    bool clamped = false;
    mCode = clampMCC152AoCode(requested, clamped);
    // Report a clamp once per episode instead of on every write of an oversized value.
    if (clamped && !mClampReported) {
      DEVLOG_WARNING("[MCCDaqHat] Address %u: AO_%u value clamped to 4095\n", mBoard.getAddress(), mChannel);
    }
    mClampReported = clamped;
    reportWriteResult(mBoard.writeAnalogCode(mChannel, mCode), mBoard, "AO", mChannel);
  }

  void MCCAnalogOutHandle::get(CIEC_ANY &paValue) {
    static_cast<CIEC_WORD &>(paValue) = CIEC_WORD(mCode);
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
