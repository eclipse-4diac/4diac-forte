/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_arrowheadevent.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_arrowheadevent_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ArrowheadEvent, g_nStringIdArrowheadEvent);

CIEC_ArrowheadEvent::CIEC_ArrowheadEvent() :
    CIEC_STRUCT(g_nStringIdArrowheadEvent, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadEvent::scmElementTypes[] = { g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDATE_AND_TIME, g_nStringIdARRAY, 10, g_nStringIdWSTRING};
const CStringDictionary::TStringId CIEC_ArrowheadEvent::scmElementNames[] =
  { g_nStringIdtype, g_nStringIdpayload, g_nStringIdtimestamp, g_nStringIdeventMetadata };
