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

#include "forte_eventfilter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_eventfilter_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(EventFilter, g_nStringIdEventFilter);

CIEC_EventFilter::CIEC_EventFilter() :
    CIEC_STRUCT(g_nStringIdEventFilter, 8, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_EventFilter::scmElementTypes[] =
  { g_nStringIdWSTRING, g_nStringIdArrowheadSystem, g_nStringIdARRAY, 10, g_nStringIdArrowheadSystem, g_nStringIdDATE_AND_TIME, g_nStringIdDATE_AND_TIME,
    g_nStringIdARRAY, 10, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdBOOL };
const CStringDictionary::TStringId CIEC_EventFilter::scmElementNames[] = { g_nStringIdeventType, g_nStringIdconsumer, g_nStringIdsources, g_nStringIdstartDate,
  g_nStringIdendDate, g_nStringIdfilterMetada, g_nStringIdnotifyUri, g_nStringIdmatchMetada };
