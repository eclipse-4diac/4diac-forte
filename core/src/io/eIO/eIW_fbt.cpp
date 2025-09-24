/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "forte/io/eIO/eIW_fbt.h"

using namespace forte::literals;

namespace forte::io {
  DEFINE_FIRMWARE_FB(FORTE_eIW, "eIW"_STRID)

  FORTE_eIW::FORTE_eIW(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CeInputFB<CIEC_WORD>(paContainer, paInstanceNameId) {
  }
} // namespace forte::io
