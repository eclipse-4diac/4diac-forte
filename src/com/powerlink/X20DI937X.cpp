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

const CStringDictionary::TStringId FORTE_X20DI937X::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID };

const CStringDictionary::TStringId FORTE_X20DI937X::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_X20DI937X::scmDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdDI01, g_nStringIdDI02, g_nStringIdDI03, g_nStringIdDI04, g_nStringIdDI05, g_nStringIdDI06, g_nStringIdDI07, g_nStringIdDI08, g_nStringIdDI09, g_nStringIdDI10, g_nStringIdDI11, g_nStringIdDI12 };

const CStringDictionary::TStringId FORTE_X20DI937X::scmDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };

const TForteInt16 FORTE_X20DI937X::scmEIWithIndexes[] = { 0, -1, -1 };
const TDataIOID FORTE_X20DI937X::scmEIWith[] = { 0, 1, 2, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_X20DI937X::scmEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ, g_nStringIdRSP };

const TDataIOID FORTE_X20DI937X::scmEOWith[] = { 0, 2, 1, scmWithListDelimiter, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, scmWithListDelimiter, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, scmWithListDelimiter };
const TForteInt16 FORTE_X20DI937X::scmEOWithIndexes[] = { 0, 4, 18, -1 };
const CStringDictionary::TStringId FORTE_X20DI937X::scmEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF, g_nStringIdIND };

const SFBInterfaceSpec FORTE_X20DI937X::scmFBInterfaceSpec = { 3, scmEventInputNames, scmEIWith, scmEIWithIndexes, 3, scmEventOutputNames, scmEOWith, scmEOWithIndexes, 3, scmDataInputNames, scmDataInputTypeIds, 15, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_X20DI937X::executeEvent(TEventID paEIID){
  switch (paEIID){
    case scmEventINITID: {
      //#error add code for INIT event!

      EplStackWrapper &eplStack = EplStackWrapper::getInstance();

      // Get settings for intputs
      ProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixOut()->getModuleEntries(CNID(), MODID());

      // Inputs (process inputs) always start with i = 1
      // Check xap.xml if a BitUnused is present
      for(unsigned int i = 1; i < moduleIOs->getNrOfEntries() - 1; i++){
        mEplMapping.mDataSizes[i] = moduleIOs->getEntry(i)[0];
        mEplMapping.mPiOffsets[i] = moduleIOs->getEntry(i)[1];
        mEplMapping.mBitOffsets[i] = moduleIOs->getEntry(i)[2];

        char* newVal = new char[mEplMapping.mDataSizes[i]]();
        mEplMapping.mCurrentValues.pushBack(newVal);
      }

      delete moduleIOs;

      eplStack.registerCallback(static_cast<IEplCNCallback*>(this));

      QO() = QI();
      CNIDO() = CNID();
      sendOutputEvent(scmEventINITOID);
      break;
    }
    case scmEventREQID: {
      mSync.lock();
      CSinglyLinkedList<char*>::Iterator itEnd = mEplMapping.mCurrentValues.end();
      CSinglyLinkedList<char*>::Iterator it = mEplMapping.mCurrentValues.begin();
      for(int i = 3; i < mInterfaceSpec->mNumDOs && it != itEnd; i++, ++it){
        bool ioVal = (bool) **it;
        *static_cast<CIEC_BOOL*>(getDO(i)) = ioVal;
      }
      mSync.unlock();

      sendOutputEvent(scmEventCNFID);
      break;
    }
    case scmEventRSPID:
      break;
  }
}

void FORTE_X20DI937X::cnSynchCallback(){
  EplStackWrapper &eplStack = EplStackWrapper::getInstance();

  mSync.lock();

  CSinglyLinkedList<char*>::Iterator itEnd = mEplMapping.mCurrentValues.end();
  CSinglyLinkedList<char*>::Iterator it = mEplMapping.mCurrentValues.begin();
  for(int i = 3, j = 1; i < mInterfaceSpec->mNumDOs && it != itEnd; i++, j++, ++it){
    bool ioVal = (bool) (eplStack.getProcImageOut()[mEplMapping.mPiOffsets[j]] & (char) (0x01 << mEplMapping.mBitOffsets[j]));
    **it = (char) ioVal;
  }

  mSync.unlock();
}
