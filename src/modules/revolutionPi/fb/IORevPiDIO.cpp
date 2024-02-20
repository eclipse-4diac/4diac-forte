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

#include "IORevPiDIO.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IORevPiDIO_gen.cpp"
#endif

#include "../RevPiController.h"

DEFINE_FIRMWARE_FB(FORTE_IORevPiDIO, g_nStringIdIORevPiDIO)

const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdDigitalInput_1, g_nStringIdDigitalInput_2,
    g_nStringIdDigitalInput_3, g_nStringIdDigitalInput_4,
    g_nStringIdDigitalInput_5, g_nStringIdDigitalInput_6,
    g_nStringIdDigitalInput_7, g_nStringIdDigitalInput_8,
    g_nStringIdDigitalInput_9, g_nStringIdDigitalInput_10,
    g_nStringIdDigitalInput_11, g_nStringIdDigitalInput_12,
    g_nStringIdDigitalInput_13, g_nStringIdDigitalInput_14,
    g_nStringIdDigitalOutput_1, g_nStringIdDigitalOutput_2,
    g_nStringIdDigitalOutput_3, g_nStringIdDigitalOutput_4,
    g_nStringIdDigitalOutput_5, g_nStringIdDigitalOutput_6,
    g_nStringIdDigitalOutput_7, g_nStringIdDigitalOutput_8,
    g_nStringIdDigitalOutput_9, g_nStringIdDigitalOutput_10,
    g_nStringIdDigitalOutput_11, g_nStringIdDigitalOutput_12,
    g_nStringIdDigitalOutput_13, g_nStringIdDigitalOutput_14 };

const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anDataInputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING };

const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anDataOutputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 FORTE_IORevPiDIO::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_IORevPiDIO::scm_anEIWith[] = { 1, 2, 5, 3, 4, 6, 7, 8, 15,
    16, 17, 18, 19, 20, 21, 22, 0, 14, 13, 12, 11, 10, 9, 23, 24, 25, 26, 27,
    28, 255 };
const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anEventInputNames[] = {
    g_nStringIdMAP };

const TDataIOID FORTE_IORevPiDIO::scm_anEOWith[] = { 0, 255, 0, 1, 255 };
const TForteInt16 FORTE_IORevPiDIO::scm_anEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId FORTE_IORevPiDIO::scm_anEventOutputNames[] =
    { g_nStringIdMAPO, g_nStringIdIND };

const SAdapterInstanceDef FORTE_IORevPiDIO::scm_astAdapterInstances[] = { {
    g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterOut, true }, {
    g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterIn, false } };

const SFBInterfaceSpec FORTE_IORevPiDIO::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 29,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 2, scm_astAdapterInstances };

const TForteUInt8 FORTE_IORevPiDIO::scm_slaveConfigurationIO[] = { };
const TForteUInt8 FORTE_IORevPiDIO::scm_slaveConfigurationIO_num = 0;

void FORTE_IORevPiDIO::initHandles() {
  // Initialize handles
  int iCount = 14;
  int oCount = 14;
  int iOffset = 1;
  int oOffset = iOffset + iCount;

  for (int i = 0; i < iCount; i++) {
    RevPiController::HandleDescriptor desc = RevPiController::HandleDescriptor(
        *static_cast<CIEC_WSTRING*>(getDI(iOffset + i)), forte::core::IO::IOMapper::In, index,
        CIEC_ANY::e_BOOL, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(&desc);
  }

  for (int i = 0; i < oCount; i++) {
    RevPiController::HandleDescriptor desc = RevPiController::HandleDescriptor(
        *static_cast<CIEC_WSTRING*>(getDI(oOffset + i)), forte::core::IO::IOMapper::Out, index,
        CIEC_ANY::e_BOOL, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(&desc);
  }
}

