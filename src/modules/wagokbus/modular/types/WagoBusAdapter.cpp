/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: WagoBusAdapter
 *** Description: 
 *** Version: 
 *************************************************************************/

#include "WagoBusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WagoBusAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_WagoBusAdapter, g_nStringIdWagoBusAdapter)

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataInputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataInputTypeIds[] = {g_nStringIdBOOL};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataOutputNames[] = {g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT};
const TDataIOID FORTE_WagoBusAdapter::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoBusAdapter::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventInputNames[] = {g_nStringIdINITO};
const TDataIOID FORTE_WagoBusAdapter::scmEOWith[] = {2, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoBusAdapter::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scmEventOutputNames[] = {g_nStringIdINIT};

const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIO[] = { };
const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIONum = 0;

const SFBInterfaceSpec FORTE_WagoBusAdapter::scmFBInterfaceSpecSocket = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  3, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

const SFBInterfaceSpec FORTE_WagoBusAdapter::scmFBInterfaceSpecPlug = {
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  3, scmDataOutputNames, scmDataOutputTypeIds,
  1, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr,
  0, nullptr
};

void FORTE_WagoBusAdapter::readInputData(const TEventID paEIID) {
  if(isSocket()) {
    switch(paEIID) {
      case scmEventINITOID: {
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default:
        break;
    }
  } else {
    switch(paEIID) {
      case scmEventINITID: {
        readData(2, *mDIs[2], mDIConns[2]);
        readData(1, *mDIs[1], mDIConns[1]);
        readData(0, *mDIs[0], mDIConns[0]);
        break;
      }
      default:
        break;
    }
  }
}

void FORTE_WagoBusAdapter::writeOutputData(const TEventID paEIID) {
  if(isSocket()) {
    switch(paEIID) {
      case scmEventINITID: {
        writeData(2, *mDOs[2], mDOConns[2]);
        writeData(1, *mDOs[1], mDOConns[1]);
        writeData(0, *mDOs[0], mDOConns[0]);
        break;
      }
      default:
        break;
    }
  } else {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(0, *mDOs[0], mDOConns[0]);
        break;
      }
      default:
        break;
    }
  }
}
