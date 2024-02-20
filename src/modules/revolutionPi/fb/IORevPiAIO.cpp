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

const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdAnalogInput_1, g_nStringIdAnalogInput_2, g_nStringIdAnalogInput_3, g_nStringIdAnalogInput_4, g_nStringIdRTD_1, g_nStringIdRTD_2, g_nStringIdAnalogOutput_1, g_nStringIdAnalogOutput_2};

const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_IORevPiAIO::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_IORevPiAIO::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_IORevPiAIO::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_IORevPiAIO::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scm_anEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_IORevPiAIO::scm_astAdapterInstances[] = {
{g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_IORevPiAIO::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  9,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};

const TForteUInt8 FORTE_IORevPiAIO::scm_slaveConfigurationIO[] = { };
const TForteUInt8 FORTE_IORevPiAIO::scm_slaveConfigurationIO_num = 0;


void FORTE_IORevPiAIO::initHandles() {
  uint8_t inputOffset = 0;
  uint8_t outputOffset = 0;

  for (int i = 1; i < 9; i++) {
    uint8_t* currentOffset = (i < 7) ? &inputOffset : &outputOffset;
      RevPiController::HandleDescriptor desc = RevPiController::HandleDescriptor(
          *static_cast<CIEC_WSTRING*>(getDI(i)), (i < 7) ? forte::core::IO::IOMapper::In : forte::core::IO::IOMapper::Out, index,
          CIEC_ANY::e_WORD, *currentOffset, 0);
      initHandle(&desc);
    *currentOffset = static_cast<uint8_t>(*currentOffset + 2);
  }
}



