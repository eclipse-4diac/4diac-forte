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

#include "Slave2301.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Slave2301_gen.cpp"
#endif

#include "../handler/bus.h"

DEFINE_FIRMWARE_FB(EmbrickSlave2301, g_nStringIdEBSlave2301)

const CStringDictionary::TStringId EmbrickSlave2301::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdRelay_1, g_nStringIdRelay_2, g_nStringIdRelay_3,
    g_nStringIdRelay_4, g_nStringIdRelay_5, g_nStringIdRelay_6,
    g_nStringIdUpdateInterval };

const CStringDictionary::TStringId EmbrickSlave2301::scm_anDataInputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdUINT };

const CStringDictionary::TStringId EmbrickSlave2301::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId EmbrickSlave2301::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 EmbrickSlave2301::scm_anEIWithIndexes[] = { 0 };
const TDataIOID EmbrickSlave2301::scm_anEIWith[] = { 1, 2, 3, 4, 5, 6, 0, 255 };
const CStringDictionary::TStringId EmbrickSlave2301::scm_anEventInputNames[] = {
    g_nStringIdMAP };

const TDataIOID EmbrickSlave2301::scm_anEOWith[] = { 0, 255, 0, 1, 255 };
const TForteInt16 EmbrickSlave2301::scm_anEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId EmbrickSlave2301::scm_anEventOutputNames[] = {
    g_nStringIdMAPO, g_nStringIdIND };

const SAdapterInstanceDef EmbrickSlave2301::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true }, {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterIn, false } };

const SFBInterfaceSpec EmbrickSlave2301::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 8,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 2, scm_astAdapterInstances };

const TForteUInt8 EmbrickSlave2301::scmSlaveConfigurationIO[] = { 7 };
const TForteUInt8 EmbrickSlave2301::scmSlaveConfigurationIONum = 1;

void EmbrickSlave2301::initHandles() {
  // Initialize handles
  int iCount = 0;
  int oCount = 6;
  int iOffset = 1;
  int oOffset = iOffset + iCount;

  for (int i = 0; i < oCount; i++) {
    EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
        *static_cast<CIEC_WSTRING*>(getDI(oOffset + i)), forte::core::io::IOMapper::Out, mIndex,
        EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(&desc);
  }
}

