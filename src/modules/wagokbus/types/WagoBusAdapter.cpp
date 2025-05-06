/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "WagoBusAdapter.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(Index);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MasterId);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(UINT);
USE_STRING_ID(WagoBusAdapter);

DEFINE_ADAPTER_TYPE(FORTE_WagoBusAdapter, STRID(WagoBusAdapter))

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataInputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataOutputNames[] = {STRID(QI), STRID(MasterId),
                                                                                 STRID(Index)};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UINT),
                                                                                   STRID(UINT)};
const TDataIOID FORTE_WagoBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventInputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_WagoBusAdapter::scmEOWith[] = {2, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventOutputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventOutputTypeIds[] = {STRID(EInit)};

const SFBInterfaceSpec FORTE_WagoBusAdapter::scmFBInterfaceSpecSocket = {1,
                                                                         scmEventInputNames,
                                                                         scmEventInputTypeIds,
                                                                         scmEIWith,
                                                                         scmEIWithIndexes,
                                                                         1,
                                                                         scmEventOutputNames,
                                                                         scmEventOutputTypeIds,
                                                                         scmEOWith,
                                                                         scmEOWithIndexes,
                                                                         1,
                                                                         scmDataInputNames,
                                                                         scmDataInputTypeIds,
                                                                         3,
                                                                         scmDataOutputNames,
                                                                         scmDataOutputTypeIds,
                                                                         0,
                                                                         nullptr,
                                                                         0,
                                                                         nullptr};

const SFBInterfaceSpec FORTE_WagoBusAdapter::scmFBInterfaceSpecPlug = {1,
                                                                       scmEventOutputNames,
                                                                       scmEventOutputTypeIds,
                                                                       scmEOWith,
                                                                       scmEOWithIndexes,
                                                                       1,
                                                                       scmEventInputNames,
                                                                       scmEventInputTypeIds,
                                                                       scmEIWith,
                                                                       scmEIWithIndexes,
                                                                       3,
                                                                       scmDataOutputNames,
                                                                       scmDataOutputTypeIds,
                                                                       1,
                                                                       scmDataInputNames,
                                                                       scmDataInputTypeIds,
                                                                       0,
                                                                       nullptr,
                                                                       0,
                                                                       nullptr};

void FORTE_WagoBusAdapter::readInputData(const TEventID paEIID) {
  if (isSocket()) {
    switch (paEIID) {
      case scmEventINITOID: {
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default: break;
    }
  } else {
    switch (paEIID) {
      case scmEventINITID: {
        readData(2, *mDIs[2], mDIConns[2]);
        readData(1, *mDIs[1], mDIConns[1]);
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default: break;
    }
  }
}

void FORTE_WagoBusAdapter::writeOutputData(const TEventID paEIID) {
  if (isSocket()) {
    switch (paEIID) {
      case scmEventINITID: {
        writeData(scmFBInterfaceSpecSocket.mNumDIs + 2, *mDOs[2], mDOConns[2]);
        writeData(scmFBInterfaceSpecSocket.mNumDIs + 1, *mDOs[1], mDOConns[1]);
        writeData(scmFBInterfaceSpecSocket.mNumDIs + 0, *mDOs[0], mDOConns[0]);
        break;
      }
      default: break;
    }
  } else {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(scmFBInterfaceSpecPlug.mNumDIs + 0, *mDOs[0], mDOConns[0]);
        break;
      }
      default: break;
    }
  }
}

const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIO[] = {};
const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIONum = 0;
