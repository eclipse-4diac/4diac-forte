/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "X20DI937X.h"

#include "EplWrapper.h"
#include "ProcessImageMatrix.h"

DEFINE_FIRMWARE_FB(FORTE_X20DI937X, g_nStringIdX20DI937X)

const CStringDictionary::TStringId FORTE_X20DI937X::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID };

const CStringDictionary::TStringId FORTE_X20DI937X::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_X20DI937X::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdDI01, g_nStringIdDI02, g_nStringIdDI03, g_nStringIdDI04, g_nStringIdDI05, g_nStringIdDI06, g_nStringIdDI07, g_nStringIdDI08, g_nStringIdDI09, g_nStringIdDI10, g_nStringIdDI11, g_nStringIdDI12 };

const CStringDictionary::TStringId FORTE_X20DI937X::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };

const TForteInt16 FORTE_X20DI937X::scm_anEIWithIndexes[] = { 0, -1, -1 };
const TDataIOID FORTE_X20DI937X::scm_anEIWith[] = { 0, 1, 2, 255 };
const CStringDictionary::TStringId FORTE_X20DI937X::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ, g_nStringIdRSP };

const TDataIOID FORTE_X20DI937X::scm_anEOWith[] = { 0, 2, 1, 255, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 255, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 255 };
const TForteInt16 FORTE_X20DI937X::scm_anEOWithIndexes[] = { 0, 4, 18, -1 };
const CStringDictionary::TStringId FORTE_X20DI937X::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF, g_nStringIdIND };

const SFBInterfaceSpec FORTE_X20DI937X::scm_stFBInterfaceSpec = { 3, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 3, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 3, scm_anDataInputNames, scm_anDataInputTypeIds, 15, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_X20DI937X::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID: {
      //#error add code for INIT event!

      EplStackWrapper &eplStack = EplStackWrapper::getInstance();

      // Get settings for intputs
      ProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixOut()->getModuleEntries(CNID(), MODID());

      // Inputs (process inputs) always start with i = 1
      // Check xap.xml if a BitUnused is present
      for(unsigned int i = 1; i < moduleIOs->getNrOfEntries() - 1; i++){
        m_oEplMapping.m_anDataSizes[i] = moduleIOs->getEntry(i)[0];
        m_oEplMapping.m_anPiOffsets[i] = moduleIOs->getEntry(i)[1];
        m_oEplMapping.m_anBitOffsets[i] = moduleIOs->getEntry(i)[2];

        char* newVal = new char[m_oEplMapping.m_anDataSizes[i]]();
        m_oEplMapping.m_lCurrentValues.pushBack(newVal);
      }

      delete moduleIOs;

      eplStack.registerCallback(static_cast<IEplCNCallback*>(this));

      QO() = QI();
      CNIDO() = CNID();
      sendOutputEvent(scm_nEventINITOID);
      break;
    }
    case scm_nEventREQID: {
      m_oSync.lock();
      CSinglyLinkedList<char*>::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
      CSinglyLinkedList<char*>::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
      for(int i = 3; i < m_pstInterfaceSpec->m_nNumDOs && it != itEnd; i++, ++it){
        bool ioVal = (bool) **it;
        *static_cast<CIEC_BOOL*>(getDO(i)) = ioVal;
      }
      m_oSync.unlock();

      sendOutputEvent(scm_nEventCNFID);
      break;
    }
    case scm_nEventRSPID:
      break;
  }
}

void FORTE_X20DI937X::cnSynchCallback(){
  EplStackWrapper &eplStack = EplStackWrapper::getInstance();

  m_oSync.lock();

  CSinglyLinkedList<char*>::Iterator itEnd = m_oEplMapping.m_lCurrentValues.end();
  CSinglyLinkedList<char*>::Iterator it = m_oEplMapping.m_lCurrentValues.begin();
  for(int i = 3, j = 1; i < m_pstInterfaceSpec->m_nNumDOs && it != itEnd; i++, j++, ++it){
    bool ioVal = (bool) (eplStack.getProcImageOut()[m_oEplMapping.m_anPiOffsets[j]] & (char) (0x01 << m_oEplMapping.m_anBitOffsets[j]));
    **it = (char) ioVal;
  }

  m_oSync.unlock();
}
