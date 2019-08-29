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
#include "X20AO4622.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20AO4622_gen.cpp"
#endif

#include "EplWrapper.h"
#include "ProcessImageMatrix.h"

DEFINE_FIRMWARE_FB(FORTE_X20AO4622, g_nStringIdX20AO4622)

const CStringDictionary::TStringId FORTE_X20AO4622::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID, g_nStringIdAO01, g_nStringIdAO02, g_nStringIdAO03, g_nStringIdAO04 };

const CStringDictionary::TStringId FORTE_X20AO4622::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT };

const CStringDictionary::TStringId FORTE_X20AO4622::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_X20AO4622::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING };

const TForteInt16 FORTE_X20AO4622::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20AO4622::scm_anEIWith[] = { 0, 1, 2, 255, 3, 4, 5, 6, 0, 255 };
const CStringDictionary::TStringId FORTE_X20AO4622::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20AO4622::scm_anEOWith[] = { 0, 1, 2, 255, 2, 0, 255 };
const TForteInt16 FORTE_X20AO4622::scm_anEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20AO4622::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20AO4622::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 7, scm_anDataInputNames, scm_anDataInputTypeIds, 3, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_X20AO4622::executeEvent(int pa_nEIID){
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
          for(unsigned int i = 0; i < moduleIOs->getNrOfEntries(); i++){
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
          short ioVal = *static_cast<CIEC_INT*>(getDI(i));
          *((short*) (it->m_pchCurrentValue)) = ioVal;
        }
        m_oSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_X20AO4622::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  m_oSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
  for(it; it != itEnd; ++it){
    short ioVal = *((short*) (it->m_pchCurrentValue));
    char highByte = (char) ((ioVal & 0xFF00) >> 8);
    char lowByte = (char) (ioVal & 0x00FF);
    (eplStack.getProcImageIn())[it->m_nPiOffset] &= (~(0xFF << it->m_nBitOffset));
    (eplStack.getProcImageIn())[it->m_nPiOffset] |= (lowByte << (it->m_nBitOffset));

    (eplStack.getProcImageIn())[it->m_nPiOffset + 1] &= (~(0xFF << it->m_nBitOffset));
    (eplStack.getProcImageIn())[it->m_nPiOffset + 1] |= (highByte << (it->m_nBitOffset));
  }

  m_oSync.unlock();
}
