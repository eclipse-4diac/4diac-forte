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
 *    Monika Wenger, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_lint.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_lint_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(LINT, g_nStringIdLINT);

const CIEC_LINT::TValueType CIEC_LINT::scmMinVal = std::numeric_limits<CIEC_LINT::TValueType>::min();
const CIEC_LINT::TValueType CIEC_LINT::scmMaxVal = std::numeric_limits<CIEC_LINT::TValueType>::max();

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_ANY_INT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_SINT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_INT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_DINT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_USINT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_UINT &paValue);

template CIEC_LINT &CIEC_LINT::operator=<>(const CIEC_UDINT &paValue);

size_t CIEC_LINT::getToStringBufferSize() const {
  return sizeof("-9223372036854775808");
}
