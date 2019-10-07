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

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(ULINT, g_nStringIdULINT)

const CIEC_ULINT::TValueType CIEC_ULINT::scm_nMaxVal = std::numeric_limits<CIEC_ULINT::TValueType>::max();

#endif
