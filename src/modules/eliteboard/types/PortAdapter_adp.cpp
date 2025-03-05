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

#include "PortAdapter_adp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "PortAdapter_adp_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_ADAPTER_TYPE(FORTE_PortAdapter, g_nStringIdPortAdapter)

const CStringDictionary::TStringId FORTE_PortAdapter::scmDataOutputNames[] = {g_nStringIdGPIO_Port_Addr};
const CStringDictionary::TStringId FORTE_PortAdapter::scmDataOutputTypeIds[] = {g_nStringIdDWORD};
const TForteInt16 FORTE_PortAdapter::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_PortAdapter::scmEventInputNames[] = {g_nStringIdMAPO};
const TDataIOID FORTE_PortAdapter::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_PortAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PortAdapter::scmEventOutputNames[] = {g_nStringIdMAP};

const CStringDictionary::TStringId FORTE_PortAdapter::scmEventInputTypeIds[] = {g_nStringIdEvent};
const CStringDictionary::TStringId FORTE_PortAdapter::scmEventOutputTypeIds[] = {g_nStringIdEvent};

const SFBInterfaceSpec FORTE_PortAdapter::scmFBInterfaceSpecSocket = {
  1, scmEventInputNames, scmEventInputTypeIds, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  0, nullptr, nullptr,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

const SFBInterfaceSpec FORTE_PortAdapter::scmFBInterfaceSpecPlug = {
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmEventInputNames, scmEventInputTypeIds, nullptr, scmEIWithIndexes,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_PortAdapter::readInputData(const TEventID paEIID) {
  if(isSocket()) {
    // nothing to do
  } else {
    switch(paEIID) {
      case scmEventMAPID: {
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default:
        break;
    }
  }
}

void FORTE_PortAdapter::writeOutputData(const TEventID paEIID) {
  if(isSocket()) {
    switch(paEIID) {
      case scmEventMAPID: {
        writeData(0, *mDOs[0], mDOConns[0]);
        break;
      }
      default:
        break;
    }
  } else {
    // nothing to do
  }
}
