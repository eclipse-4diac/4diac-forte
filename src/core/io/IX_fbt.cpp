/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/
#include "core/io/IX_fbt.h"

using namespace forte::core::literals;

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_IX, "IX"_STRID)

FORTE_IX::FORTE_IX(const forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CInputFB<CIEC_BOOL>(paContainer, paInstanceNameId) {
}
