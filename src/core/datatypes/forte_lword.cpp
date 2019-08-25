/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkiumians, Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_lword.h"

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(LWORD, g_nStringIdLWORD)

const CIEC_LWORD::TValueType CIEC_LWORD::scm_nMaxVal = std::numeric_limits<CIEC_LWORD::TValueType>::max();

#endif
