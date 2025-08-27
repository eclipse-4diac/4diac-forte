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

#include "forte/io/eIO/eIX_fbt.h"

using namespace forte::core::literals;

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_eIX, "eIX"_STRID)

FORTE_eIX::FORTE_eIX(const forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CeInputFB<CIEC_BOOL>(paContainer, paInstanceNameId) {
}
