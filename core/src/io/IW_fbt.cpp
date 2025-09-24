/*************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#include "forte/io/IW_fbt.h"

using namespace forte::literals;

namespace forte::io {
  DEFINE_FIRMWARE_FB(FORTE_IW, "IW"_STRID)

  FORTE_IW::FORTE_IW(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CInputFB<CIEC_WORD>(paContainer, paInstanceNameId) {
  }
} // namespace forte::io
