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

#include "MCC152Board.h"

#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  MCC152Board::MCC152Board(IMCCDaqHatApi &paApi, uint8_t paAddress) : MCCBoard(paApi, paAddress) {
  }

  MCC152Board::~MCC152Board() {
    close();
  }

  std::string MCC152Board::open(uint8_t paDirectionMask, uint8_t paPullEnable, uint8_t paPullConfig) {
    util::CCriticalRegion criticalRegion(mMutex);
    int result = mApi.mcc152Open(mAddress);
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc152DioReset(mAddress);
    }
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc152DioConfigWritePort(mAddress, DioConfigItem::PullEnable, paPullEnable);
    }
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc152DioConfigWritePort(mAddress, DioConfigItem::PullConfig, paPullConfig);
    }
    // Drive all outputs FALSE (the IEC default) before switching any bit to output, so no pulse appears at start.
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc152DioOutputWritePort(mAddress, 0);
    }
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc152DioConfigWritePort(mAddress, DioConfigItem::Direction, paDirectionMask);
    }
    if (result != IMCCDaqHatApi::scmSuccess) {
      mApi.mcc152Close(mAddress);
      return formatError(result);
    }
    mInputPort = 0;
    mOpen = true;
    return {};
  }

  void MCC152Board::close() {
    util::CCriticalRegion criticalRegion(mMutex);
    if (mOpen) {
      mApi.mcc152Close(mAddress);
      mOpen = false;
    }
  }

  bool MCC152Board::getInputBit(uint8_t paChannel) {
    util::CCriticalRegion criticalRegion(mMutex);
    return (paChannel < scmMCC152DioChannels) && ((mInputPort >> paChannel) & 1U) != 0;
  }

  int MCC152Board::writeOutputBit(uint8_t paChannel, bool paValue) {
    util::CCriticalRegion criticalRegion(mMutex);
    if (!mOpen) {
      return IMCCDaqHatApi::scmSuccess;
    }
    return track(mApi.mcc152DioOutputWriteBit(mAddress, paChannel, paValue ? 1 : 0));
  }

  int MCC152Board::writeAnalogCode(uint8_t paChannel, uint16_t paCode) {
    util::CCriticalRegion criticalRegion(mMutex);
    if (!mOpen) {
      return IMCCDaqHatApi::scmSuccess;
    }
    return track(mApi.mcc152OutputWriteCode(mAddress, paChannel, paCode));
  }

  int MCC152Board::readInputs() {
    uint8_t port = 0;
    const int result = mApi.mcc152DioInputReadPort(mAddress, port);
    if (result == IMCCDaqHatApi::scmSuccess) {
      mInputPort = port;
    }
    return result;
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
