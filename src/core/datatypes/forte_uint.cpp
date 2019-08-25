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

DEFINE_FIRMWARE_DATATYPE(UINT, g_nStringIdUINT)

const CIEC_UINT::TValueType CIEC_UINT::scm_nMaxVal = std::numeric_limits<CIEC_UINT::TValueType>::max();
