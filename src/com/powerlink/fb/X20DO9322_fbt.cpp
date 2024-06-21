/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: X20DO9322
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2011-05-31/4DIAC-IDE - 4DIAC-Consortium -
 ***     1.0: 2011-05-31/FA,TS - AIT -
 *************************************************************************/

#include "X20DO9322_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20DO9322_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_X20DO9322, g_nStringIdX20DO9322)

const CStringDictionary::TStringId FORTE_X20DO9322::scmDataInputNames[] = {
    g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID, g_nStringIdDO01, g_nStringIdDO02, g_nStringIdDO03,
    g_nStringIdDO04, g_nStringIdDO05, g_nStringIdDO06, g_nStringIdDO07, g_nStringIdDO08, g_nStringIdDO09,
    g_nStringIdDO10, g_nStringIdDO11, g_nStringIdDO12
};
const CStringDictionary::TStringId FORTE_X20DO9322::scmDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL,
    g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL,
    g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL
};
const CStringDictionary::TStringId FORTE_X20DO9322::scmDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS
};
const CStringDictionary::TStringId FORTE_X20DO9322::scmDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING
};
const TDataIOID FORTE_X20DO9322::scmEIWith[] = {
    0, 1, 2, scmWithListDelimiter, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, scmWithListDelimiter
};
const TForteInt16 FORTE_X20DO9322::scmEIWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DO9322::scmEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};
const TDataIOID FORTE_X20DO9322::scmEOWith[] = {0, 2, 1, scmWithListDelimiter, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DO9322::scmEOWithIndexes[] = {0, 4};
const CStringDictionary::TStringId FORTE_X20DO9322::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};
const SFBInterfaceSpec FORTE_X20DO9322::scmFBInterfaceSpec = {
    2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
    2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
    15, scmDataInputNames, scmDataInputTypeIds,
    3, scmDataOutputNames, scmDataOutputTypeIds,
    0, nullptr,
    0, nullptr
};

FORTE_X20DO9322::FORTE_X20DO9322(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) : CFunctionBlock(paContainer,
                                                                               &scmFBInterfaceSpec, paInstanceNameId),
                                                                           var_conn_QO(var_QO),
                                                                           var_conn_CNIDO(var_CNIDO),
                                                                           var_conn_STATUS(var_STATUS),
                                                                           conn_INITO(this, 0),
                                                                           conn_CNF(this, 1),
                                                                           conn_QI(nullptr),
                                                                           conn_CNID(nullptr),
                                                                           conn_MODID(nullptr),
                                                                           conn_DO01(nullptr),
                                                                           conn_DO02(nullptr),
                                                                           conn_DO03(nullptr),
                                                                           conn_DO04(nullptr),
                                                                           conn_DO05(nullptr),
                                                                           conn_DO06(nullptr),
                                                                           conn_DO07(nullptr),
                                                                           conn_DO08(nullptr),
                                                                           conn_DO09(nullptr),
                                                                           conn_DO10(nullptr),
                                                                           conn_DO11(nullptr),
                                                                           conn_DO12(nullptr),
                                                                           conn_QO(this, 0, &var_conn_QO),
                                                                           conn_CNIDO(this, 1, &var_conn_CNIDO),
                                                                           conn_STATUS(this, 2, &var_conn_STATUS) {
};

void FORTE_X20DO9322::setInitialValues() {
    var_QI = 0_BOOL;
    var_CNID = 0_USINT;
    var_MODID = 0_UINT;
    var_DO01 = 0_BOOL;
    var_DO02 = 0_BOOL;
    var_DO03 = 0_BOOL;
    var_DO04 = 0_BOOL;
    var_DO05 = 0_BOOL;
    var_DO06 = 0_BOOL;
    var_DO07 = 0_BOOL;
    var_DO08 = 0_BOOL;
    var_DO09 = 0_BOOL;
    var_DO10 = 0_BOOL;
    var_DO11 = 0_BOOL;
    var_DO12 = 0_BOOL;
    var_QO = 0_BOOL;
    var_CNIDO = 0_USINT;
    var_STATUS = ""_STRING;
}

