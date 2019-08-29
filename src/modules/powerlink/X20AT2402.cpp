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
#include "X20AT2402.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20AT2402_gen.cpp"
#endif

#include "EplWrapper.h"
#include "ProcessImageMatrix.h"

DEFINE_FIRMWARE_FB(FORTE_X20AT2402, g_nStringIdX20AT2402)

const CStringDictionary::TStringId FORTE_X20AT2402::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID };

const CStringDictionary::TStringId FORTE_X20AT2402::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_X20AT2402::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdT01, g_nStringIdT02 };

const CStringDictionary::TStringId FORTE_X20AT2402::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdREAL, g_nStringIdREAL };

const TForteInt16 FORTE_X20AT2402::scm_anEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20AT2402::scm_anEIWith[] = { 0, 1, 2, 255, 0, 255 };
const CStringDictionary::TStringId FORTE_X20AT2402::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20AT2402::scm_anEOWith[] = { 0, 1, 2, 255, 2, 3, 4, 0, 255 };
const TForteInt16 FORTE_X20AT2402::scm_anEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20AT2402::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20AT2402::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 3, scm_anDataInputNames, scm_anDataInputTypeIds, 5, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_X20AT2402::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(QI() == true){
        m_bInitOk = false;

        CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

        // Get settings for intputs
        CProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixOut()->getModuleEntries(CNID(), MODID());

        if(moduleIOs){
          // Inputs (process inputs) always start with i = 1
          // Check xap.xml if a BitUnused is present
          for(unsigned int i = 1; i < moduleIOs->getNrOfEntries() - 1; i++){
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
        for(int i = 3; i < m_pstInterfaceSpec->m_nNumDOs && it != itEnd; i++, ++it){
          short ioVal = *((short*) (it->m_pchCurrentValue));
          TForteFloat ioValFloat = static_cast<TForteFloat>(ioVal);
          *static_cast<CIEC_REAL*>(getDO(i)) = ioValFloat / 10;
        }
        m_oSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_X20AT2402::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  m_oSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
  for(it; it != itEnd; ++it){
    short ioVal = 0x0000;
    char lowByte;
    char highByte;
    lowByte = (eplStack.getProcImageOut()[it->m_nPiOffset] & (0xFF << it->m_nBitOffset)) >> it->m_nBitOffset;
    highByte = (eplStack.getProcImageOut()[it->m_nPiOffset + 1] & (0xFF << it->m_nBitOffset)) >> it->m_nBitOffset;
    ioVal = (short) ((0xFF00 & (highByte << 8))) | (short) (0xFF & lowByte);

    *((short*) (it->m_pchCurrentValue)) = ioVal;
  }

  m_oSync.unlock();
}

