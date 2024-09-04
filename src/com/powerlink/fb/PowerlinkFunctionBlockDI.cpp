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

#include "PowerlinkFunctionBlockDI.h"


void PowerlinkFunctionBlockDI::cnSynchCallback() {
    CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();
    sync.lock();

    EplMapping::TEplMappingList::Iterator itEnd = eplMapping.mCurrentValues.end();
    EplMapping::TEplMappingList::Iterator it = eplMapping.mCurrentValues.begin();
    for (; it != itEnd; ++it) {
        bool ioVal = (eplStack.getProcImageOut()[it->mPiOffset] & (char) (0x01 << it->mBitOffset)) != 0x00;
        *(it->mCurrentValue) = (char) ioVal;
    }

    sync.unlock();
}

void PowerlinkFunctionBlockDI::executePowerlinkEvent(const TEventID paEIID,
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
            for (int i = 3; i < getFBInterfaceSpec().mNumDOs && it != itEnd; i++, ++it) {
                bool ioVal = *(it->mCurrentValue) != 0x00;
                *static_cast<CIEC_BOOL *>(getDO(i)) = CIEC_BOOL(ioVal);
            }
            sync.unlock();
        }
        var_QO = var_QI;
        sendOutputEvent(scmEventCNFID, paECET);
    }
}
