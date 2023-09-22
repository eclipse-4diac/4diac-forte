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

#include "EBBusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EBBusAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(EmbrickBusAdapter, g_nStringIdEBBusAdapter)

const CStringDictionary::TStringId EmbrickBusAdapter::scmDataInputNames[] =
    { g_nStringIdQO };

const CStringDictionary::TStringId EmbrickBusAdapter::scmDataInputTypeIds[] =
    { g_nStringIdBOOL };

const CStringDictionary::TStringId EmbrickBusAdapter::scmDataOutputNames[] =
    { g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex,
        g_nStringIdUpdateInterval };

const CStringDictionary::TStringId EmbrickBusAdapter::scmDataOutputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT, g_nStringIdUINT };

const TDataIOID EmbrickBusAdapter::scmEIWith[] = { 0, scmWithListDelimiter };
const TForteInt16 EmbrickBusAdapter::scmEIWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId EmbrickBusAdapter::scmEventInputNames[] =
    { g_nStringIdINITO };

const TDataIOID EmbrickBusAdapter::scmEOWith[] = { 2, 3, 1, 0, scmWithListDelimiter };
const TForteInt16 EmbrickBusAdapter::scmEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId EmbrickBusAdapter::scmEventOutputNames[] =
    { g_nStringIdINIT };

const SFBInterfaceSpec EmbrickBusAdapter::scmFBInterfaceSpecSocket = { 1,
    scmEventInputNames, scmEIWith, scmEIWithIndexes, 1,
    scmEventOutputNames, scmEOWith, scmEOWithIndexes, 1,
    scmDataInputNames, scmDataInputTypeIds, 4, scmDataOutputNames,
    scmDataOutputTypeIds,
    0, nullptr,
    0, nullptr };

const SFBInterfaceSpec EmbrickBusAdapter::scmFBInterfaceSpecPlug = { 1,
    scmEventOutputNames, scmEOWith, scmEOWithIndexes, 1,
    scmEventInputNames, scmEIWith, scmEIWithIndexes, 4,
    scmDataOutputNames, scmDataOutputTypeIds, 1, scmDataInputNames,
    scmDataInputTypeIds,
    0, nullptr,
    0, nullptr};

const TForteUInt8 EmbrickBusAdapter::scmSlaveConfigurationIO[] = { 3 };
const TForteUInt8 EmbrickBusAdapter::scmSlaveConfigurationIONum = 1;

