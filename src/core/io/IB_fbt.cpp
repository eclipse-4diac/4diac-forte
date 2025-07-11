/*************************************************************************
 * Copyright (c) 2025 TU Wien/ACIN, Johannes Kepler University Linz
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
 *************************************************************************/

#include "core/io/IB_fbt.h"

USE_STRING_ID(IB);

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_IB, STRID(IB))

FORTE_IB::FORTE_IB(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CInputFB<CIEC_BYTE>(paContainer, paInstanceNameId) {
}
