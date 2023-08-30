/*******************************************************************************
 * Copyright (c) 2013 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "X20DI4653.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20DI4653_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_X20DI4653, g_nStringIdX20DI4653)

const CStringDictionary::TStringId FORTE_X20DI4653::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID};

const CStringDictionary::TStringId FORTE_X20DI4653::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_X20DI4653::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdDI01, g_nStringIdDI02, g_nStringIdDI03, g_nStringIdDI04};

const CStringDictionary::TStringId FORTE_X20DI4653::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL};

const TForteInt16 FORTE_X20DI4653::scmEIWithIndexes[] = {0, 4};
const TDataIOID FORTE_X20DI4653::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_X20DI4653::scmEOWith[] = {0, 2, 1, scmWithListDelimiter, 3, 4, 5, 6, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_X20DI4653::scmEOWithIndexes[] = {0, 4, -1};
const CStringDictionary::TStringId FORTE_X20DI4653::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec FORTE_X20DI4653::scmFBInterfaceSpec = {
  2,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  3,  scmDataInputNames, scmDataInputTypeIds,
  7,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};


void FORTE_X20DI4653::executeEvent(TEventID paEIID){
  switch(paEIID){
    case scmEventINITID:
      if(QI() == true){
        mInitOk = false;

        CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

        // Get settings for intputs
        CProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixOut()->getModuleEntries(CNID(), MODID());

        if(moduleIOs){
          // Inputs (process inputs) always start with i = 1
          // Check xap.xml if a BitUnused is present
          for(unsigned int i = 1; i < moduleIOs->getNrOfEntries() - 1; i++){
            mEplMapping.mCurrentValues.pushBack(new SEplMapping::SEplMappingValues(moduleIOs->getEntry(i)[0], moduleIOs->getEntry(i)[1], moduleIOs->getEntry(i)[2]));
          }

          delete moduleIOs;

          eplStack.registerCallback(static_cast<IEplCNCallback*>(this));

          mInitOk = true;
        }
      }
      QO() = QI();
      CNIDO() = CNID();
      sendOutputEvent(scmEventINITOID);
      break;
    case scmEventREQID:
      if(QI() == true && mInitOk){
        mSync.lock();
        SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
        SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
        for(int i = 3; i < mInterfaceSpec->mNumDOs && it != itEnd; i++, ++it){
          bool ioVal = *(it->mCurrentValue) != 0x00;
          *static_cast<CIEC_BOOL*>(getDO(i)) = ioVal;
        }
        mSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_X20DI4653::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  mSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
  for(; it != itEnd; ++it){
    bool ioVal = (eplStack.getProcImageOut()[it->mPiOffset] & (char) (0x01 << it->mBitOffset)) != 0x00;
    *(it->mCurrentValue) = (char) ioVal;
  }

  mSync.unlock();
}



