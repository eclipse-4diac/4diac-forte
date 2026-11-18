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

#include "MCCBoard.h"

#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::mccdaqhat {

  MCCBoard::MCCBoard(IMCCDaqHatApi &paApi, uint8_t paAddress) : mApi(paApi), mAddress(paAddress) {
  }

  bool MCCBoard::poll() {
    util::CCriticalRegion criticalRegion(mMutex);
    if (!mOpen) {
      return true;
    }
    track(readInputs());
    return mConsecutiveErrors < scmMaxConsecutiveErrors;
  }

  std::string MCCBoard::getLastError() {
    util::CCriticalRegion criticalRegion(mMutex);
    return mLastError;
  }

  int MCCBoard::track(int paResult) {
    if (paResult == IMCCDaqHatApi::scmSuccess) {
      mConsecutiveErrors = 0;
    } else {
      ++mConsecutiveErrors;
      mLastError = formatError(paResult);
    }
    return paResult;
  }

  std::string MCCBoard::formatError(int paResult) const {
    return "Address " + std::to_string(mAddress) + ": " + mApi.errorMessage(paResult);
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
