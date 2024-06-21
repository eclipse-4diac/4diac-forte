/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: X20DI9371
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2011-05-31/4DIAC-IDE - 4DIAC-Consortium -
 ***     1.0: 2011-05-31/FA,TS - AIT -
 *************************************************************************/

#include "X20DI9371_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20DI9371_fbt_gen.cpp"
#endif

#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_X20DI9371, g_nStringIdX20DI9371)

const CStringDictionary::TStringId FORTE_X20DI9371::scmDataInputNames[] = {
    g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID
};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT
};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdDI01, g_nStringIdDI02, g_nStringIdDI03,
    g_nStringIdDI04, g_nStringIdDI05, g_nStringIdDI06, g_nStringIdDI07, g_nStringIdDI08, g_nStringIdDI09,
    g_nStringIdDI10, g_nStringIdDI11, g_nStringIdDI12
};
const CStringDictionary::TStringId FORTE_X20DI9371::scmDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL,
    g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL,
    g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL
};
const TDataIOID FORTE_X20DI9371::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI9371::scmEIWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};
const TDataIOID FORTE_X20DI9371::scmEOWith[] = {
    0, 2, 1, scmWithListDelimiter, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 0, scmWithListDelimiter
};
const TForteInt16 FORTE_X20DI9371::scmEOWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DI9371::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};
const SFBInterfaceSpec FORTE_X20DI9371::scmFBInterfaceSpec = {
    2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
    2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
    3, scmDataInputNames, scmDataInputTypeIds,
    15, scmDataOutputNames, scmDataOutputTypeIds,
    0, nullptr,
    0, nullptr
};

FORTE_X20DI9371::FORTE_X20DI9371(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) : PowerlinkFunctionBlockDI(paContainer,
                                                                               &scmFBInterfaceSpec, paInstanceNameId),
                                                                           var_conn_QO(var_QO),
                                                                           var_conn_CNIDO(var_CNIDO),
                                                                           var_conn_STATUS(var_STATUS),
                                                                           var_conn_DI01(var_DI01),
                                                                           var_conn_DI02(var_DI02),
                                                                           var_conn_DI03(var_DI03),
                                                                           var_conn_DI04(var_DI04),
                                                                           var_conn_DI05(var_DI05),
                                                                           var_conn_DI06(var_DI06),
                                                                           var_conn_DI07(var_DI07),
                                                                           var_conn_DI08(var_DI08),
                                                                           var_conn_DI09(var_DI09),
                                                                           var_conn_DI10(var_DI10),
                                                                           var_conn_DI11(var_DI11),
                                                                           var_conn_DI12(var_DI12),
                                                                           conn_INITO(this, 0),
                                                                           conn_CNF(this, 1),
                                                                           conn_QI(nullptr),
                                                                           conn_CNID(nullptr),
                                                                           conn_MODID(nullptr),
                                                                           conn_QO(this, 0, &var_conn_QO),
                                                                           conn_CNIDO(this, 1, &var_conn_CNIDO),
                                                                           conn_STATUS(this, 2, &var_conn_STATUS),
                                                                           conn_DI01(this, 3, &var_conn_DI01),
                                                                           conn_DI02(this, 4, &var_conn_DI02),
                                                                           conn_DI03(this, 5, &var_conn_DI03),
                                                                           conn_DI04(this, 6, &var_conn_DI04),
                                                                           conn_DI05(this, 7, &var_conn_DI05),
                                                                           conn_DI06(this, 8, &var_conn_DI06),
                                                                           conn_DI07(this, 9, &var_conn_DI07),
                                                                           conn_DI08(this, 10, &var_conn_DI08),
                                                                           conn_DI09(this, 11, &var_conn_DI09),
                                                                           conn_DI10(this, 12, &var_conn_DI10),
                                                                           conn_DI11(this, 13, &var_conn_DI11),
                                                                           conn_DI12(this, 14, &var_conn_DI12) {
};

