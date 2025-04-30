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

USE_STRING_ID(BOOL);
USE_STRING_ID(EBBusAdapter);
USE_STRING_ID(EInit);
USE_STRING_ID(Index);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MasterId);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);


DEFINE_ADAPTER_TYPE(FORTE_EBBusAdapter, STRID(EBBusAdapter))

const CStringDictionary::TStringId FORTE_EBBusAdapter::scmDataInputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmDataOutputNames[] = {STRID(QI), STRID(MasterId), STRID(Index),
                                                                               STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UINT), STRID(UINT),
                                                                                 STRID(UINT)};
const TDataIOID FORTE_EBBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_EBBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmEventInputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_EBBusAdapter::scmEOWith[] = {2, 3, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_EBBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmEventOutputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_EBBusAdapter::scmEventOutputTypeIds[] = {STRID(EInit)};

const SFBInterfaceSpec FORTE_EBBusAdapter::scmFBInterfaceSpecSocket = {1,
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
                                                                       4,
                                                                       scmDataOutputNames,
                                                                       scmDataOutputTypeIds,
                                                                       0,
                                                                       nullptr,
                                                                       0,
                                                                       nullptr};

const SFBInterfaceSpec FORTE_EBBusAdapter::scmFBInterfaceSpecPlug = {1,
                                                                     scmEventOutputNames,
                                                                     scmEventOutputTypeIds,
                                                                     scmEOWith,
                                                                     scmEOWithIndexes,
                                                                     1,
                                                                     scmEventInputNames,
                                                                     scmEventInputTypeIds,
                                                                     scmEIWith,
                                                                     scmEIWithIndexes,
                                                                     4,
                                                                     scmDataOutputNames,
                                                                     scmDataOutputTypeIds,
                                                                     1,
                                                                     scmDataInputNames,
                                                                     scmDataInputTypeIds,
                                                                     0,
                                                                     nullptr,
                                                                     0,
                                                                     nullptr};

void FORTE_EBBusAdapter::readInputData(const TEventID paEIID) {
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
        readData(3, *mDIs[3], mDIConns[3]);
        readData(1, *mDIs[1], mDIConns[1]);
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default: break;
    }
  }
}

void FORTE_EBBusAdapter::writeOutputData(const TEventID paEIID) {
  if (isSocket()) {
    switch (paEIID) {
      case scmEventINITID: {
        writeData(scmFBInterfaceSpecSocket.mNumDIs + 2, *mDOs[2], mDOConns[2]);
        writeData(scmFBInterfaceSpecSocket.mNumDIs + 3, *mDOs[3], mDOConns[3]);
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

const TForteUInt8 FORTE_EBBusAdapter::scmSlaveConfigurationIO[] = {3};
const TForteUInt8 FORTE_EBBusAdapter::scmSlaveConfigurationIONum = 1;
