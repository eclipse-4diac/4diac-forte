/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include "PortAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "PortAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_PortAdapter, g_nStringIdPortAdapter)


const CStringDictionary::TStringId FORTE_PortAdapter::scm_anDataOutputNames[] = {g_nStringIdGPIO_Port_Addr};

const CStringDictionary::TStringId FORTE_PortAdapter::scm_anDataOutputTypeIds[] = {g_nStringIdDWORD};

const TForteInt16 FORTE_PortAdapter::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_PortAdapter::scm_anEventInputNames[] = {g_nStringIdMAPO};

const TDataIOID FORTE_PortAdapter::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_PortAdapter::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PortAdapter::scm_anEventOutputNames[] = {g_nStringIdMAP};


const SFBInterfaceSpec FORTE_PortAdapter::scm_stFBInterfaceSpecSocket = {
  1, scm_anEventInputNames, 0, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

const SFBInterfaceSpec FORTE_PortAdapter::scm_stFBInterfaceSpecPlug = {
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anEventInputNames, 0, scm_anEIWithIndexes,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr, nullptr,
  0, nullptr
};