void FORTE_X20DI9371::setInitialValues() {
    var_QI = 0_BOOL;
    var_CNID = 0_USINT;
    var_MODID = 0_UINT;
    var_QO = 0_BOOL;
    var_CNIDO = 0_USINT;
    var_STATUS = ""_STRING;
    var_DI01 = 0_BOOL;
    var_DI02 = 0_BOOL;
    var_DI03 = 0_BOOL;
    var_DI04 = 0_BOOL;
    var_DI05 = 0_BOOL;
    var_DI06 = 0_BOOL;
    var_DI07 = 0_BOOL;
    var_DI08 = 0_BOOL;
    var_DI09 = 0_BOOL;
    var_DI10 = 0_BOOL;
    var_DI11 = 0_BOOL;
    var_DI12 = 0_BOOL;
}

void FORTE_X20DI9371::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    executePowerlinkEvent(paEIID,
                          paECET,
                          scmEventINITID,
                          scmEventREQID,
                          scmEventINITOID,
                          scmEventCNFID,
                          var_QI,
                          var_QO,
                          var_CNID,
                          var_MODID);
}

void FORTE_X20DI9371::readInputData(const TEventID paEIID) {
    switch (paEIID) {
        case scmEventINITID: {
            readData(0, var_QI, conn_QI);
            readData(1, var_CNID, conn_CNID);
            readData(2, var_MODID, conn_MODID);
            break;
        }
        case scmEventREQID: {
            readData(0, var_QI, conn_QI);
            break;
        }
        default:
            break;
    }
}

void FORTE_X20DI9371::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
        case scmEventINITOID: {
            writeData(0, var_QO, conn_QO);
            writeData(2, var_STATUS, conn_STATUS);
            writeData(1, var_CNIDO, conn_CNIDO);
            break;
        }
        case scmEventCNFID: {
            writeData(3, var_DI01, conn_DI01);
            writeData(4, var_DI02, conn_DI02);
            writeData(5, var_DI03, conn_DI03);
            writeData(6, var_DI04, conn_DI04);
            writeData(7, var_DI05, conn_DI05);
            writeData(8, var_DI06, conn_DI06);
            writeData(9, var_DI07, conn_DI07);
            writeData(10, var_DI08, conn_DI08);
            writeData(11, var_DI09, conn_DI09);
            writeData(12, var_DI10, conn_DI10);
            writeData(13, var_DI11, conn_DI11);
            writeData(14, var_DI12, conn_DI12);
            writeData(2, var_STATUS, conn_STATUS);
            writeData(0, var_QO, conn_QO);
            break;
        }
        default:
            break;
    }
}

CIEC_ANY *FORTE_X20DI9371::getDI(const size_t paIndex) {
    switch (paIndex) {
        case 0: return &var_QI;
        case 1: return &var_CNID;
        case 2: return &var_MODID;
    }
    return nullptr;
}

CIEC_ANY *FORTE_X20DI9371::getDO(const size_t paIndex) {
    switch (paIndex) {
        case 0: return &var_QO;
        case 1: return &var_CNIDO;
        case 2: return &var_STATUS;
        case 3: return &var_DI01;
        case 4: return &var_DI02;
        case 5: return &var_DI03;
        case 6: return &var_DI04;
        case 7: return &var_DI05;
        case 8: return &var_DI06;
        case 9: return &var_DI07;
        case 10: return &var_DI08;
        case 11: return &var_DI09;
        case 12: return &var_DI10;
        case 13: return &var_DI11;
        case 14: return &var_DI12;
    }
    return nullptr;
}

CEventConnection *FORTE_X20DI9371::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_INITO;
        case 1: return &conn_CNF;
    }
    return nullptr;
}

CDataConnection **FORTE_X20DI9371::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_QI;
        case 1: return &conn_CNID;
        case 2: return &conn_MODID;
    }
    return nullptr;
}

CDataConnection *FORTE_X20DI9371::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_QO;
        case 1: return &conn_CNIDO;
        case 2: return &conn_STATUS;
        case 3: return &conn_DI01;
        case 4: return &conn_DI02;
        case 5: return &conn_DI03;
        case 6: return &conn_DI04;
        case 7: return &conn_DI05;
        case 8: return &conn_DI06;
        case 9: return &conn_DI07;
        case 10: return &conn_DI08;
        case 11: return &conn_DI09;
        case 12: return &conn_DI10;
        case 13: return &conn_DI11;
        case 14: return &conn_DI12;
    }
    return nullptr;
}
