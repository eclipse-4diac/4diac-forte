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
#include "core/datatypes/forte_any.h"
#include "core/typelib_internal.h"
#include "core/adapter.h"
#include "core/stringid.h"
#include <stddef.h>
#include <string_view>
#include "core/util/string_utils.h"

using namespace forte::core::literals;

using namespace std::string_literals;

namespace {
  std::vector<forte::core::CFBTypeEntry *> &getFBTypeLib() {
    static std::vector<forte::core::CFBTypeEntry *> *fbTypeLib = new std::vector<forte::core::CFBTypeEntry *>();
    return *fbTypeLib;
  }

  std::vector<forte::core::CAdapterTypeEntry *> &getAdapterTypeLib() {
    static std::vector<forte::core::CAdapterTypeEntry *> *adapterTypeLib =
        new std::vector<forte::core::CAdapterTypeEntry *>();
    return *adapterTypeLib;
  }

  std::vector<forte::core::CDataTypeEntry *> &getDataTypeLib() {
    static std::vector<forte::core::CDataTypeEntry *> *dataTypeLib = new std::vector<forte::core::CDataTypeEntry *>();
    return *dataTypeLib;
  }

  std::vector<forte::core::CGlobalConstEntry *> &getGlobalConstTypeLib() {
    static std::vector<forte::core::CGlobalConstEntry *> *globalConstTypeLib =
        new std::vector<forte::core::CGlobalConstEntry *>();
    return *globalConstTypeLib;
  }

  CFunctionBlock *createGenericFB(forte::core::StringId paInstanceNameId,
                                  forte::core::StringId paFBTypeId,
                                  forte::core::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG);

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
  //! part
  std::size_t getFirstNonTypeNameUnderscorePos(std::string_view paTypeName);

  /*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
   */
  void addFBType(forte::core::CFBTypeEntry *paFBTypeEntry);

  /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
   */
  void addAdapterType(forte::core::CAdapterTypeEntry *paAdapterTypeEntry);

  /*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
   */
  void addDataType(forte::core::CDataTypeEntry *paDTEntry);

  void addGlobalConstType(forte::core::CGlobalConstEntry *paGlobalConstTypeEntry);

  template<typename T>
  T *findTypeEntry(std::vector<T *> &vec, forte::core::StringId paTypeNameId);

  template<typename T>
  T *findGenericTypeEntry(std::vector<T *> &vec, forte::core::StringId paTypeNameId);

}; // namespace

constexpr forte::core::CTypeEntry::CTypeEntry(forte::core::StringId paTypeNameId, std::string_view paTypeHash) :
    mTypeNameId(paTypeNameId),
    mTypeHash(paTypeHash) {
}

forte::core::CFBTypeEntry::CFBTypeEntry(forte::core::StringId paTypeNameId,
                                        std::string_view paTypeHash,
                                        TFunctionBlockCreateFunc paCreateFB) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mFBCreationFunc(paCreateFB) {
  addFBType(this);
}

forte::core::CAdapterTypeEntry::CAdapterTypeEntry(forte::core::StringId paTypeNameId,
                                                  std::string_view paTypeHash,
                                                  TAdapterCreateFunc paCreateAdapter) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mAdapterCreationFunc(paCreateAdapter) {
  addAdapterType(this);
}

forte::core::CDataTypeEntry::CDataTypeEntry(forte::core::StringId paTypeNameId,
                                            std::string_view paTypeHash,
                                            TDataTypeCreateFunc paCreateDT,
                                            size_t paSize) :
    CTypeEntry(paTypeNameId, paTypeHash),
    mDTCreateFunc(paCreateDT),
    mSize(paSize) {
  addDataType(this);
}

forte::core::CGlobalConstEntry::CGlobalConstEntry(forte::core::StringId paTypeNameId, std::string_view paTypeHash) :
    CTypeEntry(paTypeNameId, paTypeHash) {
  addGlobalConstType(this);
}

