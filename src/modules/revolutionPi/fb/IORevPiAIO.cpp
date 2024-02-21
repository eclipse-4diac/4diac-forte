/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: IORevPiAIO
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2017-10-26/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "IORevPiAIO.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IORevPiAIO_gen.cpp"
#endif

#include "../RevPiController.h"

DEFINE_FIRMWARE_FB(FORTE_IORevPiAIO, g_nStringIdIORevPiAIO)

const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdAnalogInput_1, g_nStringIdAnalogInput_2, g_nStringIdAnalogInput_3, g_nStringIdAnalogInput_4, g_nStringIdRTD_1, g_nStringIdRTD_2, g_nStringIdAnalogOutput_1, g_nStringIdAnalogOutput_2};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};
const TDataIOID FORTE_IORevPiAIO::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiAIO::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventInputNames[] = {g_nStringIdMAP};
const TDataIOID FORTE_IORevPiAIO::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiAIO::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_IORevPiAIO::scmAdapterInstances[] = {
  {g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterIn, false},
  {g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterOut, true}
};
const SFBInterfaceSpec FORTE_IORevPiAIO::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  9, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

const TForteUInt8 FORTE_IORevPiAIO::scm_slaveConfigurationIO[] = { };
const TForteUInt8 FORTE_IORevPiAIO::scm_slaveConfigurationIO_num = 0;


void FORTE_IORevPiAIO::initHandles() {
  uint8_t inputOffset = 0;
  uint8_t outputOffset = 0;

  for (int i = 1; i < 9; i++) {
    uint8_t* currentOffset = (i < 7) ? &inputOffset : &outputOffset;
      RevPiController::HandleDescriptor desc = RevPiController::HandleDescriptor(
          *static_cast<CIEC_WSTRING*>(getDI(i)), (i < 7) ? forte::core::io::IOMapper::In : forte::core::io::IOMapper::Out, mIndex,
          CIEC_ANY::e_WORD, *currentOffset, 0);
      initHandle(&desc);
    *currentOffset = static_cast<uint8_t>(*currentOffset + 2);
  }
}

void FORTE_IORevPiAIO::setInitialValues() {
  var_QI = 0_BOOL;
  var_AnalogInput_1 = u""_WSTRING;
  var_AnalogInput_2 = u""_WSTRING;
  var_AnalogInput_3 = u""_WSTRING;
  var_AnalogInput_4 = u""_WSTRING;
  var_RTD_1 = u""_WSTRING;
  var_RTD_2 = u""_WSTRING;
  var_AnalogOutput_1 = u""_WSTRING;
  var_AnalogOutput_2 = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_IORevPiAIO::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPID: {
      readData(1, var_AnalogInput_1, conn_AnalogInput_1);
      readData(2, var_AnalogInput_2, conn_AnalogInput_2);
      readData(5, var_RTD_1, conn_RTD_1);
      readData(3, var_AnalogInput_3, conn_AnalogInput_3);
      readData(4, var_AnalogInput_4, conn_AnalogInput_4);
      readData(6, var_RTD_2, conn_RTD_2);
      readData(7, var_AnalogOutput_1, conn_AnalogOutput_1);
      readData(8, var_AnalogOutput_2, conn_AnalogOutput_2);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_IORevPiAIO::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_IORevPiAIO::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_AnalogInput_1;
    case 2: return &var_AnalogInput_2;
    case 3: return &var_AnalogInput_3;
    case 4: return &var_AnalogInput_4;
    case 5: return &var_RTD_1;
    case 6: return &var_RTD_2;
    case 7: return &var_AnalogOutput_1;
    case 8: return &var_AnalogOutput_2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IORevPiAIO::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}
