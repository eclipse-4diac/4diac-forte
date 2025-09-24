/*************************************************************************
 * Copyright (c) 2018, 2025 TU Wien/ACIN, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds initial implementation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *************************************************************************/

#pragma once

#include "forte/io/inputfb.h"
#include "forte/datatypes/forte_byte.h"

namespace forte::io {
  class FORTE_IB final : public CInputFB<CIEC_BYTE> {
      DECLARE_FIRMWARE_FB(FORTE_IB)

    public:
      FORTE_IB(StringId paInstanceNameId, CFBContainer &paContainer);
  };
} // namespace forte::io
