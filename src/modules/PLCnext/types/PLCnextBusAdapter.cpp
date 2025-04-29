/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextBusAdapter
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextBusAdapter.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(Index);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(MasterId);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_ADAPTER_TYPE(FORTE_PLCnextBusAdapter, STRID(PLCnextBusAdapter))

const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataInputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataOutputNames[] = {
    STRID(QI), STRID(MasterId), STRID(Index), STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UINT),
                                                                                      STRID(UINT), STRID(UINT)};
const TDataIOID FORTE_PLCnextBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventInputNames[] = {STRID(INITO)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_PLCnextBusAdapter::scmEOWith[] = {2, 3, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventOutputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_PLCnextBusAdapter::scmEventOutputTypeIds[] = {STRID(EInit)};

const SFBInterfaceSpec FORTE_PLCnextBusAdapter::scmFBInterfaceSpecSocket = {1,
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

const SFBInterfaceSpec FORTE_PLCnextBusAdapter::scmFBInterfaceSpecPlug = {1,
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

void FORTE_PLCnextBusAdapter::readInputData(const TEventID paEIID) {
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

void FORTE_PLCnextBusAdapter::writeOutputData(const TEventID paEIID) {
  if (isSocket()) {
    switch (paEIID) {
      case scmEventINITID: {
        writeData(2, *mDOs[2], mDOConns[2]);
        writeData(3, *mDOs[3], mDOConns[3]);
        writeData(1, *mDOs[1], mDOConns[1]);
        writeData(0, *mDOs[0], mDOConns[0]);
        break;
      }
      default: break;
    }
  } else {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(0, *mDOs[0], mDOConns[0]);
        break;
      }
      default: break;
    }
  }
}
