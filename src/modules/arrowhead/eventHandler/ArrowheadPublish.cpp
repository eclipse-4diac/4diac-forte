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

#include "ArrowheadPublish.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ArrowheadPublish_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_ArrowheadPublish, g_nStringIdArrowheadPublish)

const CStringDictionary::TStringId FORTE_ArrowheadPublish::scm_anDataInputNames[] = {g_nStringIdpublishEvent, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_ArrowheadPublish::scm_anDataInputTypeIds[] = {g_nStringIdPublishEvent, g_nStringIdWSTRING};

const TForteInt16 FORTE_ArrowheadPublish::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_ArrowheadPublish::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scm_anEventInputNames[] = {g_nStringIdpublish};

const TForteInt16 FORTE_ArrowheadPublish::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scm_anEventOutputNames[] = {g_nStringIdpublished};

const SAdapterInstanceDef FORTE_ArrowheadPublish::scm_astAdapterInstances[] = {
{g_nStringIdArrowheadPublishAdp, g_nStringIdArrowheadPublishAdp, true }};

const SFBInterfaceSpec FORTE_ArrowheadPublish::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBConnectionData FORTE_ArrowheadPublish::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdpublish), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdArrowheadPublishAdp, g_nStringIdpublish), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdArrowheadPublishAdp, g_nStringIdpublished), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdpublished), -1},
};

const SCFB_FBConnectionData FORTE_ArrowheadPublish::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdpublishEvent), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdArrowheadPublishAdp, g_nStringIdpublishEvent), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdendpoint), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdArrowheadPublishAdp, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBNData FORTE_ArrowheadPublish::scm_stFBNData = {
  0, 0,
  2, scm_astEventConnections,
  0, 0,
  2, scm_astDataConnections,
  0, 0,
  0, 0
};


