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

#include "forte/io/QL_fbt.h"
using namespace forte::literals;

namespace forte::io {
  DEFINE_FIRMWARE_FB(FORTE_QL, "QL"_STRID)

  FORTE_QL::FORTE_QL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      COutputFB<CIEC_LWORD>(paContainer, paInstanceNameId) {};
} // namespace forte::io
