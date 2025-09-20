/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#include "membdataconn.h"

namespace forte::internal {
  void CMemberDataConnection::getSourcePortName(TNameIdentifier &paResult) const {
    CDataConnection::getSourcePortName(paResult);
    paResult.insert(paResult.end(), cmMemberName.cbegin(), cmMemberName.cend());
  }
} // namespace forte::internal
