//
// Created by GafertM on 21.06.2024.
//

#ifndef POWERLINKFUNCTIONBLOCKDO_H
#define POWERLINKFUNCTIONBLOCKDO_H

#include "funcbloc.h"
#include "forte_sync.h"

#include "EplCNCallback.h"
#include "EplWrapper.h"

class PowerlinkFunctionBlockDO : public CFunctionBlock, public IEplCNCallback {
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


#endif //POWERLINKFUNCTIONBLOCKDO_H
