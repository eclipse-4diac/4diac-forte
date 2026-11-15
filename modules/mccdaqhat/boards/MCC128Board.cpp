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

#include "MCC128Board.h"

#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  MCC128Board::MCC128Board(IMCCDaqHatApi &paApi, uint8_t paAddress) : MCCBoard(paApi, paAddress) {
  }

  MCC128Board::~MCC128Board() {
    close();
  }

  std::string MCC128Board::open(uint8_t paMode, uint8_t paRange, const TChannelUsage &paUsed) {
    util::CCriticalRegion criticalRegion(mMutex);
    int result = mApi.mcc128Open(mAddress);
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc128InputModeWrite(mAddress, paMode);
    }
    if (result == IMCCDaqHatApi::scmSuccess) {
      result = mApi.mcc128InputRangeWrite(mAddress, paRange);
    }
    if (result != IMCCDaqHatApi::scmSuccess) {
      mApi.mcc128Close(mAddress);
      return formatError(result);
    }
    mUsed = paUsed;
    mCodes.fill(0);
    mOpen = true;
    return {};
  }

  void MCC128Board::close() {
    util::CCriticalRegion criticalRegion(mMutex);
    if (mOpen) {
      mApi.mcc128Close(mAddress);
      mOpen = false;
    }
  }

  uint16_t MCC128Board::getCode(uint8_t paChannel) {
    util::CCriticalRegion criticalRegion(mMutex);
    return (paChannel < scmMCC128Channels) ? mCodes[paChannel] : 0;
  }

  int MCC128Board::readInputs() {
    for (uint8_t channel = 0; channel < scmMCC128Channels; ++channel) {
      if (!mUsed[channel]) {
        continue;
      }
      uint16_t code = 0;
      const int result = mApi.mcc128InputReadCode(mAddress, channel, code);
      if (result != IMCCDaqHatApi::scmSuccess) {
        return result;
      }
      mCodes[channel] = code;
    }
    return IMCCDaqHatApi::scmSuccess;
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
