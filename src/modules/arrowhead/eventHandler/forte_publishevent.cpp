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

#include "forte_publishevent.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_publishevent_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(PublishEvent, g_nStringIdPublishEvent);

CIEC_PublishEvent::CIEC_PublishEvent() :
    CIEC_STRUCT(g_nStringIdPublishEvent, 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_PublishEvent::scmElementTypes[] = { g_nStringIdArrowheadSystem, g_nStringIdArrowheadEvent, g_nStringIdWSTRING};
const CStringDictionary::TStringId CIEC_PublishEvent::scmElementNames[] =
  { g_nStringIdsource, g_nStringIdevent, g_nStringIddeliveryCompleteUri};
