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
#include "forte/datatypes/forte_any.h"
#include "typelib_internal.h"
#include "forte/adapter.h"
#include "forte/stringid.h"
#include <stddef.h>
#include <string_view>
#include "forte/util/string_utils.h"

using namespace forte::literals;

using namespace std::string_literals;

namespace {
  std::vector<forte::CFBTypeEntry *> &getFBTypeLib() {
    static std::vector<forte::CFBTypeEntry *> *fbTypeLib = new std::vector<forte::CFBTypeEntry *>();
    return *fbTypeLib;
  }

  std::vector<forte::CAdapterTypeEntry *> &getAdapterTypeLib() {
    static std::vector<forte::CAdapterTypeEntry *> *adapterTypeLib = new std::vector<forte::CAdapterTypeEntry *>();
    return *adapterTypeLib;
  }

  std::vector<forte::CDataTypeEntry *> &getDataTypeLib() {
    static std::vector<forte::CDataTypeEntry *> *dataTypeLib = new std::vector<forte::CDataTypeEntry *>();
    return *dataTypeLib;
  }

  std::vector<forte::CGlobalConstEntry *> &getGlobalConstTypeLib() {
    static std::vector<forte::CGlobalConstEntry *> *globalConstTypeLib = new std::vector<forte::CGlobalConstEntry *>();
    return *globalConstTypeLib;
  }

  CFunctionBlock *createGenericFB(forte::StringId paInstanceNameId,
                                  forte::StringId paFBTypeId,
                                  forte::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG);

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
  //! part
  std::size_t getFirstNonTypeNameUnderscorePos(std::string_view paTypeName);

  /*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
   */
  void addFBType(forte::CFBTypeEntry *paFBTypeEntry);

  /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
   */
  void addAdapterType(forte::CAdapterTypeEntry *paAdapterTypeEntry);

  /*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
   */
  void addDataType(forte::CDataTypeEntry *paDTEntry);

  void addGlobalConstType(forte::CGlobalConstEntry *paGlobalConstTypeEntry);

  template<typename T>
  T *findTypeEntry(std::vector<T *> &vec, forte::StringId paTypeNameId);

  template<typename T>
  T *findGenericTypeEntry(std::vector<T *> &vec, forte::StringId paTypeNameId);

}; // namespace

constexpr forte::CTypeEntry::CTypeEntry(forte::StringId paTypeNameId, std::string_view paTypeHash) :
    mTypeNameId(paTypeNameId),
    mTypeHash(paTypeHash) {
}

forte::CFBTypeEntry::CFBTypeEntry(forte::StringId paTypeNameId,
                                  std::string_view paTypeHash,
                                  TFunctionBlockCreateFunc paCreateFB) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mFBCreationFunc(paCreateFB) {
  addFBType(this);
}

forte::CAdapterTypeEntry::CAdapterTypeEntry(forte::StringId paTypeNameId,
                                            std::string_view paTypeHash,
                                            TAdapterCreateFunc paCreateAdapter) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mAdapterCreationFunc(paCreateAdapter) {
  addAdapterType(this);
}

forte::CDataTypeEntry::CDataTypeEntry(forte::StringId paTypeNameId,
                                      std::string_view paTypeHash,
                                      TDataTypeCreateFunc paCreateDT,
                                      size_t paSize) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mDTCreateFunc(paCreateDT),
    mSize(paSize) {
  addDataType(this);
}

forte::CGlobalConstEntry::CGlobalConstEntry(forte::StringId paTypeNameId, std::string_view paTypeHash) :
    CTypeEntry(paTypeNameId, paTypeHash) {
  addGlobalConstType(this);
}

