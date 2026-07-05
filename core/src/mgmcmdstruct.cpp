/*******************************************************************************
 * Copyright (c) 2026 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "forte/mgmcmdstruct.h"

namespace forte {

  void SManagementCMD::clear() {
    mCMD = EMGMCommandType::INVALID;
    mDestination = StringId{};
    mFirstParam.clear();
    mSecondParam.clear();
    mAdditionalParams.clear();
    mID.clear();
    mMonitorResponse.clear();
  }
} // namespace forte
