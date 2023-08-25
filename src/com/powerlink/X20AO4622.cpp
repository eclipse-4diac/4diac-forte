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

const CStringDictionary::TStringId FORTE_X20AO4622::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID, g_nStringIdAO01, g_nStringIdAO02, g_nStringIdAO03, g_nStringIdAO04 };

const CStringDictionary::TStringId FORTE_X20AO4622::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT };

const CStringDictionary::TStringId FORTE_X20AO4622::scmDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_X20AO4622::scmDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING };

const TForteInt16 FORTE_X20AO4622::scmEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20AO4622::scmEIWith[] = { 0, 1, 2, scmWithListDelimiter, 3, 4, 5, 6, 0, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_X20AO4622::scmEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20AO4622::scmEOWith[] = { 0, 1, 2, scmWithListDelimiter, 2, 0, scmWithListDelimiter };
const TForteInt16 FORTE_X20AO4622::scmEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20AO4622::scmEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20AO4622::scmFBInterfaceSpec = { 2, scmEventInputNames, scmEIWith, scmEIWithIndexes, 2, scmEventOutputNames, scmEOWith, scmEOWithIndexes, 7, scmDataInputNames, scmDataInputTypeIds, 3, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_X20AO4622::executeEvent(TEventID paEIID){
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
          for(unsigned int i = 0; i < moduleIOs->getNrOfEntries(); i++){
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
        for(int i = 3; i < mInterfaceSpec->mNumDIs && it != itEnd; i++, ++it){
          short ioVal = *static_cast<CIEC_INT*>(getDI(i));
          *((short*) (it->mCurrentValue)) = ioVal;
        }
        mSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_X20AO4622::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  mSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
  for(it; it != itEnd; ++it){
    short ioVal = *((short*) (it->mCurrentValue));
    char highByte = (char) ((ioVal & 0xFF00) >> 8);
    char lowByte = (char) (ioVal & 0x00FF);
    (eplStack.getProcImageIn())[it->mPiOffset] &= (~(0xFF << it->mBitOffset));
    (eplStack.getProcImageIn())[it->mPiOffset] |= (lowByte << (it->mBitOffset));

    (eplStack.getProcImageIn())[it->mPiOffset + 1] &= (~(0xFF << it->mBitOffset));
    (eplStack.getProcImageIn())[it->mPiOffset + 1] |= (highByte << (it->mBitOffset));
  }

  mSync.unlock();
}
