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
#include "X20AT4222.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "X20AT4222_gen.cpp"
#endif

#include "ProcessImageMatrix.h"

DEFINE_FIRMWARE_FB(FORTE_X20AT4222, g_nStringIdX20AT4222)

const CStringDictionary::TStringId FORTE_X20AT4222::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdCNID, g_nStringIdMODID };

const CStringDictionary::TStringId FORTE_X20AT4222::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT };

const CStringDictionary::TStringId FORTE_X20AT4222::scmDataOutputNames[] = { g_nStringIdQO, g_nStringIdCNIDO, g_nStringIdSTATUS, g_nStringIdT01, g_nStringIdT02, g_nStringIdT03, g_nStringIdT04 };

const CStringDictionary::TStringId FORTE_X20AT4222::scmDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdSTRING, g_nStringIdREAL, g_nStringIdREAL, g_nStringIdREAL, g_nStringIdREAL };

const TForteInt16 FORTE_X20AT4222::scmEIWithIndexes[] = { 0, 4 };
const TDataIOID FORTE_X20AT4222::scmEIWith[] = { 0, 1, 2, scmWithListDelimiter, 0, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_X20AT4222::scmEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_X20AT4222::scmEOWith[] = { 0, 1, 2, scmWithListDelimiter, 2, 3, 4, 0, 5, 6, scmWithListDelimiter };
const TForteInt16 FORTE_X20AT4222::scmEOWithIndexes[] = { 0, 4, -1 };
const CStringDictionary::TStringId FORTE_X20AT4222::scmEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_X20AT4222::scmFBInterfaceSpec = { 2, scmEventInputNames, scmEIWith, scmEIWithIndexes, 2, scmEventOutputNames, scmEOWith, scmEOWithIndexes, 3, scmDataInputNames, scmDataInputTypeIds, 7, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_X20AT4222::executeEvent(TEventID paEIID){
  switch (paEIID){
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
          short ioVal = *((short*) (it->mCurrentValue));
          TForteFloat ioValFloat = static_cast<TForteFloat>(ioVal);
          *static_cast<CIEC_REAL*>(getDO(i)) = ioValFloat / 10;
        }
        mSync.unlock();
      }
      QO() = QI();
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_X20AT4222::cnSynchCallback(){
  CEplStackWrapper &eplStack = CEplStackWrapper::getInstance();

  mSync.lock();

  SEplMapping::TEplMappingList::Iterator itEnd = mEplMapping.mCurrentValues.end();
  SEplMapping::TEplMappingList::Iterator it = mEplMapping.mCurrentValues.begin();
  for(it; it != itEnd; ++it){
    short ioVal = 0x0000;
    char lowByte;
    char highByte;
    lowByte = (eplStack.getProcImageOut()[it->mPiOffset] & (0xFF << it->mBitOffset)) >> it->mBitOffset;
    highByte = (eplStack.getProcImageOut()[it->mPiOffset + 1] & (0xFF << it->mBitOffset)) >> it->mBitOffset;
    ioVal = (short) ((0xFF00 & (highByte << 8))) | (short) (0xFF & lowByte);

    *((short*) (it->mCurrentValue)) = ioVal;
  }

  mSync.unlock();
}

