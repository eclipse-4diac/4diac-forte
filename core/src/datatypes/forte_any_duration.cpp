/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Hesam Rezaee
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/datatypes/forte_any_duration.h"
#include <format>

namespace forte {
  // change time elements to string

  void CIEC_ANY_DURATION::timeElementsToString(std::string &paTargetBuf,
                                               int64_t paTimeElement,
                                               const std::string &paUnit) const {
    if (paTimeElement != 0) {
      std::format_to(std::back_inserter(paTargetBuf), "{}", paTimeElement);
      paTargetBuf += paUnit;
    }
  }
} // namespace forte
