/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "BusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BusAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(EmbrickBusAdapter, g_nStringIdEBBusAdapter)

const CStringDictionary::TStringId EmbrickBusAdapter::scm_anDataInputNames[] =
    { g_nStringIdQO };

const CStringDictionary::TStringId EmbrickBusAdapter::scm_anDataInputTypeIds[] =
    { g_nStringIdBOOL };

const CStringDictionary::TStringId EmbrickBusAdapter::scm_anDataOutputNames[] =
    { g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex,
        g_nStringIdUpdateInterval };

const CStringDictionary::TStringId EmbrickBusAdapter::scm_anDataOutputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT, g_nStringIdUINT };

const TDataIOID EmbrickBusAdapter::scm_anEIWith[] = { 0, 255 };
const TForteInt16 EmbrickBusAdapter::scm_anEIWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId EmbrickBusAdapter::scm_anEventInputNames[] =
    { g_nStringIdINITO };

const TDataIOID EmbrickBusAdapter::scm_anEOWith[] = { 2, 3, 1, 0, 255 };
const TForteInt16 EmbrickBusAdapter::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId EmbrickBusAdapter::scm_anEventOutputNames[] =
    { g_nStringIdINIT };

const SFBInterfaceSpec EmbrickBusAdapter::scm_stFBInterfaceSpecSocket = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 1,
    scm_anDataInputNames, scm_anDataInputTypeIds, 4, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 0, 0 };

const SFBInterfaceSpec EmbrickBusAdapter::scm_stFBInterfaceSpecPlug = { 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 4,
    scm_anDataOutputNames, scm_anDataOutputTypeIds, 1, scm_anDataInputNames,
    scm_anDataInputTypeIds, 0, 0 };

const TForteUInt8 EmbrickBusAdapter::scmSlaveConfigurationIO[] = { 3 };
const TForteUInt8 EmbrickBusAdapter::scmSlaveConfigurationIONum = 1;

