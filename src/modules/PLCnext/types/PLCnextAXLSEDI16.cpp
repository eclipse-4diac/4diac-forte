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
 *** Name: PLCnextAXLSEDI16
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextAXLSEDI16.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(DI_1);
USE_STRING_ID(DI_10);
USE_STRING_ID(DI_11);
USE_STRING_ID(DI_12);
USE_STRING_ID(DI_13);
USE_STRING_ID(DI_14);
USE_STRING_ID(DI_15);
USE_STRING_ID(DI_16);
USE_STRING_ID(DI_2);
USE_STRING_ID(DI_3);
USE_STRING_ID(DI_4);
USE_STRING_ID(DI_5);
USE_STRING_ID(DI_6);
USE_STRING_ID(DI_7);
USE_STRING_ID(DI_8);
USE_STRING_ID(DI_9);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PLCnextAXLSEDI16);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);


#include "PLCnextBusAdapter.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_PLCnextAXLSEDI16, STRID(PLCnextAXLSEDI16))

const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmDataInputNames[] = {STRID(QI), STRID(DI_1), STRID(DI_2), STRID(DI_3), STRID(DI_4), STRID(DI_5), STRID(DI_6), STRID(DI_7), STRID(DI_8), STRID(DI_9), STRID(DI_10), STRID(DI_11), STRID(DI_12), STRID(DI_13), STRID(DI_14), STRID(DI_15), STRID(DI_16)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_PLCnextAXLSEDI16::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextAXLSEDI16::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_PLCnextAXLSEDI16::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextAXLSEDI16::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmEventOutputNames[] = {STRID(INITO), STRID(IND)};
const CStringDictionary::TStringId FORTE_PLCnextAXLSEDI16::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
const SAdapterInstanceDef FORTE_PLCnextAXLSEDI16::scmAdapterInstances[] = {
  {STRID(PLCnextBusAdapter), STRID(BusAdapterIn), false},
  {STRID(PLCnextBusAdapter), STRID(BusAdapterOut), true}
};
const SFBInterfaceSpec FORTE_PLCnextAXLSEDI16::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  17, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_PLCnextAXLSEDI16::FORTE_PLCnextAXLSEDI16(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    PLCnextSlaveHandler (PLCnextSlaveHandler::Input, pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData),
    var_QI(0_BOOL),
    var_DI_1(""_STRING),
    var_DI_2(""_STRING),
    var_DI_3(""_STRING),
    var_DI_4(""_STRING),
    var_DI_5(""_STRING),
    var_DI_6(""_STRING),
    var_DI_7(""_STRING),
    var_DI_8(""_STRING),
    var_DI_9(""_STRING),
    var_DI_10(""_STRING),
    var_DI_11(""_STRING),
    var_DI_12(""_STRING),
    var_DI_13(""_STRING),
    var_DI_14(""_STRING),
    var_DI_15(""_STRING),
    var_DI_16(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_BusAdapterIn(STRID(BusAdapterIn), *this, false),
    var_BusAdapterOut(STRID(BusAdapterOut), *this, true),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_DI_1(nullptr),
    conn_DI_2(nullptr),
    conn_DI_3(nullptr),
    conn_DI_4(nullptr),
    conn_DI_5(nullptr),
    conn_DI_6(nullptr),
    conn_DI_7(nullptr),
    conn_DI_8(nullptr),
    conn_DI_9(nullptr),
    conn_DI_10(nullptr),
    conn_DI_11(nullptr),
    conn_DI_12(nullptr),
    conn_DI_13(nullptr),
    conn_DI_14(nullptr),
    conn_DI_15(nullptr),
    conn_DI_16(nullptr),
    conn_QO(this, 0, var_QO),
    conn_STATUS(this, 1, var_STATUS) {
};

bool FORTE_PLCnextAXLSEDI16::initialize() {
  if(!var_BusAdapterIn.initialize()) { return false; }
  if(!var_BusAdapterOut.initialize()) { return false; }
  var_BusAdapterIn.setParentFB(this, 0);
  var_BusAdapterOut.setParentFB(this, 1);
  return CFunctionBlock::initialize();
}

void FORTE_PLCnextAXLSEDI16::setInitialValues() {
  var_QI = 0_BOOL;
  var_DI_1 = ""_STRING;
  var_DI_2 = ""_STRING;
  var_DI_3 = ""_STRING;
  var_DI_4 = ""_STRING;
  var_DI_5 = ""_STRING;
  var_DI_6 = ""_STRING;
  var_DI_7 = ""_STRING;
  var_DI_8 = ""_STRING;
  var_DI_9 = ""_STRING;
  var_DI_10 = ""_STRING;
  var_DI_11 = ""_STRING;
  var_DI_12 = ""_STRING;
  var_DI_13 = ""_STRING;
  var_DI_14 = ""_STRING;
  var_DI_15 = ""_STRING;
  var_DI_16 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_PLCnextAXLSEDI16::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_DI_1, conn_DI_1);
      readData(2, var_DI_2, conn_DI_2);
      readData(3, var_DI_3, conn_DI_3);
      readData(4, var_DI_4, conn_DI_4);
      readData(5, var_DI_5, conn_DI_5);
      readData(6, var_DI_6, conn_DI_6);
      readData(7, var_DI_7, conn_DI_7);
      readData(8, var_DI_8, conn_DI_8);
      readData(9, var_DI_9, conn_DI_9);
      readData(10, var_DI_10, conn_DI_10);
      readData(11, var_DI_11, conn_DI_11);
      readData(12, var_DI_12, conn_DI_12);
      readData(13, var_DI_13, conn_DI_13);
      readData(14, var_DI_14, conn_DI_14);
      readData(15, var_DI_15, conn_DI_15);
      readData(16, var_DI_16, conn_DI_16);
      break;
    }
    default:
      break;
  }
}

