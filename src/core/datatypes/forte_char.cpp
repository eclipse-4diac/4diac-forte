/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_char.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_char_gen.cpp"
#endif
#include <limits>

DEFINE_FIRMWARE_DATATYPE(CHAR, g_nStringIdCHAR)

const CIEC_CHAR::TValueType CIEC_CHAR::scm_nMaxVal = std::numeric_limits<CIEC_CHAR::TValueType>::max();