forte::CAdapter *forte::createAdapter(forte::StringId paInstanceNameId,
                                      forte::StringId paAdapterTypeId,
                                      forte::CFBContainer &paContainer,
                                      bool paIsPlug,
                                      TForteUInt8 paParentAdapterlistID,
                                      EMGMResponse &paErrorMSG) {
  CAdapterTypeEntry *poToCreate = getAdapterTypeEntry(paAdapterTypeId);
  if (poToCreate == nullptr) {
    paErrorMSG = EMGMResponse::UnsupportedType;
    return nullptr;
  }

  CAdapter *newAdapter =
      poToCreate->createAdapterInstance(paInstanceNameId, paContainer, paIsPlug, paParentAdapterlistID);
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

CFunctionBlock *forte::createFB(forte::StringId paInstanceNameId,
                                forte::StringId paFBTypeId,
                                std::string_view paTypeHash,
                                forte::CFBContainer &paContainer,
                                EMGMResponse &paErrorMSG) {
  CFunctionBlock *newFB = nullptr;
  CFBTypeEntry *typeEntry = findTypeEntry(getFBTypeLib(), paFBTypeId);
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

CFunctionBlock *
forte::createFB(forte::StringId paInstanceNameId, forte::StringId paFBTypeId, forte::CFBContainer &paContainer) {
  EMGMResponse errorMSG;
  return forte::createFB(paInstanceNameId, paFBTypeId, std::string_view{}, paContainer, errorMSG);
}

bool forte::deleteFB(CFunctionBlock *paFBToDelete) {
  paFBToDelete->deinitialize();
  delete paFBToDelete;
  return true;
}

CIEC_ANY *forte::createDataTypeInstance(forte::StringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG) {
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

CIEC_ANY *forte::createDataTypeInstance(forte::StringId paDTNameId, TForteByte *paDataBuf) {
  EMGMResponse errorMSG;
  return createDataTypeInstance(paDTNameId, paDataBuf, errorMSG);
}

namespace {

  template<typename T>
  T *findTypeEntry(std::vector<T *> &vec, const forte::StringId paTypeNameId) {
    auto it = std::lower_bound(vec.begin(), vec.end(), paTypeNameId, [](T *paTypeEnry, const forte::StringId paId) {
      return paTypeEnry->getTypeNameId() < paId;
    });
    if (it != vec.end() && (*it)->getTypeNameId() == paTypeNameId) {
      return *it;
    }
    return nullptr;
  }

  template<typename T>
  T *findGenericTypeEntry(std::vector<T *> &vec, const forte::StringId paTypeNameId) {
    const std::size_t underScore = getFirstNonTypeNameUnderscorePos(paTypeNameId);
    if (underScore == std::string_view::npos) {
      // We found no underscore in the type name, so it can't be a generic type
      return nullptr;
    }

    std::string genFBName;
    genFBName.reserve(4 + underScore);
    if (const std::size_t lastPackageSeparator = paTypeNameId.get().rfind("::", underScore);
        lastPackageSeparator != std::string_view::npos) {
      genFBName.append(paTypeNameId, 0, lastPackageSeparator + 2);
      genFBName += "GEN_"s;
      genFBName.append(paTypeNameId, lastPackageSeparator + 2, underScore - lastPackageSeparator - 2);
    } else {
      genFBName += "GEN_"s;
      genFBName.append(paTypeNameId, 0, underScore);
    }

    return findTypeEntry(vec, forte::StringId::lookup(genFBName));
  }

} // namespace

forte::CFBTypeEntry *forte::getFBTypeEntry(const forte::StringId paTypeNameId) {
  if (const auto entry = findTypeEntry(getFBTypeLib(), paTypeNameId); entry != nullptr) {
    return entry;
  }
  return findGenericTypeEntry(getFBTypeLib(), paTypeNameId);
}

forte::CAdapterTypeEntry *forte::getAdapterTypeEntry(const forte::StringId paTypeNameId) {
  return findTypeEntry(getAdapterTypeLib(), paTypeNameId);
}

forte::CDataTypeEntry *forte::getDataTypeEntry(const forte::StringId paTypeNameId) {
  return findTypeEntry(getDataTypeLib(), paTypeNameId);
}

forte::CGlobalConstEntry *forte::getGlobalConstTypeEntry(const forte::StringId paTypeNameId) {
  return findTypeEntry(getGlobalConstTypeLib(), paTypeNameId);
}

const std::vector<forte::CFBTypeEntry *> &forte::getFBTypeEntries() {
  return getFBTypeLib();
}

const std::vector<forte::CAdapterTypeEntry *> &forte::getAdapterTypeEntries() {
  return getAdapterTypeLib();
}

const std::vector<forte::CDataTypeEntry *> &forte::getDataTypeEntries() {
  return getDataTypeLib();
}

const std::vector<forte::CGlobalConstEntry *> &forte::getGlobalConstEntries() {
  return getGlobalConstTypeLib();
}

namespace {

  CFunctionBlock *createGenericFB(const forte::StringId paInstanceNameId,
                                  const forte::StringId paFBTypeId,
                                  forte::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG) {
    forte::CFBTypeEntry *typeEntry = findGenericTypeEntry(getFBTypeLib(), paFBTypeId);
    if (typeEntry == nullptr) {
      paErrorMSG = EMGMResponse::UnsupportedType;
      return nullptr;
    }

    CFunctionBlock *newFB = typeEntry->createFBInstance(paInstanceNameId, paContainer);
    if (newFB == nullptr) { // we could not create the requested object
      paErrorMSG = EMGMResponse::Overflow;
    } else if (!newFB->configureFB(paFBTypeId.data())) { // we got a configurable block
      delete newFB;
      paErrorMSG = EMGMResponse::Overflow;
      return nullptr;
    }

    return newFB;
  }

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
  //! part
  std::size_t getFirstNonTypeNameUnderscorePos(const std::string_view paTypeName) {
    std::size_t result = 0;

    do {
      result = paTypeName.find('_', result + 1);
      if (result != std::string_view::npos && forte::util::isDigit(paTypeName[result + 1])) {
        // only when the element after the underscore is a digit it is a correct type name
        break;
      }
    } while (result < paTypeName.size());

    return result;
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

  void addFBType(forte::CFBTypeEntry *paFBTypeEntry) {
    sortedTypeEntryInsert(getFBTypeLib(), paFBTypeEntry);
  }

  void addAdapterType(forte::CAdapterTypeEntry *paAdapterTypeEntry) {
    sortedTypeEntryInsert(getAdapterTypeLib(), paAdapterTypeEntry);
  }

  void addDataType(forte::CDataTypeEntry *paDTEntry) {
    sortedTypeEntryInsert(getDataTypeLib(), paDTEntry);
  }

  void addGlobalConstType(forte::CGlobalConstEntry *paGlobalConstTypeEntry) {
    sortedTypeEntryInsert(getGlobalConstTypeLib(), paGlobalConstTypeEntry);
  }

}; // namespace
