/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure,
 *******************************************************************************/
#include "forte_state.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_state_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(STATE, g_nStringIdSTATE)

const CIEC_STATE::TValueType CIEC_STATE::scmMaxVal = std::numeric_limits<CIEC_STATE::TValueType>::max();

size_t CIEC_STATE::getToStringBufferSize() const {
    return sizeof("65535");
}
