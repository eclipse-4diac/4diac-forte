/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny, Monika Wenger,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_word.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_word_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(WORD, g_nStringIdWORD)

size_t CIEC_WORD::getToStringBufferSize() const {
  return sizeof("65535");
}
