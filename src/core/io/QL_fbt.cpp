/*************************************************************************
 * Copyright (c) 2018, 2025 TU Wien/ACIN, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds intial implememtation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#include "QL_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(LWORD);
USE_STRING_ID(OUT);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QI);
USE_STRING_ID(QL);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_QL, STRID(QL))

const CStringDictionary::TStringId FORTE_QL::scmDataInputNames[] = {STRID(QI), STRID(PARAMS), STRID(OUT)};
const CStringDictionary::TStringId FORTE_QL::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(LWORD)};
const CStringDictionary::TStringId FORTE_QL::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_QL::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING)};
const TDataIOID FORTE_QL::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 2, scmWithListDelimiter};
const TForteInt16 FORTE_QL::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_QL::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_QL::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_QL::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_QL::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_QL::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_QL::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
const SFBInterfaceSpec FORTE_QL::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_QL::FORTE_QL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    COutputFB<CIEC_LWORD>(paContainer, scmFBInterfaceSpec, paInstanceNameId) {
};
