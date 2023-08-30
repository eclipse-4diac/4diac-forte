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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Rene Smodic, Ingo Hegny,
 *    Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_udint.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_udint_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(UDINT, g_nStringIdUDINT)

const CIEC_UDINT::TValueType CIEC_UDINT::scmMaxVal = std::numeric_limits<CIEC_UDINT::TValueType>::max();

template CIEC_UDINT &CIEC_UDINT::operator=<>(const CIEC_ANY_INT &paValue);

template CIEC_UDINT &CIEC_UDINT::operator=<>(const CIEC_USINT &paValue);

template CIEC_UDINT &CIEC_UDINT::operator=<>(const CIEC_UINT &paValue);

size_t CIEC_UDINT::getToStringBufferSize() const {
  return sizeof("4294967295");
}
