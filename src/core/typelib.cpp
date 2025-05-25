/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Martin Erich Jobst
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
#include "adapterconn.h"
#include "adapter.h"
#include "stringdict.h"
#include <stddef.h>
#include <string_view>
#include "string_utils.h"

USE_STRING_ID(ARRAY);

using namespace std::string_literals;

namespace {
  std::vector<CTypeLib::CFBTypeEntry *> &getFBTypeLib() {
    static std::vector<CTypeLib::CFBTypeEntry *> *fbTypeLib = new std::vector<CTypeLib::CFBTypeEntry *>();
    return *fbTypeLib;
  }

  std::vector<CTypeLib::CAdapterTypeEntry *> &getAdapterTypeLib() {
    static std::vector<CTypeLib::CAdapterTypeEntry *> *adapterTypeLib =
        new std::vector<CTypeLib::CAdapterTypeEntry *>();
    return *adapterTypeLib;
  }

  std::vector<CTypeLib::CDataTypeEntry *> &getDataTypeLib() {
    static std::vector<CTypeLib::CDataTypeEntry *> *dataTypeLib = new std::vector<CTypeLib::CDataTypeEntry *>();
    return *dataTypeLib;
  }

  CFunctionBlock *createGenericFB(CStringDictionary::TStringId paInstanceNameId,
                                  CStringDictionary::TStringId paFBTypeId,
                                  forte::core::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG);

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
  //! part
  const char *getFirstNonTypeNameUnderscorePos(const char *paTypeName);

  /*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
   */
  void addFBType(CTypeLib::CFBTypeEntry *paFBTypeEntry);

  /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
   */
  void addAdapterType(CTypeLib::CAdapterTypeEntry *paAdapterTypeEntry);

  /*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
   */
  void addDataType(CTypeLib::CDataTypeEntry *paDTEntry);

}; // namespace

constexpr CTypeLib::CTypeEntry::CTypeEntry(CStringDictionary::TStringId paTypeNameId, std::string_view paTypeHash) :
    mTypeNameId(paTypeNameId),
    mTypeHash(paTypeHash) {
}

constexpr CTypeLib::CSpecTypeEntry::CSpecTypeEntry(CStringDictionary::TStringId paTypeNameId,
                                                   std::string_view paTypeHash,
                                                   const SFBInterfaceSpec *paSocketInterfaceSpec) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mSocketInterfaceSpec(paSocketInterfaceSpec) {
}

CTypeLib::CFBTypeEntry::CFBTypeEntry(CStringDictionary::TStringId paTypeNameId,
                                     std::string_view paTypeHash,
                                     TFunctionBlockCreateFunc pa_pfuncCreateFB,
                                     const SFBInterfaceSpec *paSocketInterfaceSpec) :
    CSpecTypeEntry(paTypeNameId, paTypeHash, paSocketInterfaceSpec),
    m_pfuncFBCreationFunc(pa_pfuncCreateFB) {
  addFBType(this);
}

CTypeLib::CAdapterTypeEntry::CAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId,
                                               std::string_view paTypeHash,
                                               TAdapterCreateFunc pa_pfuncCreateAdapter,
                                               const SFBInterfaceSpec *paSocketInterfaceSpec) :
    CSpecTypeEntry(paTypeNameId, paTypeHash, paSocketInterfaceSpec),
    m_pfuncAdapterCreationFunc(pa_pfuncCreateAdapter) {
  addAdapterType(this);
}

CTypeLib::CDataTypeEntry::CDataTypeEntry(CStringDictionary::TStringId paTypeNameId,
                                         std::string_view paTypeHash,
                                         TDataTypeCreateFunc pafuncDTCreateFunc,
                                         size_t paSize) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mDTCreateFunc(pafuncDTCreateFunc),
    mSize(paSize) {
  addDataType(this);
}

CAdapter *CTypeLib::createAdapter(CStringDictionary::TStringId paInstanceNameId,
                                  CStringDictionary::TStringId paAdapterTypeId,
                                  forte::core::CFBContainer &paContainer,
                                  bool paIsPlug,
                                  EMGMResponse &paErrorMSG) {
  // CAdapter *poNewAdapter = nullptr;
  CAdapterTypeEntry *poToCreate = getAdapterTypeEntry(paAdapterTypeId);
  if (poToCreate == nullptr) {
    paErrorMSG = EMGMResponse::UnsupportedType;
    return nullptr;
  }

  CAdapter *newAdapter =
      (static_cast<CAdapterTypeEntry *>(poToCreate))->createAdapterInstance(paInstanceNameId, paContainer, paIsPlug);
  if (newAdapter == nullptr) {
    paErrorMSG = EMGMResponse::Overflow;
    return nullptr;
  }

  if (!newAdapter->initialize()) {
    delete newAdapter;
    paErrorMSG = EMGMResponse::Overflow;
    return nullptr;
  }

  return newAdapter;
}

