//
// Created by GafertM on 21.06.2024.
//

#include "PowerlinkFunctionBlockAT.h"


void PowerlinkFunctionBlockAT::cnSynchCallback() {
    CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();
    sync.lock();

    EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
    EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
    for (; it != itEnd; ++it) {
        short ioVal = 0x0000;
        char lowByte;
        char highByte;
        lowByte = (eplStack.getProcImageOut()[it->mPiOffset] & (0xFF << it->mBitOffset)) >> it->mBitOffset;
        highByte = (eplStack.getProcImageOut()[it->mPiOffset + 1] & (0xFF << it->mBitOffset)) >> it->mBitOffset;
        ioVal = (short) ((0xFF00 & (highByte << 8))) | (short) (0xFF & lowByte);

        *((short *) (it->mCurrentValue)) = ioVal;
    }

    sync.unlock();
}

void PowerlinkFunctionBlockAT::executePowerlinkEvent(const TEventID paEIID,
                                                     CEventChainExecutionThread *const paECET,
                                                     TEventID scmEventINITID,
                                                     TEventID scmEventREQID,
                                                     TEventID scmEventINITOID,
                                                     TEventID scmEventCNFID,
                                                     const CIEC_BOOL &var_QI,
                                                     CIEC_BOOL &var_QO,
                                                     const CIEC_USINT &var_CNID,
                                                     const CIEC_UINT &var_MODID) {
    if (paEIID == scmEventINITID) {
        if (var_QI == true) {
            initOk = false;

            CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

            CProcessImageMatrix *moduleIOs = eplStack.getProcessImageMatrixOut()->getModuleEntries(
                var_CNID.getUnsignedValue(), var_MODID.getUnsignedValue());

            if (moduleIOs) {
                // Inputs (process inputs) always start with i = 1
                // Check xap.xml if a BitUnused is present
                for (unsigned int i = 1; i < moduleIOs->getNrOfEntries() - 1; i++) {
                    eplMapping.mCurrentValues.pushBack(new EplMapping::EplMappingValues(
                        moduleIOs->getEntry(i)[0], moduleIOs->getEntry(i)[1], moduleIOs->getEntry(i)[2]));
                }

                delete moduleIOs;
                eplStack.registerCallback(this);

                initOk = true;
            } else {
                DEVLOG_ERROR("[powerlink] No module IOs");
            }
        }

        sendOutputEvent(scmEventINITOID, paECET);
    } else if (paEIID == scmEventREQID) {
        if (var_QI == true && initOk) {
            sync.lock();
            EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
            EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
            for (int i = 3; i < mInterfaceSpec->mNumDOs && it != itEnd; i++, ++it) {
                short ioVal = *((short *) (it->mCurrentValue));
                *static_cast<CIEC_REAL *>(getDO(i)) = CIEC_REAL(ioVal / 10);
            }
            sync.unlock();
        }
        var_QO = var_QI;
        sendOutputEvent(scmEventCNFID, paECET);
    }
}
