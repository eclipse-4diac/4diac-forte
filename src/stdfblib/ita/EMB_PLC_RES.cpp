/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *******************************************************************************/
#include "EMB_PLC_RES.h"

#include <stringdict.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EMB_PLC_RES_gen.cpp"
#endif

#include <ecetFactory.h>
#include <ecetplc.h>
DEFINE_FIRMWARE_FB(EMB_PLC_RES, g_nStringIdEMB_PLC_RES);

const CStringDictionary::TStringId EMB_PLC_RES::scm_aunDINameIds[] = { g_nStringIdCYCLE_TIME };
const CStringDictionary::TStringId EMB_PLC_RES::scm_aunDIDataTypeIds[] = { g_nStringIdTIME };

const SFBInterfaceSpec EMB_PLC_RES::scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0, 0, 1, scm_aunDINameIds, scm_aunDIDataTypeIds, 0, 0, 0, 0, 0 };

EMB_PLC_RES::EMB_PLC_RES(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poDevice) :
        CResource(pa_poDevice, CEventChainExecutionThreadFactory::createEventChainExecutionThread<CEventChainExecutionThread61131>(), &scm_stFBInterfaceSpec,
          pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
}

EMGMResponse EMB_PLC_RES::changeFBExecutionState(EMGMCommandType pa_unCommand) {
  EMGMResponse retVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  if(e_RDY == retVal) {
    retVal = changeContainedFBsExecutionState(pa_unCommand);
    if(e_RDY == retVal) {
      if(cg_nMGM_CMD_Start == pa_unCommand && 0 != m_pstInterfaceSpec) { //on start, sample inputs
        for(int i = 0; i < m_pstInterfaceSpec->m_nNumDIs; ++i) {
          if(0 != m_apoDIConns[i]) {
            m_apoDIConns[i]->readData(getDI(i));
          }
        }
        DEVLOG_INFO("xx: Set Cycle Time to: %d ms\n", CYCLE_TIME().getInMilliSeconds());
        ((CEventChainExecutionThread61131*) getResourceEventExecution())->setCycleTime(CYCLE_TIME().getInMilliSeconds());
      }
      if(0 != getResourceEventExecution()) {
        getResourceEventExecution()->changeExecutionState(pa_unCommand);
      }
    }
  }
  return retVal;
}

void EMB_PLC_RES::postFBcreateAction(CFunctionBlock &paNewFB) {
  SEventEntry *newEventEntry = new SEventEntry;
  SEventEntry *newInitEventEntry = new SEventEntry;
  TPortId initEI = paNewFB.getEIID(g_nStringIdINIT);
  (*newEventEntry).mFB = &paNewFB;
  (*newEventEntry).mPortId = paNewFB.getEIID(CStringDictionary::getInstance().getId("REQ"));
  CResource *res = paNewFB.getResourcePtr();
  CEventChainExecutionThread61131 *executionThread = reinterpret_cast<CEventChainExecutionThread61131*>(res->getResourceEventExecution());
  if(cg_nInvalidEventID != initEI) {
    if(strstr(paNewFB.getInstanceName(), "SUBSCRIBE") || strstr(paNewFB.getInstanceName(), "SUBL_")) {
      (*newEventEntry).mPortId = cg_nExternal61131EventID;
    }
    (*newInitEventEntry).mPortId = initEI;
    (*newInitEventEntry).mFB = &paNewFB;
    executionThread->addInitEventEntry(newInitEventEntry);
  }
  executionThread->addEventEntry(newEventEntry);
}

EMB_PLC_RES::~EMB_PLC_RES() {
}