void FORTE_PLCnextAXLSEDI16::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
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

CIEC_ANY *FORTE_PLCnextAXLSEDI16::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DI_1;
    case 2: return &var_DI_2;
    case 3: return &var_DI_3;
    case 4: return &var_DI_4;
    case 5: return &var_DI_5;
    case 6: return &var_DI_6;
    case 7: return &var_DI_7;
    case 8: return &var_DI_8;
    case 9: return &var_DI_9;
    case 10: return &var_DI_10;
    case 11: return &var_DI_11;
    case 12: return &var_DI_12;
    case 13: return &var_DI_13;
    case 14: return &var_DI_14;
    case 15: return &var_DI_15;
    case 16: return &var_DI_16;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextAXLSEDI16::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CAdapter *FORTE_PLCnextAXLSEDI16::getAdapterUnchecked(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_BusAdapterIn;
    case 1: return &var_BusAdapterOut;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextAXLSEDI16::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextAXLSEDI16::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DI_1;
    case 2: return &conn_DI_2;
    case 3: return &conn_DI_3;
    case 4: return &conn_DI_4;
    case 5: return &conn_DI_5;
    case 6: return &conn_DI_6;
    case 7: return &conn_DI_7;
    case 8: return &conn_DI_8;
    case 9: return &conn_DI_9;
    case 10: return &conn_DI_10;
    case 11: return &conn_DI_11;
    case 12: return &conn_DI_12;
    case 13: return &conn_DI_13;
    case 14: return &conn_DI_14;
    case 15: return &conn_DI_15;
    case 16: return &conn_DI_16;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextAXLSEDI16::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_PLCnextAXLSEDI16::initHandles() {
    // Initialize handles
    for (int i = 0; i < 16; i++) {
        PLCnextDeviceController::HandleDescriptor desc = PLCnextDeviceController::HandleDescriptor(
            *static_cast<CIEC_WSTRING*>(getDI(i+1)), 
            forte::core::io::IOMapper::In, 
            mIndex,
            (uint16_t) i,
            PLCnextDeviceController::Bit);

        initHandle(desc);
    }
}

const char* FORTE_PLCnextAXLSEDI16::init() {
    PLCnextDeviceController &controller = *static_cast<PLCnextDeviceController*>(&getController());

    // initialize buffer with correct size
    initBufferImage(16); 
    controller.registerSlaveHandler(this);
    
    return plcNextDevice.init(mIndex, "IN01");
}
