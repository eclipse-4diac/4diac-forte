/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Smodic Rene, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add support for data types with different size
 *******************************************************************************/
#include "./datatypes/forte_any.h"
#include "typelib.h"
#include "eventconn.h"
#include "dataconn.h"
#include "adapterconn.h"
#include "resource.h"
#include "if2indco.h"
#include "adapter.h"
#include <stddef.h>

CTypeLib::CTypeEntry::CTypeEntry(CStringDictionary::TStringId paTypeNameId) :
  mTypeNameId(paTypeNameId),
  mNext(nullptr){
}

CTypeLib::CTypeEntry::~CTypeEntry() = default;

CTypeLib::CSpecTypeEntry::CSpecTypeEntry(CStringDictionary::TStringId paTypeNameId, const SFBInterfaceSpec* paSocketInterfaceSpec) :
  CTypeEntry(paTypeNameId),
  mSocketInterfaceSpec(paSocketInterfaceSpec){
}

CTypeLib::CSpecTypeEntry::~CSpecTypeEntry() = default;

CTypeLib::CFBTypeEntry::CFBTypeEntry(CStringDictionary::TStringId paTypeNameId, TFunctionBlockCreateFunc pa_pfuncCreateFB, const SFBInterfaceSpec* paSocketInterfaceSpec):
  CSpecTypeEntry(paTypeNameId, paSocketInterfaceSpec),
  m_pfuncFBCreationFunc(pa_pfuncCreateFB){
CTypeLib::addFBType(this);

}


CTypeLib::CFBTypeEntry::~CFBTypeEntry() = default;

CTypeLib::CAdapterTypeEntry::CAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId, TAdapterCreateFunc pa_pfuncCreateAdapter, const SFBInterfaceSpec* paSocketInterfaceSpec):
  CSpecTypeEntry(paTypeNameId, paSocketInterfaceSpec),
  m_pfuncAdapterCreationFunc(pa_pfuncCreateAdapter){
CTypeLib::addAdapterType(this);

}

CTypeLib::CAdapterTypeEntry::~CAdapterTypeEntry() = default;

CTypeLib::CDataTypeEntry::CDataTypeEntry(CStringDictionary::TStringId paTypeNameId, TDataTypeCreateFunc pafuncDTCreateFunc, size_t paSize) :
    CTypeEntry(paTypeNameId), mDTCreateFunc(pafuncDTCreateFunc), mSize(paSize) {
  CTypeLib::addDataType(this);
}

CTypeLib::CDataTypeEntry::~CDataTypeEntry() = default;


EMGMResponse CTypeLib::mLastErrorMSG = EMGMResponse::Ready;

CTypeLib::CFBTypeEntry *CTypeLib::mFBLibStart = nullptr;
CTypeLib::CFBTypeEntry *CTypeLib::mFBLibEnd = nullptr;

CTypeLib::CAdapterTypeEntry *CTypeLib::mAdapterLibStart = nullptr;
CTypeLib::CAdapterTypeEntry *CTypeLib::mAdapterLibEnd = nullptr;

CTypeLib::CDataTypeEntry *CTypeLib::mDTLibStart = nullptr;
CTypeLib::CDataTypeEntry *CTypeLib::mDTLibEnd = nullptr;

CTypeLib::CTypeEntry *CTypeLib::findType(CStringDictionary::TStringId paTypeId, CTypeLib::CTypeEntry *paListStart) {
  CTypeEntry *retval = nullptr;
  for (CTypeEntry *poRunner = paListStart; poRunner != nullptr; poRunner
      = poRunner->mNext){
    if (paTypeId == poRunner->getTypeNameId()) {
      retval = poRunner;
      break;
    }
  }
  return retval;
}

CAdapter *CTypeLib::createAdapter(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paAdapterTypeId, forte::core::CFBContainer &paContainer, bool paIsPlug) {
  CAdapter *poNewAdapter = nullptr;
  CTypeEntry *poToCreate = findType(paAdapterTypeId, mAdapterLibStart);
  if (nullptr != poToCreate) {
    poNewAdapter =
      (static_cast<CAdapterTypeEntry *>(poToCreate))->createAdapterInstance(paInstanceNameId, paContainer, paIsPlug);
    if(nullptr == poNewAdapter) {
      mLastErrorMSG = EMGMResponse::Overflow;
    }
  } else { // no generic adapters supported
    mLastErrorMSG = EMGMResponse::UnsupportedType;
  }

  if(nullptr != poNewAdapter){
    if(!poNewAdapter->initialize()) {
      delete poNewAdapter;
      poNewAdapter = nullptr;
    }
  }

  return poNewAdapter;
}

