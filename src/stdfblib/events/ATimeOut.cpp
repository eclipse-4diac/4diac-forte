/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "ATimeOut.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ATimeOut_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_ATimeOut, g_nStringIdATimeOut)

const CStringDictionary::TStringId FORTE_ATimeOut::scm_anDataOutputNames[] = {g_nStringIdDT};

const CStringDictionary::TStringId FORTE_ATimeOut::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 FORTE_ATimeOut::scm_anEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_ATimeOut::scm_anEventInputNames[] = {g_nStringIdTimeOut};

const TDataIOID FORTE_ATimeOut::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_ATimeOut::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_ATimeOut::scm_anEventOutputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const SFBInterfaceSpec FORTE_ATimeOut::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_ATimeOut::scm_stFBInterfaceSpecPlug = {
  2,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  0, 0,  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



