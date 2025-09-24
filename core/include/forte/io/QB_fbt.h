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
 *   Martin Melik-Merkumians - adds intial implememtation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include "forte/io/outputfb.h"
#include "forte/datatypes/forte_byte.h"

namespace forte::io {
  class FORTE_QB final : public COutputFB<CIEC_BYTE> {
      DECLARE_FIRMWARE_FB(FORTE_QB)

    public:
      FORTE_QB(StringId paInstanceNameId, CFBContainer &paContainer);
  };
} // namespace forte::io
