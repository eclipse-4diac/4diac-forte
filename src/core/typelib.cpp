/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
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
 *******************************************************************************/
#include "./datatypes/forte_any.h"
#include "typelib.h"
#include "eventconn.h"
#include "dataconn.h"
#include "adapterconn.h"
#include "resource.h"
#include "if2indco.h"
#include <stddef.h>

CTypeLib::CTypeEntry::CTypeEntry(CStringDictionary::TStringId pa_nTypeNameId) :
  m_nTypeNameId(pa_nTypeNameId),
  m_poNext(nullptr){
}

CTypeLib::CTypeEntry::~CTypeEntry() = default;

CTypeLib::CSpecTypeEntry::CSpecTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, const SFBInterfaceSpec* paSocketInterfaceSpec) :
  CTypeEntry(pa_nTypeNameId),
  mSocketInterfaceSpec(paSocketInterfaceSpec){
}

CTypeLib::CSpecTypeEntry::~CSpecTypeEntry() = default;

CTypeLib::CFBTypeEntry::CFBTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, TFunctionBlockCreateFunc pa_pfuncCreateFB, const SFBInterfaceSpec* paSocketInterfaceSpec):
  CSpecTypeEntry(pa_nTypeNameId, paSocketInterfaceSpec),
  m_pfuncFBCreationFunc(pa_pfuncCreateFB){
CTypeLib::addFBType(this);

}


CTypeLib::CFBTypeEntry::~CFBTypeEntry() = default;

CTypeLib::CAdapterTypeEntry::CAdapterTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, TAdapterCreateFunc pa_pfuncCreateAdapter, const SFBInterfaceSpec* paSocketInterfaceSpec):
  CSpecTypeEntry(pa_nTypeNameId, paSocketInterfaceSpec),
  m_pfuncAdapterCreationFunc(pa_pfuncCreateAdapter){
CTypeLib::addAdapterType(this);

}

CTypeLib::CAdapterTypeEntry::~CAdapterTypeEntry() = default;

CTypeLib::CDataTypeEntry::CDataTypeEntry(CStringDictionary::TStringId paTypeNameId, TDataTypeCreateFunc pafuncDTCreateFunc) :
    CTypeEntry(paTypeNameId), m_pfuncDTCreateFunc(pafuncDTCreateFunc) {
  CTypeLib::addDataType(this);
}

CTypeLib::CDataTypeEntry::~CDataTypeEntry() = default;


EMGMResponse CTypeLib::m_eLastErrorMSG = e_RDY;

CTypeLib::CFBTypeEntry *CTypeLib::m_poFBLibStart = nullptr;
CTypeLib::CFBTypeEntry *CTypeLib::m_poFBLibEnd = nullptr;

CTypeLib::CAdapterTypeEntry *CTypeLib::m_poAdapterLibStart = nullptr;
CTypeLib::CAdapterTypeEntry *CTypeLib::m_poAdapterLibEnd = nullptr;

CTypeLib::CDataTypeEntry *CTypeLib::m_poDTLibStart = nullptr;
CTypeLib::CDataTypeEntry *CTypeLib::m_poDTLibEnd = nullptr;

CTypeLib::CTypeEntry *CTypeLib::findType(CStringDictionary::TStringId pa_nTypeId, CTypeLib::CTypeEntry *pa_poListStart) {
  CTypeEntry *retval = nullptr;
  for (CTypeEntry *poRunner = pa_poListStart; poRunner != nullptr; poRunner
      = poRunner->m_poNext){
    if (pa_nTypeId == poRunner->getTypeNameId()) {
      retval = poRunner;
      break;
    }
  }
  return retval;
}

CAdapter *CTypeLib::createAdapter(CStringDictionary::TStringId pa_nInstanceNameId, CStringDictionary::TStringId pa_nAdapterTypeId, CResource *pa_poRes, bool pa_bIsPlug) {
  CAdapter *poNewAdapter = nullptr;
  CTypeEntry *poToCreate = findType(pa_nAdapterTypeId, m_poAdapterLibStart);
  if (nullptr != poToCreate) {
    poNewAdapter =
      (static_cast<CAdapterTypeEntry *>(poToCreate))->createAdapterInstance(pa_nInstanceNameId,pa_poRes, pa_bIsPlug);
    if(nullptr == poNewAdapter) {
      m_eLastErrorMSG = e_OVERFLOW;
    }
  } //no generic adapters supported

    return poNewAdapter;

}

