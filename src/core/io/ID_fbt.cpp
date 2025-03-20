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

#include "ID_fbt.h"

USE_STRING_ID(ID);

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_ID, STRID(ID))


FORTE_ID::FORTE_ID(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CInputFB<CIEC_DWORD>(paContainer, paInstanceNameId) {
};
