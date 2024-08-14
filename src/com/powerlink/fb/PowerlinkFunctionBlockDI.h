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

#ifndef POWERLINKFUNCTIONBLOCKDI_H
#define POWERLINKFUNCTIONBLOCKDI_H

#include "funcbloc.h"
#include "forte_sync.h"

#include "EplCNCallback.h"
#include "EplWrapper.h"

class PowerlinkFunctionBlockDI : public CFunctionBlock, public IEplCNCallback {
    EplMapping eplMapping;
    CSyncObject sync;
    bool initOk;

    void cnSynchCallback() override;

    using CFunctionBlock::CFunctionBlock;

protected:
    void executePowerlinkEvent(TEventID paEIID,
                               CEventChainExecutionThread *paECET,
                               TEventID scmEventINITID,
                               TEventID scmEventREQID,
                               TEventID scmEventINITOID,
                               TEventID scmEventCNFID,
                               const CIEC_BOOL &var_QI,
                               CIEC_BOOL &var_QO,
                               const CIEC_USINT &var_CNID,
                               const CIEC_UINT &var_MODID);
};


#endif //POWERLINKFUNCTIONBLOCKDI_H
