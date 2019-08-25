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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_sint.h"

DEFINE_FIRMWARE_DATATYPE(SINT, g_nStringIdSINT)

const CIEC_SINT::TValueType CIEC_SINT::scm_nMinVal = std::numeric_limits<CIEC_SINT::TValueType>::min();
const CIEC_SINT::TValueType CIEC_SINT::scm_nMaxVal = std::numeric_limits<CIEC_SINT::TValueType>::max();
