/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "RegisterServiceHTTP.h"

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1_2);
USE_STRING_ID(CNF);
USE_STRING_ID(CreateRegisterID);
USE_STRING_ID(CreateUnRegisterID);
USE_STRING_ID(doneRegister);
USE_STRING_ID(doneUnregister);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_PERMIT_1);
USE_STRING_ID(E_SR);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(F_STRING_TO_WSTRING_1);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlagReg);
USE_STRING_ID(InitFlagUnReg);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(R);
USE_STRING_ID(Register);
USE_STRING_ID(registerService);
USE_STRING_ID(RegisterServiceAdp);
USE_STRING_ID(RegisterServiceHTTP);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(Unregister);
USE_STRING_ID(unregisterService);

DEFINE_FIRMWARE_FB(FORTE_RegisterServiceHTTP, STRID(RegisterServiceHTTP))

const TForteInt16 FORTE_RegisterServiceHTTP::scmEOWithIndexes[] = {-1};
const SAdapterInstanceDef FORTE_RegisterServiceHTTP::scmAdapterInstances[] = {
    {STRID(RegisterServiceAdp), STRID(registerService), false}};

const SFBInterfaceSpec FORTE_RegisterServiceHTTP::scmFBInterfaceSpec = {
    0, 0, nullptr, 0, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 1, scmAdapterInstances};

const SCFB_FBInstanceData FORTE_RegisterServiceHTTP::scmInternalFBs[] = {
    {STRID(Register), STRID(CLIENT_1_2)},
    {STRID(Unregister), STRID(CLIENT_1_2)},
    {STRID(InitFlagReg), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(CreateRegisterID), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(E_PERMIT_1), STRID(E_PERMIT)},
    {STRID(InitFlagUnReg), STRID(E_SR)},
    {STRID(CreateUnRegisterID), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING_1), STRID(F_STRING_TO_WSTRING)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
};

const SCFB_FBParameter FORTE_RegisterServiceHTTP::scmParamters[] = {
    {4, STRID(IN_1), "STRING#http["},
    {4, STRID(IN_3), "STRING#/register; POST; application/json]"},
    {8, STRID(IN_1), "STRING#http["},
    {8, STRID(IN_3), "STRING#/remove; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_RegisterServiceHTTP::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateRegisterID), STRID(CNF)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(REQ)), 5},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(CNF)), 5,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagReg), STRID(S)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateUnRegisterID), STRID(CNF)), 8,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING_1), STRID(REQ)), 9},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING_1), STRID(CNF)), 9,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagUnReg), STRID(S)), 7},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagUnReg), STRID(EO)), 7,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(INIT)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(INITO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT_1), STRID(EI)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT_1), STRID(EO)), 6,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(REQ)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(CNF)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagUnReg), STRID(R)), 7},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagReg), STRID(EO)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(INIT)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 3,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(REQ)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(INITO)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EI)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(CNF)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagReg), STRID(R)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(registerService)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateRegisterID), STRID(REQ)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(unregisterService)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateUnRegisterID), STRID(REQ)), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceHTTP::scmFannedOutEventConnections[] = {
    {1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(REQ)), 10},
    {3, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(REQ)), 10},
    {7, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(doneUnregister)),
     CCompositeFB::scmAdapterMarker | 0},
    {11, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(doneRegister)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_RegisterServiceHTTP::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagReg), STRID(Q)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(PERMIT)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateRegisterID), STRID(OUT)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(IN)), 5},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateUnRegisterID), STRID(OUT)), 8,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING_1), STRID(IN)), 9},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING_1), STRID(OUT)), 9,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(ID)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlagUnReg), STRID(Q)), 7,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(QI)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(OUT)), 5,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(ID)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(output)), 10,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(SD_1)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(serviceRegistryEntry)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(input)), 10},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateRegisterID), STRID(IN_2)), 4},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceHTTP::scmFannedOutDataConnections[] = {
    {0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Register), STRID(QI)), 0},
    {4, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT_1), STRID(PERMIT)), 6},
    {6, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Unregister), STRID(SD_1)), 1},
    {8, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CreateUnRegisterID), STRID(IN_2)), 8},
};

const SCFB_FBNData FORTE_RegisterServiceHTTP::scmFBNData = {
    11, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    9,  scmDataConnections, 4,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
