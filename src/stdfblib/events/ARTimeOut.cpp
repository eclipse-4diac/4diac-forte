/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "ARTimeOut.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ARTimeOut_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_ADAPTER_TYPE(FORTE_ARTimeOut, g_nStringIdARTimeOut)

const CStringDictionary::TStringId FORTE_ARTimeOut::scm_anDataOutputNames[] = {g_nStringIdDT};
const CStringDictionary::TStringId FORTE_ARTimeOut::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};
const TForteInt16 FORTE_ARTimeOut::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_ARTimeOut::scm_anEventInputNames[] = {g_nStringIdTimeOut};
const TDataIOID FORTE_ARTimeOut::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ARTimeOut::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ARTimeOut::scm_anEventOutputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const SFBInterfaceSpec FORTE_ARTimeOut::scm_stFBInterfaceSpecSocket = {
  1, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

const SFBInterfaceSpec FORTE_ARTimeOut::scm_stFBInterfaceSpecPlug = {
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};