void FORTE_X20DO9322::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
        case scmEventINITID:
            if (var_QI == true) {
                initOk = false;

                CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

                // Get settings for intputs
                CProcessImageMatrix *moduleIOs = eplStack.getProcessImageMatrixIn()->getModuleEntries(
                    var_CNID.getUnsignedValue(), var_MODID.getUnsignedValue());

                if (moduleIOs) {
                    // Outputs (process inputs) always start with i = 0
                    // Check xap.xml if a BitUnused is present
                    for(unsigned int i = 0; i < moduleIOs->getNrOfEntries() - 1; i++){
                        eplMapping.mCurrentValues.pushBack(new EplMapping::EplMappingValues(moduleIOs->getEntry(i)[0], moduleIOs->getEntry(i)[1], moduleIOs->getEntry(i)[2]));
                    }
                    delete moduleIOs;
                    eplStack.registerCallback(this);

                    initOk = true;
                } else {
                    DEVLOG_ERROR("[powerlink] No module IOs");
                }
            }

            sendOutputEvent(scmEventINITOID, paECET);
            break;
        case scmEventREQID:
            if (var_QI == true && initOk) {
                sync.lock();
                EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
                EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
                for(int i = 3; i < mInterfaceSpec->mNumDIs && it != itEnd; i++, ++it){
                    bool ioVal = *static_cast<CIEC_BOOL*>(getDI(i));
                    *(it->mCurrentValue) = (char) ioVal;
                }

                sync.unlock();
            }
            var_QO = var_QI;
            sendOutputEvent(scmEventCNFID, paECET);
            break;
    }
}

void FORTE_X20DO9322::readInputData(const TEventID paEIID) {
    switch (paEIID) {
        case scmEventINITID: {
            readData(0, var_QI, conn_QI);
            readData(1, var_CNID, conn_CNID);
            readData(2, var_MODID, conn_MODID);
            break;
        }
        case scmEventREQID: {
            readData(3, var_DO01, conn_DO01);
            readData(4, var_DO02, conn_DO02);
            readData(5, var_DO03, conn_DO03);
            readData(6, var_DO04, conn_DO04);
            readData(7, var_DO05, conn_DO05);
            readData(8, var_DO06, conn_DO06);
            readData(9, var_DO07, conn_DO07);
            readData(10, var_DO08, conn_DO08);
            readData(11, var_DO09, conn_DO09);
            readData(12, var_DO10, conn_DO10);
            readData(13, var_DO11, conn_DO11);
            readData(14, var_DO12, conn_DO12);
            readData(0, var_QI, conn_QI);
            break;
        }
        default:
            break;
    }
}

void FORTE_X20DO9322::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
        case scmEventINITOID: {
            writeData(0, var_QO, conn_QO);
            writeData(2, var_STATUS, conn_STATUS);
            writeData(1, var_CNIDO, conn_CNIDO);
            break;
        }
        case scmEventCNFID: {
            writeData(2, var_STATUS, conn_STATUS);
            writeData(0, var_QO, conn_QO);
            break;
        }
        default:
            break;
    }
}

CIEC_ANY *FORTE_X20DO9322::getDI(const size_t paIndex) {
    switch (paIndex) {
        case 0: return &var_QI;
        case 1: return &var_CNID;
        case 2: return &var_MODID;
        case 3: return &var_DO01;
        case 4: return &var_DO02;
        case 5: return &var_DO03;
        case 6: return &var_DO04;
        case 7: return &var_DO05;
        case 8: return &var_DO06;
        case 9: return &var_DO07;
        case 10: return &var_DO08;
        case 11: return &var_DO09;
        case 12: return &var_DO10;
        case 13: return &var_DO11;
        case 14: return &var_DO12;
    }
    return nullptr;
}

CIEC_ANY *FORTE_X20DO9322::getDO(const size_t paIndex) {
    switch (paIndex) {
        case 0: return &var_QO;
        case 1: return &var_CNIDO;
        case 2: return &var_STATUS;
    }
    return nullptr;
}

CEventConnection *FORTE_X20DO9322::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_INITO;
        case 1: return &conn_CNF;
    }
    return nullptr;
}

CDataConnection **FORTE_X20DO9322::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_QI;
        case 1: return &conn_CNID;
        case 2: return &conn_MODID;
        case 3: return &conn_DO01;
        case 4: return &conn_DO02;
        case 5: return &conn_DO03;
        case 6: return &conn_DO04;
        case 7: return &conn_DO05;
        case 8: return &conn_DO06;
        case 9: return &conn_DO07;
        case 10: return &conn_DO08;
        case 11: return &conn_DO09;
        case 12: return &conn_DO10;
        case 13: return &conn_DO11;
        case 14: return &conn_DO12;
    }
    return nullptr;
}

CDataConnection *FORTE_X20DO9322::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
        case 0: return &conn_QO;
        case 1: return &conn_CNIDO;
        case 2: return &conn_STATUS;
    }
    return nullptr;
}

void FORTE_X20DO9322::cnSynchCallback() {
    CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();
    sync.lock();

    EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
    EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
    for(it; it != itEnd; ++it){
        bool ioVal = *(it->mCurrentValue) != 0x00;
        (eplStack.getProcImageIn())[it->mPiOffset] &= (char) (~(0x01 << it->mBitOffset));
        (eplStack.getProcImageIn())[it->mPiOffset] |= (char) (ioVal << it->mBitOffset);
    }

    sync.unlock();
}
