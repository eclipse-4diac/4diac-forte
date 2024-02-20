/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "IORevPiBusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IORevPiBusAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_IORevPiBusAdapter, g_nStringIdIORevPiBusAdapter)

const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anDataInputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anDataOutputNames[] = {g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex};

const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT};

const TDataIOID FORTE_IORevPiBusAdapter::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_IORevPiBusAdapter::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anEventInputNames[] = {g_nStringIdINITO};

const TDataIOID FORTE_IORevPiBusAdapter::scm_anEOWith[] = {2, 1, 0, 255};
const TForteInt16 FORTE_IORevPiBusAdapter::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_IORevPiBusAdapter::scm_anEventOutputNames[] = {g_nStringIdINIT};

const SFBInterfaceSpec FORTE_IORevPiBusAdapter::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_IORevPiBusAdapter::scm_stFBInterfaceSpecPlug = {
  1,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  scm_anEIWith, scm_anEIWithIndexes,  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};

const TForteUInt8 FORTE_IORevPiBusAdapter::scm_slaveConfigurationIO[] = { };
const TForteUInt8 FORTE_IORevPiBusAdapter::scm_slaveConfigurationIO_num = 0;


