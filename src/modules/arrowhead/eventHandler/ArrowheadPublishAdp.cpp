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

#include "ArrowheadPublishAdp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ArrowheadPublishAdp_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_ArrowheadPublishAdp, g_nStringIdArrowheadPublishAdp)

const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scm_anDataOutputNames[] = {g_nStringIdpublishEvent, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scm_anDataOutputTypeIds[] = {g_nStringIdPublishEvent, g_nStringIdWSTRING};

const TForteInt16 FORTE_ArrowheadPublishAdp::scm_anEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scm_anEventInputNames[] = {g_nStringIdpublished};

const TDataIOID FORTE_ArrowheadPublishAdp::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FORTE_ArrowheadPublishAdp::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scm_anEventOutputNames[] = {g_nStringIdpublish};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scm_stFBInterfaceSpecPlug = {
  1,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  0, 0,  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



