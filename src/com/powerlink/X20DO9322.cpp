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

const CStringDictionary::TStringId FORTE_X20DO9322::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID, g_nStringIdDO01, g_nStringIdDO02, g_nStringIdDO03, g_nStringIdDO04, g_nStringIdDO05, g_nStringIdDO06, g_nStringIdDO07, g_nStringIdDO08, g_nStringIdDO09, g_nStringIdDO10, g_nStringIdDO11, g_nStringIdDO12 };

const CStringDictionary::TStringId FORTE_X20DO9322::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL };

const CStringDictionary::TStringId FORTE_X20DO9322::scmDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_X20DO9322::scmDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING };

const TForteInt16 FORTE_X20DO9322::scmEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20DO9322::scmEIWith[] = { 0, 1, 2, scmWithListDelimiter, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_X20DO9322::scmEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20DO9322::scmEOWith[] = { 0, 2, 1, scmWithListDelimiter, 2, 0, scmWithListDelimiter };
const TForteInt16 FORTE_X20DO9322::scmEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20DO9322::scmEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20DO9322::scmFBInterfaceSpec = { 2, scmEventInputNames, scmEIWith, scmEIWithIndexes, 2, scmEventOutputNames, scmEOWith, scmEOWithIndexes, 15, scmDataInputNames, scmDataInputTypeIds, 3, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_X20DO9322::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID){
    case scmEventINITID:
      if(QI() == true){
        mInitOk = false;

        CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

        // Get settings for intputs
        CProcessImageMatrix* moduleIOs = eplStack.getProcessImageMatrixIn()->getModuleEntries(CNID(), MODID());

        if(moduleIOs){
          // Outputs (process inputs) always start with i = 0
          // Check xap.xml if a BitUnused is present
          for(unsigned int i = 0; i < moduleIOs->getNrOfEntries() - 1; i++){
            mEplMapping.mCurrentValues.pushBack(new SEplMapping::SEplMappingValues(moduleIOs->getEntry(i)[0], moduleIOs->getEntry(i)[1], moduleIOs->getEntry(i)[2]));
          }
          delete moduleIOs;

          eplStack.registerCallback(static_cast<IEplCNCallback*>(this));

          mInitOk = true;
        }
      }
      QO() = QI();
      CNIDO() = CNID();
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      if(QI() == true && mInitOk){
        mSync.lock();
        SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
        SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
        for(int i = 3; i < getFBInterfaceSpec().mNumDIs && it != itEnd; i++, ++it){
          bool ioVal = *static_cast<CIEC_BOOL*>(getDI(i));
          *(it->mCurrentValue) = (char) ioVal;
        }
        mSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_X20DO9322::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  mSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
  for(; it != itEnd; ++it){
    bool ioVal = *(it->mCurrentValue) != 0x00;
    (eplStack.getProcImageIn())[it->mPiOffset] &= (char) (~(0x01 << it->mBitOffset));
    (eplStack.getProcImageIn())[it->mPiOffset] |= (char) (ioVal << it->mBitOffset);
  }

  mSync.unlock();
}
