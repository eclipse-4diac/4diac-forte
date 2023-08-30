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
#include "forte_uint.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_uint_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(UINT, g_nStringIdUINT)

const CIEC_UINT::TValueType CIEC_UINT::scmMaxVal = std::numeric_limits<CIEC_UINT::TValueType>::max();

template CIEC_UINT &CIEC_UINT::operator=<>(const CIEC_ANY_INT &paValue);

template CIEC_UINT &CIEC_UINT::operator=<>(const CIEC_USINT &paValue);

size_t CIEC_UINT::getToStringBufferSize() const {
  return sizeof("65535");
}
