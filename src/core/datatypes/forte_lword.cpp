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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_lword_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(LWORD, g_nStringIdLWORD)

size_t CIEC_LWORD::getToStringBufferSize() const {
  return sizeof("18446744073709551615");
}
