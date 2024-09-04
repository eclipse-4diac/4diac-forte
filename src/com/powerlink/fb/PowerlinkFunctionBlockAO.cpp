/*******************************************************************************
* Copyright (c) 2024 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Gafert - abstracted functions for better reusability
 *******************************************************************************/

#include "PowerlinkFunctionBlockAO.h"


void PowerlinkFunctionBlockAO::cnSynchCallback() {
    CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();
    sync.lock();

    EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
    EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
    for (it; it != itEnd; ++it) {
        short ioVal = *((short *) (it->mCurrentValue));
        char highByte = (char) ((ioVal & 0xFF00) >> 8);
        char lowByte = (char) (ioVal & 0x00FF);
        (eplStack.getProcImageIn())[it->mPiOffset] &= (~(0xFF << it->mBitOffset));
        (eplStack.getProcImageIn())[it->mPiOffset] |= (lowByte << (it->mBitOffset));

        (eplStack.getProcImageIn())[it->mPiOffset + 1] &= (~(0xFF << it->mBitOffset));
        (eplStack.getProcImageIn())[it->mPiOffset + 1] |= (highByte << (it->mBitOffset));
    }

    sync.unlock();
}

void PowerlinkFunctionBlockAO::executePowerlinkEvent(const TEventID paEIID,
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

            CProcessImageMatrix *moduleIOs = eplStack.getProcessImageMatrixIn()->getModuleEntries(
                var_CNID.getUnsignedValue(), var_MODID.getUnsignedValue());

            if (moduleIOs) {
                // Outputs (process inputs) always start with i = 0
                // Check xap.xml if a BitUnused is present
                for (unsigned int i = 0; i < moduleIOs->getNrOfEntries() - 1; i++) {
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
            for (int i = 3; i < getFBInterfaceSpec().mNumDIs && it != itEnd; i++, ++it) {
                short ioVal = static_cast<CIEC_INT *>(getDI(i))->getSignedValue();
                *((short*) (it->mCurrentValue)) = ioVal;
            }
            sync.unlock();
        }
        var_QO = var_QI;
        sendOutputEvent(scmEventCNFID, paECET);
    }
}
