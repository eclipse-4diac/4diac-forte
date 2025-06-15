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

#include "PublishEventOpcUa.h"

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(ArrowheadPublishAdp);
USE_STRING_ID(CLIENT_1_0);
USE_STRING_ID(CNF);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_CTU_1);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(E_DELAY_1);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_SWITCH_1_1);
USE_STRING_ID(E_SWITCH_2);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(G);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlag);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(publish);
USE_STRING_ID(published);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEventOpcUa);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(Query);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(START);

DEFINE_FIRMWARE_FB(FORTE_PublishEventOpcUa, STRID(PublishEventOpcUa))

namespace {
  const auto cSocketNameIds = std::array{STRID(publishEvent)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = {},
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = {},
  };
} // namespace

const SCFB_FBInstanceData FORTE_PublishEventOpcUa::scmInternalFBs[] = {
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(E_SWITCH_2), STRID(E_SWITCH)},
    {STRID(E_SWITCH_1_1), STRID(E_SWITCH)},
    {STRID(E_CTU_1), STRID(E_CTU)},
    {STRID(E_DELAY_1), STRID(E_DELAY)},
    {STRID(Query), STRID(CLIENT_1_0)},
};

const SCFB_FBParameter FORTE_PublishEventOpcUa::scmParamters[] = {
    {2, STRID(IN_1), "STRING#opc_ua[CALL_METHOD; "},
    {2, STRID(IN_3), "STRING##; /Objects/2:EventHandler/2:publish]"},
    {7, STRID(PV), "UINT#3"},
    {8, STRID(DT), "T#250ms"},
};

const SCFB_FBConnectionData FORTE_PublishEventOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(CNF)), 2,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 3},
    {STRID(ANYToJSON), STRID(CNF)), 0,
     STRID(APPEND_STRING_3), STRID(REQ)), 2},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 3,
     STRID(InitFlag), STRID(S)), 1},
    {STRID(publishEvent), STRID(publish)), CCompositeFB::scmAdapterMarker | 0,
     STRID(ANYToJSON), STRID(REQ)), 0},
    {STRID(E_SWITCH_2), STRID(EO1)), 5,
     STRID(E_CTU_1), STRID(R)), 7},
    {STRID(E_SWITCH_1_1), STRID(EO1)), 6,
     STRID(E_CTU_1), STRID(R)), 7},
    {STRID(E_SWITCH_2), STRID(EO0)), 5,
     STRID(E_CTU_1), STRID(CU)), 7},
    {STRID(E_CTU_1), STRID(CUO)), 7,
     STRID(E_SWITCH_1_1), STRID(EI)), 6},
    {STRID(E_SWITCH_1_1), STRID(EO0)), 6,
     STRID(E_DELAY_1), STRID(START)), 8},
    {STRID(InitFlag), STRID(EO)), 1,
     STRID(Query), STRID(INIT)), 9},
    {STRID(E_PERMIT), STRID(EO)), 4,
     STRID(Query), STRID(REQ)), 9},
    {STRID(E_DELAY_1), STRID(EO)), 8,
     STRID(Query), STRID(REQ)), 9},
    {STRID(Query), STRID(INITO)), 9,
     STRID(E_PERMIT), STRID(EI)), 4},
    {STRID(Query), STRID(CNF)), 9,
     STRID(E_SWITCH_2), STRID(EI)), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventOpcUa::scmFannedOutEventConnections[] = {
    {4, STRID(publishEvent), STRID(published)), CCompositeFB::scmAdapterMarker | 0},
    {4, STRID(InitFlag), STRID(R)), 1},
    {5, STRID(InitFlag), STRID(R)), 1},
    {5, STRID(publishEvent), STRID(published)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_PublishEventOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(OUT)), 2,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 3},
    {STRID(InitFlag), STRID(Q)), 1,
     STRID(E_PERMIT), STRID(PERMIT)), 4},
    {STRID(publishEvent), STRID(publishEvent)), CCompositeFB::scmAdapterMarker | 0,
     STRID(ANYToJSON), STRID(input)), 0},
    {STRID(publishEvent), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     STRID(APPEND_STRING_3), STRID(IN_2)), 2},
    {STRID(E_CTU_1), STRID(Q)), 7,
     STRID(E_SWITCH_1_1), STRID(G)), 6},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 3,
     STRID(Query), STRID(ID)), 9},
    {STRID(ANYToJSON), STRID(output)), 0,
     STRID(Query), STRID(SD_1)), 9},
    {STRID(Query), STRID(QO)), 9,
     STRID(E_SWITCH_2), STRID(G)), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventOpcUa::scmFannedOutDataConnections[] = {
    {1, STRID(Query), STRID(QI)), 9},
};

const SCFB_FBNData FORTE_PublishEventOpcUa::scmFBNData = {
    10, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    8,  scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