CFunctionBlock *CTypeLib::createFB(CStringDictionary::TStringId pa_nInstanceNameId, CStringDictionary::TStringId pa_nFBTypeId, CResource *pa_poRes) {
  CFunctionBlock *poNewFB = nullptr;
  CTypeEntry *poToCreate = findType(pa_nFBTypeId, m_poFBLibStart);
  //TODO: Avoid that the user can create generic blocks.
  if (nullptr != poToCreate) {
    poNewFB
        = (static_cast<CFBTypeEntry *>(poToCreate))->createFBInstance(pa_nInstanceNameId, pa_poRes);
    if(nullptr == poNewFB) { // we could not create the requested object
      m_eLastErrorMSG = e_OVERFLOW;
    }
  } else { //check for parameterizable FBs (e.g. SERVER)
    TIdentifier acGenFBName = { "GEN_" };
    const char *acTypeBuf = CStringDictionary::getInstance().get(pa_nFBTypeId);
    const char *pcUnderScore = getFirstNonTypeNameUnderscorePos(acTypeBuf);

    if (nullptr != pcUnderScore) { // We found no underscore in the type name therefore it can not be a generic type
      ptrdiff_t nCopyLen = pcUnderScore - acTypeBuf;
      if(nCopyLen > static_cast<ptrdiff_t>(cg_nIdentifierLength - 4)) {
        nCopyLen = cg_nIdentifierLength - 4;
      }
      memcpy(&(acGenFBName[4]), acTypeBuf, nCopyLen);
      acGenFBName[cg_nIdentifierLength] = '\0';
      poToCreate = findType(CStringDictionary::getInstance().getId(acGenFBName), m_poFBLibStart);
      if (nullptr != poToCreate) {
        poNewFB = (static_cast<CFBTypeEntry *>(poToCreate))->createFBInstance(pa_nInstanceNameId, pa_poRes);
        if (nullptr == poNewFB){ // we could not create the requested object
          m_eLastErrorMSG = e_OVERFLOW;
        }
        else { // we got a configurable block
          if (!poNewFB->configureFB(acTypeBuf)) {
            deleteFB(poNewFB);
            poNewFB = nullptr;
          }
        }
      }
      else{
        m_eLastErrorMSG = e_UNSUPPORTED_TYPE;
      }
    }
    else{
      m_eLastErrorMSG = e_UNSUPPORTED_TYPE;
    }
  }

  if(nullptr != poNewFB){
    // perform reset to move it into idle state and set the initial values
    poNewFB->changeFBExecutionState(cg_nMGM_CMD_Reset);
  }

  return poNewFB;
}

bool CTypeLib::deleteFB(CFunctionBlock *pa_poFBToDelete) {
  delete pa_poFBToDelete;
  return true;
}

CIEC_ANY *CTypeLib::createDataTypeInstance(CStringDictionary::TStringId pa_nDTNameId, TForteByte *pa_acDataBuf) {
  CIEC_ANY *poNewDT = nullptr;
  CTypeEntry *poToCreate = findType(pa_nDTNameId, m_poDTLibStart);
  if (nullptr != poToCreate) {
    poNewDT = (static_cast<CDataTypeEntry *>(poToCreate))->createDataTypeInstance(pa_acDataBuf);
    if(nullptr == poNewDT) { // we could not create the requested object
      m_eLastErrorMSG = e_OVERFLOW;
    }
  } else {
    m_eLastErrorMSG = e_UNSUPPORTED_TYPE;
  }

  return poNewDT;
}

void CTypeLib::addFBType(CFBTypeEntry *pa_poFBTypeEntry) {
  if (nullptr == findType(pa_poFBTypeEntry->getTypeNameId(), m_poFBLibStart)) {
    if(m_poFBLibStart == nullptr) {
      m_poFBLibStart = pa_poFBTypeEntry;
    } else {
      m_poFBLibEnd->m_poNext = pa_poFBTypeEntry;
    }
    m_poFBLibEnd = pa_poFBTypeEntry;
  }
}

void CTypeLib::addAdapterType(CAdapterTypeEntry *pa_poAdapterTypeEntry) {
  if (nullptr == findType(pa_poAdapterTypeEntry->getTypeNameId(), m_poAdapterLibStart)) {
    if(m_poAdapterLibStart == nullptr) {
      m_poAdapterLibStart = pa_poAdapterTypeEntry;
    } else {
      m_poAdapterLibEnd->m_poNext = pa_poAdapterTypeEntry;
    }
    m_poAdapterLibEnd = pa_poAdapterTypeEntry;
  }
}


void CTypeLib::addDataType(CDataTypeEntry *pa_poDTEntry) {
  if (nullptr == findType(pa_poDTEntry->getTypeNameId(), m_poDTLibStart)) {
    if(m_poDTLibStart == nullptr) {
      m_poDTLibStart = pa_poDTEntry;
    } else {
      m_poDTLibEnd->m_poNext = pa_poDTEntry;
    }
    m_poDTLibEnd = pa_poDTEntry;
  }
}


const char *CTypeLib::getFirstNonTypeNameUnderscorePos(const char *pa_acTypeName){
  const char* acRetVal = pa_acTypeName;

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

