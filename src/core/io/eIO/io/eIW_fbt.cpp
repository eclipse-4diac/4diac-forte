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

#include "eIW_fbt.h"

USE_STRING_ID(eIW);
USE_STRING_ID(eGenAdapter);

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_eIW, STRID(eIW))

FORTE_eIW::FORTE_eIW(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CeInputFB<CIEC_WORD>(paContainer, paInstanceNameId) {}