forte::CAdapter *forte::core::createAdapter(forte::core::StringId paInstanceNameId,
                                            forte::core::StringId paAdapterTypeId,
                                            forte::core::CFBContainer &paContainer,
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

CFunctionBlock *forte::core::createFB(forte::core::StringId paInstanceNameId,
                                      forte::core::StringId paFBTypeId,
                                      std::string_view paTypeHash,
                                      forte::core::CFBContainer &paContainer,
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

CFunctionBlock *forte::core::createFB(forte::core::StringId paInstanceNameId,
                                      forte::core::StringId paFBTypeId,
                                      forte::core::CFBContainer &paContainer) {
  EMGMResponse errorMSG;
  return forte::core::createFB(paInstanceNameId, paFBTypeId, std::string_view{}, paContainer, errorMSG);
}

bool forte::core::deleteFB(CFunctionBlock *paFBToDelete) {
  paFBToDelete->deinitialize();
  delete paFBToDelete;
  return true;
}

CIEC_ANY *
forte::core::createDataTypeInstance(forte::core::StringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG) {
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

CIEC_ANY *forte::core::createDataTypeInstance(forte::core::StringId paDTNameId, TForteByte *paDataBuf) {
  EMGMResponse errorMSG;
  return createDataTypeInstance(paDTNameId, paDataBuf, errorMSG);
}

namespace {

  template<typename T>
  T *findTypeEntry(std::vector<T *> &vec, const forte::core::StringId paTypeNameId) {
    auto it =
        std::lower_bound(vec.begin(), vec.end(), paTypeNameId, [](T *paTypeEnry, const forte::core::StringId paId) {
          return paTypeEnry->getTypeNameId() < paId;
        });
    if (it != vec.end() && (*it)->getTypeNameId() == paTypeNameId) {
      return *it;
    }
    return nullptr;
  }

  template<typename T>
  T *findGenericTypeEntry(std::vector<T *> &vec, const forte::core::StringId paTypeNameId) {
    const std::size_t underScore = getFirstNonTypeNameUnderscorePos(paTypeNameId);

    if (underScore == std::string_view::npos) {
      // We found no underscore in the type name, so it can't be a generic type
      return nullptr;
    }

    std::string genFBName;
    genFBName.reserve(4 + underScore);
    genFBName += "GEN_"s;
    genFBName.append(paTypeNameId, 0, underScore);

    return findTypeEntry(vec, forte::core::StringId::lookup(genFBName));
  }

} // namespace

forte::core::CFBTypeEntry *forte::core::getFBTypeEntry(const forte::core::StringId paTypeNameId) {
  if (const auto entry = findTypeEntry(getFBTypeLib(), paTypeNameId); entry != nullptr) {
    return entry;
  }
  return findGenericTypeEntry(getFBTypeLib(), paTypeNameId);
}

forte::core::CAdapterTypeEntry *forte::core::getAdapterTypeEntry(const forte::core::StringId paTypeNameId) {
  return findTypeEntry(getAdapterTypeLib(), paTypeNameId);
}

forte::core::CDataTypeEntry *forte::core::getDataTypeEntry(const forte::core::StringId paTypeNameId) {
  return findTypeEntry(getDataTypeLib(), paTypeNameId);
}

forte::core::CGlobalConstEntry *forte::core::getGlobalConstTypeEntry(const forte::core::StringId paTypeNameId) {
  return findTypeEntry(getGlobalConstTypeLib(), paTypeNameId);
}

const std::vector<forte::core::CFBTypeEntry *> &forte::core::getFBTypeEntries() {
  return getFBTypeLib();
}

const std::vector<forte::core::CAdapterTypeEntry *> &forte::core::getAdapterTypeEntries() {
  return getAdapterTypeLib();
}

const std::vector<forte::core::CDataTypeEntry *> &forte::core::getDataTypeEntries() {
  return getDataTypeLib();
}

const std::vector<forte::core::CGlobalConstEntry *> &forte::core::getGlobalConstEntries() {
  return getGlobalConstTypeLib();
}

namespace {

  CFunctionBlock *createGenericFB(const forte::core::StringId paInstanceNameId,
                                  const forte::core::StringId paFBTypeId,
                                  forte::core::CFBContainer &paContainer,
                                  EMGMResponse &paErrorMSG) {
    forte::core::CFBTypeEntry *typeEntry = findGenericTypeEntry(getFBTypeLib(), paFBTypeId);
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
      if (result != std::string_view::npos && forte::core::util::isDigit(paTypeName[result + 1])) {
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

  void addFBType(forte::core::CFBTypeEntry *paFBTypeEntry) {
    sortedTypeEntryInsert(getFBTypeLib(), paFBTypeEntry);
  }

  void addAdapterType(forte::core::CAdapterTypeEntry *paAdapterTypeEntry) {
    sortedTypeEntryInsert(getAdapterTypeLib(), paAdapterTypeEntry);
  }

  void addDataType(forte::core::CDataTypeEntry *paDTEntry) {
    sortedTypeEntryInsert(getDataTypeLib(), paDTEntry);
  }

  void addGlobalConstType(forte::core::CGlobalConstEntry *paGlobalConstTypeEntry) {
    sortedTypeEntryInsert(getGlobalConstTypeLib(), paGlobalConstTypeEntry);
  }

}; // namespace
