/*******************************************************************************
 * Copyright (c) 2012, 2013 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "X20DO9322.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20DO9322_gen.cpp"
#endif

#include "EplWrapper.h"
#include "ProcessImageMatrix.h"

DEFINE_FIRMWARE_FB(FORTE_X20DO9322, g_nStringIdX20DO9322)

const CStringDictionary::TStringId FORTE_X20DO9322::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID, g_nStringIdDO01, g_nStringIdDO02, g_nStringIdDO03, g_nStringIdDO04, g_nStringIdDO05, g_nStringIdDO06, g_nStringIdDO07, g_nStringIdDO08, g_nStringIdDO09, g_nStringIdDO10, g_nStringIdDO11, g_nStringIdDO12 };

const CStringDictionary::TStringId FORTE_X20DO9322::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };

const CStringDictionary::TStringId FORTE_X20DO9322::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_X20DO9322::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING };

const TForteInt16 FORTE_X20DO9322::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20DO9322::scm_anEIWith[] = { 0, 1, 2, 255, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 255 };
const CStringDictionary::TStringId FORTE_X20DO9322::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20DO9322::scm_anEOWith[] = { 0, 2, 1, 255, 2, 0, 255 };
const TForteInt16 FORTE_X20DO9322::scm_anEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20DO9322::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20DO9322::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 15, scm_anDataInputNames, scm_anDataInputTypeIds, 3, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_X20DO9322::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(QI() == true){
        m_bInitOk = false;

        CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

        // Get settings for intputs
        CProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixIn()->getModuleEntries(CNID(), MODID());

        if(moduleIOs){
          // Outputs (process inputs) always start with i = 0
          // Check xap.xml if a BitUnused is present
          for(unsigned int i = 0; i < moduleIOs->getNrOfEntries() - 1; i++){
            m_oEplMapping.m_lCurrentValues.pushBack(new SEplMapping::SEplMappingValues(moduleIOs->getEntry(i)[0], moduleIOs->getEntry(i)[1], moduleIOs->getEntry(i)[2]));
          }
          delete moduleIOs;

          eplStack.registerCallback(static_cast<IEplCNCallback*>(this));

          m_bInitOk = true;
        }
      }
      QO() = QI();
      CNIDO() = CNID();
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      if(QI() == true && m_bInitOk){
        m_oSync.lock();
        SEplMapping::TEplMappingList::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
        SEplMapping::TEplMappingList::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
        for(int i = 3; i < m_pstInterfaceSpec->m_nNumDIs && it != itEnd; i++, ++it){
          bool ioVal = *static_cast<CIEC_BOOL*>(getDI(i));
          *(it->m_pchCurrentValue) = (char) ioVal;
        }
        m_oSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_X20DO9322::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  m_oSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
  for(it; it != itEnd; ++it){
    bool ioVal = *(it->m_pchCurrentValue) != 0x00;
    (eplStack.getProcImageIn())[it->m_nPiOffset] &= (char) (~(0x01 << it->m_nBitOffset));
    (eplStack.getProcImageIn())[it->m_nPiOffset] |= (char) (ioVal << it->m_nBitOffset);
  }

  m_oSync.unlock();
}