CFunctionBlock *CTypeLib::createFB(CStringDictionary::TStringId paInstanceNameId,
                                   CStringDictionary::TStringId paFBTypeId,
                                   std::string_view paTypeHash,
                                   forte::core::CFBContainer &paContainer,
                                   EMGMResponse &paErrorMSG) {
  CFunctionBlock *newFB = nullptr;
  CFBTypeEntry *typeEntry = getFBTypeEntry(paFBTypeId);
  // TODO: Avoid that the user can create generic blocks.
  if (typeEntry != nullptr) {
    if (!paTypeHash.empty() && paTypeHash != typeEntry->getTypeHash()) {
      paErrorMSG = EMGMResponse::UnsupportedType;
      return nullptr;
    }

    newFB = typeEntry->createFBInstance(paInstanceNameId, paContainer);
    if (newFB == nullptr) { // we could not create the requested object
      paErrorMSG = EMGMResponse::Overflow;
    }
  } else { // check for parameterizable FBs (e.g. SERVER)
    newFB = createGenericFB(paInstanceNameId, paFBTypeId, paContainer, paErrorMSG);
  }

  if (newFB != nullptr && !newFB->initialize()) {
    delete newFB;
    paErrorMSG = EMGMResponse::Overflow;
    newFB = nullptr;
  }

  return newFB;
}

CFunctionBlock *CTypeLib::createFB(CStringDictionary::TStringId paInstanceNameId,
                                   CStringDictionary::TStringId paFBTypeId,
                                   forte::core::CFBContainer &paContainer) {
  EMGMResponse errorMSG;
  return CTypeLib::createFB(paInstanceNameId, paFBTypeId, std::string_view{}, paContainer, errorMSG);
}

namespace {
  void nextDataPoint(const CStringDictionary::TStringId *&paDataTypeIds) {
    while (*(paDataTypeIds++) == STRID(ARRAY)) {
      paDataTypeIds += 2;
    }
  }
} // namespace

CIEC_ANY *CTypeLib::createDataPoint(const CStringDictionary::TStringId *&paDataTypeIds, TForteByte *&paDataBuf) {
  CStringDictionary::TStringId dataTypeId = *paDataTypeIds;
  EMGMResponse errorMSG;
  CIEC_ANY *poRetVal = createDataTypeInstance(dataTypeId, paDataBuf, errorMSG);
  if (nullptr != poRetVal) {
    if (STRID(ARRAY) == dataTypeId) {
      static_cast<CIEC_ARRAY_DYNAMIC *>(poRetVal)->setup(paDataTypeIds + 1);
    }
    if (paDataBuf != nullptr) {
      paDataBuf += poRetVal->getSizeof();
    }
  }
  nextDataPoint(paDataTypeIds);
  return poRetVal;
}

bool CTypeLib::deleteFB(CFunctionBlock *paFBToDelete) {
  paFBToDelete->deinitialize();
  delete paFBToDelete;
  return true;
}

CIEC_ANY *CTypeLib::createDataTypeInstance(CStringDictionary::TStringId paDTNameId,
                                           TForteByte *paDataBuf,
                                           EMGMResponse &paErrorMSG) {
  CIEC_ANY *poNewDT = nullptr;
  CDataTypeEntry *poToCreate = getDataTypeEntry(paDTNameId);
  if (nullptr != poToCreate) {
    poNewDT = poToCreate->createDataTypeInstance(paDataBuf);
    if (nullptr == poNewDT) { // we could not create the requested object
      paErrorMSG = EMGMResponse::Overflow;
    }
  } else {
    paErrorMSG = EMGMResponse::UnsupportedType;
  }

  return poNewDT;
}

CIEC_ANY *CTypeLib::createDataTypeInstance(CStringDictionary::TStringId paDTNameId, TForteByte *paDataBuf) {
  EMGMResponse errorMSG;
  return createDataTypeInstance(paDTNameId, paDataBuf, errorMSG);
}

namespace {

