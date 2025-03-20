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
#include "forte_int.h"

USE_STRING_ID(INT);


DEFINE_FIRMWARE_DATATYPE(INT, STRID(INT))

template CIEC_INT &CIEC_INT::operator=<>(const CIEC_SINT &paValue);

template CIEC_INT &CIEC_INT::operator=<>(const CIEC_USINT &paValue);

const CStringDictionary::TStringId forte::CDataTypeTrait<CIEC_INT>::scmDataTypeName = STRID(INT);
