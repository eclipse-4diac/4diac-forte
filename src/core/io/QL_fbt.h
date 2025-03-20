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

#include "outputfb.h"
#include "forte_lword.h"

class FORTE_QL final : public forte::core::io::COutputFB<CIEC_LWORD> {
  DECLARE_FIRMWARE_FB(FORTE_QL)

  public:
    FORTE_QL(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

};