CFunctionBlock *CTypeLib::createFB(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paFBTypeId, forte::core::CFBContainer &paContainer) {
  CFunctionBlock *poNewFB = nullptr;
  CTypeEntry *poToCreate = findType(paFBTypeId, mFBLibStart);
  //TODO: Avoid that the user can create generic blocks.
  if (nullptr != poToCreate) {
    poNewFB = (static_cast<CFBTypeEntry *>(poToCreate))->createFBInstance(paInstanceNameId, paContainer);
    if(nullptr == poNewFB) { // we could not create the requested object
      mLastErrorMSG = EMGMResponse::Overflow;
    }
  } else { //check for parameterizable FBs (e.g. SERVER)
    TIdentifier acGenFBName = { "GEN_" };
    const char *acTypeBuf = CStringDictionary::get(paFBTypeId);
    const char *pcUnderScore = getFirstNonTypeNameUnderscorePos(acTypeBuf);

    if (nullptr != pcUnderScore) { // We found no underscore in the type name therefore it can not be a generic type
      ptrdiff_t nCopyLen = pcUnderScore - acTypeBuf;
      if(nCopyLen > static_cast<ptrdiff_t>(cgIdentifierLength - 4)) {
        nCopyLen = cgIdentifierLength - 4;
      }
      memcpy(&(acGenFBName[4]), acTypeBuf, nCopyLen);
      acGenFBName[cgIdentifierLength] = '\0';
      poToCreate = findType(CStringDictionary::getId(acGenFBName), mFBLibStart);
      if (nullptr != poToCreate) {
        poNewFB = (static_cast<CFBTypeEntry *>(poToCreate))->createFBInstance(paInstanceNameId, paContainer);
        if (nullptr == poNewFB){ // we could not create the requested object
          mLastErrorMSG = EMGMResponse::Overflow;
        }
        else { // we got a configurable block
          if (!poNewFB->configureFB(acTypeBuf)) {
            deleteFB(poNewFB);
            poNewFB = nullptr;
          }
        }
      }
      else{
        mLastErrorMSG = EMGMResponse::UnsupportedType;
      }
    }
    else{
      mLastErrorMSG = EMGMResponse::UnsupportedType;
    }
  }

  if(nullptr != poNewFB && !poNewFB->initialize()) {
    deleteFB(poNewFB);
    poNewFB = nullptr;
  }

  return poNewFB;
}

bool CTypeLib::deleteFB(CFunctionBlock *paFBToDelete) {
  delete paFBToDelete;
  return true;
}

CIEC_ANY *CTypeLib::createDataTypeInstance(CStringDictionary::TStringId paDTNameId, TForteByte *paDataBuf) {
  CIEC_ANY *poNewDT = nullptr;
  CTypeEntry *poToCreate = findType(paDTNameId, mDTLibStart);
  if (nullptr != poToCreate) {
    poNewDT = (static_cast<CDataTypeEntry *>(poToCreate))->createDataTypeInstance(paDataBuf);
    if(nullptr == poNewDT) { // we could not create the requested object
      mLastErrorMSG = EMGMResponse::Overflow;
    }
  } else {
    mLastErrorMSG = EMGMResponse::UnsupportedType;
  }

  return poNewDT;
}

void CTypeLib::addFBType(CFBTypeEntry *paFBTypeEntry) {
  if (nullptr == findType(paFBTypeEntry->getTypeNameId(), mFBLibStart)) {
    if(mFBLibStart == nullptr) {
      mFBLibStart = paFBTypeEntry;
    } else {
      mFBLibEnd->mNext = paFBTypeEntry;
    }
    mFBLibEnd = paFBTypeEntry;
  }
}

void CTypeLib::addAdapterType(CAdapterTypeEntry *paAdapterTypeEntry) {
  if (nullptr == findType(paAdapterTypeEntry->getTypeNameId(), mAdapterLibStart)) {
    if(mAdapterLibStart == nullptr) {
      mAdapterLibStart = paAdapterTypeEntry;
    } else {
      mAdapterLibEnd->mNext = paAdapterTypeEntry;
    }
    mAdapterLibEnd = paAdapterTypeEntry;
  }
}


void CTypeLib::addDataType(CDataTypeEntry *paDTEntry) {
  if (nullptr == findType(paDTEntry->getTypeNameId(), mDTLibStart)) {
    if(mDTLibStart == nullptr) {
      mDTLibStart = paDTEntry;
    } else {
      mDTLibEnd->mNext = paDTEntry;
    }
    mDTLibEnd = paDTEntry;
  }
}


const char *CTypeLib::getFirstNonTypeNameUnderscorePos(const char *paTypeName){
  const char* acRetVal = paTypeName;

  do{
    acRetVal = strchr(acRetVal, '_');
    if(nullptr != acRetVal){
      if(forte::core::util::isDigit(*(acRetVal + 1))){
        //only when the element after the underscore is a digit it is a correct type name
        break;
      }
      acRetVal++;
    }
  } while(nullptr != acRetVal);

  return acRetVal;
}

