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
 *   Jose Cabral - Modification to double
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *************************************************************************/

#include "core/io/ID_fbt.h"

using namespace forte::core::literals;

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_ID, "ID"_STRID)

FORTE_ID::FORTE_ID(const forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CInputFB<CIEC_DWORD>(paContainer, paInstanceNameId) {};
