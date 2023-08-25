/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_ulint.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_ulint_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ULINT, g_nStringIdULINT)

const CIEC_ULINT::TValueType CIEC_ULINT::scmMaxVal = std::numeric_limits<CIEC_ULINT::TValueType>::max();

template CIEC_ULINT &CIEC_ULINT::operator=<>(const CIEC_ANY_INT &paValue);

template CIEC_ULINT &CIEC_ULINT::operator=<>(const CIEC_USINT &paValue);

template CIEC_ULINT &CIEC_ULINT::operator=<>(const CIEC_UINT &paValue);

template CIEC_ULINT &CIEC_ULINT::operator=<>(const CIEC_UDINT &paValue);

size_t CIEC_ULINT::getToStringBufferSize() const {
  return sizeof("18446744073709551615");
}
