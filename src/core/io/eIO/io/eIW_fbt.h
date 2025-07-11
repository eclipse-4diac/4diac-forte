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

#pragma once

#include "core/io/eIO/io/einputfb.h"
#include "core/datatypes/forte_word.h"

USE_STRING_ID(eIW);

/*! /brief generic class for eIW function blocks providing (event-triggerd) access to a physical input
 *
 */
class FORTE_eIW final : public forte::core::io::CeInputFB<CIEC_WORD> {
    DECLARE_FIRMWARE_FB(FORTE_eIW)

  public:
    FORTE_eIW(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
};