  template<typename T>
  T *findTypeEntry(std::vector<T *> &vec, CStringDictionary::TStringId paTypeNameId) {
    auto it = std::lower_bound(vec.begin(), vec.end(), paTypeNameId,
                               [](T *paTypeEnry, CStringDictionary::TStringId paTypeNameId) {
                                 return paTypeEnry->getTypeNameId() < paTypeNameId;
                               });
    if (it != vec.end() && (*it)->getTypeNameId() == paTypeNameId) {
      return *it;
    }
    return nullptr;
  }

} // namespace

CTypeLib::CFBTypeEntry *CTypeLib::getFBTypeEntry(CStringDictionary::TStringId paTypeNameId) {
  return findTypeEntry(getFBTypeLib(), paTypeNameId);
}

CTypeLib::CAdapterTypeEntry *CTypeLib::getAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId) {
  return findTypeEntry(getAdapterTypeLib(), paTypeNameId);
}

CTypeLib::CDataTypeEntry *CTypeLib::getDataTypeEntry(CStringDictionary::TStringId paTypeNameId) {
  return findTypeEntry(getDataTypeLib(), paTypeNameId);
}

const std::vector<CTypeLib::CFBTypeEntry *> &CTypeLib::getFBTypeEntries() {
  return getFBTypeLib();
}

const std::vector<CTypeLib::CAdapterTypeEntry *> &CTypeLib::getAdapterTypeEntries() {
  return getAdapterTypeLib();
}

const std::vector<CTypeLib::CDataTypeEntry *> &CTypeLib::getDataTypeEntries() {
  return getDataTypeLib();
}

namespace {

  CFunctionBlock *createGenericFB(CStringDictionary::TStringId paInstanceNameId,
                                  CStringDictionary::TStringId paFBTypeId,
                                  forte::core::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG) {
    const char *const typeBuf = CStringDictionary::get(paFBTypeId);
    const char *const underScore = getFirstNonTypeNameUnderscorePos(typeBuf);

    if (underScore == nullptr) {
      // We found no underscore in the type name therefore it can not be a generic type
      paErrorMSG = EMGMResponse::UnsupportedType;
      return nullptr;
    }

    ptrdiff_t typeNameLen = underScore - typeBuf;
    std::string genFBName;
    genFBName.reserve(4 + typeNameLen);
    genFBName += "GEN_"s;
    genFBName.append(typeBuf, typeNameLen);

    CTypeLib::CFBTypeEntry *typeEntry = CTypeLib::getFBTypeEntry(CStringDictionary::getId(genFBName.c_str()));

    if (typeEntry == nullptr) {
      paErrorMSG = EMGMResponse::UnsupportedType;
      return nullptr;
    }

    CFunctionBlock *newFB = typeEntry->createFBInstance(paInstanceNameId, paContainer);
    if (newFB == nullptr) { // we could not create the requested object
      paErrorMSG = EMGMResponse::Overflow;
    } else if (!newFB->configureFB(typeBuf)) { // we got a configurable block
      delete newFB;
      paErrorMSG = EMGMResponse::Overflow;
      return nullptr;
    }

    return newFB;
  }

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
  //! part
  const char *getFirstNonTypeNameUnderscorePos(const char *paTypeName) {
    const char *acRetVal = paTypeName;

    do {
      acRetVal = strchr(acRetVal, '_');
      if (nullptr != acRetVal) {
        if (forte::core::util::isDigit(*(acRetVal + 1))) {
          // only when the element after the underscore is a digit it is a correct type name
          break;
        }
        acRetVal++;
      }
    } while (nullptr != acRetVal);

    return acRetVal;
  }

  template<typename T>
  void sortedTypeEntryInsert(std::vector<T *> &paTypeLib, T *paTypeEntry) {
    auto pos = std::lower_bound(paTypeLib.begin(), paTypeLib.end(), paTypeEntry, [](const T *paLeft, const T *paRight) {
      return paLeft->getTypeNameId() < paRight->getTypeNameId();
    });

    if (pos != paTypeLib.end() && (*pos)->getTypeNameId() == paTypeEntry->getTypeNameId()) {
      // entry with the same name is already in the typelib
      return;
    }

    paTypeLib.insert(pos, paTypeEntry);
  }

  void addFBType(CTypeLib::CFBTypeEntry *paFBTypeEntry) {
    sortedTypeEntryInsert(getFBTypeLib(), paFBTypeEntry);
  }

  void addAdapterType(CTypeLib::CAdapterTypeEntry *paAdapterTypeEntry) {
    sortedTypeEntryInsert(getAdapterTypeLib(), paAdapterTypeEntry);
  }

  void addDataType(CTypeLib::CDataTypeEntry *paDTEntry) {
    sortedTypeEntryInsert(getDataTypeLib(), paDTEntry);
  }

}; // namespace
