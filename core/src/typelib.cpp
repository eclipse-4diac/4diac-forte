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

namespace forte {
  namespace {
    std::vector<CFBTypeEntry *> &getFBTypeLib() {
      static std::vector<CFBTypeEntry *> *fbTypeLib = new std::vector<CFBTypeEntry *>();
      return *fbTypeLib;
    }

    std::vector<CAdapterTypeEntry *> &getAdapterTypeLib() {
      static std::vector<CAdapterTypeEntry *> *adapterTypeLib = new std::vector<CAdapterTypeEntry *>();
      return *adapterTypeLib;
    }

    std::vector<CDataTypeEntry *> &getDataTypeLib() {
      static std::vector<CDataTypeEntry *> *dataTypeLib = new std::vector<CDataTypeEntry *>();
      return *dataTypeLib;
    }

    std::vector<CGlobalConstEntry *> &getGlobalConstTypeLib() {
      static std::vector<CGlobalConstEntry *> *globalConstTypeLib = new std::vector<CGlobalConstEntry *>();
      return *globalConstTypeLib;
    }

    CFunctionBlock *createGenericFB(StringId paInstanceNameId,
                                    StringId paFBTypeId,
                                    CFBContainer &paContainer,
                                    EMGMResponse &paErrorMSG);

    //! find the position of the first underscore that marks the end of the type name and the beginning of the generic
    //! part
    std::size_t getFirstNonTypeNameUnderscorePos(std::string_view paTypeName);

    /*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
     */
    void addFBType(CFBTypeEntry *paFBTypeEntry);

    /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
     */
    void addAdapterType(CAdapterTypeEntry *paAdapterTypeEntry);

    /*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
     */
    void addDataType(CDataTypeEntry *paDTEntry);

    void addGlobalConstType(CGlobalConstEntry *paGlobalConstTypeEntry);

    template<typename T>
    T *findTypeEntry(std::vector<T *> &vec, StringId paTypeNameId);

    template<typename T>
    T *findGenericTypeEntry(std::vector<T *> &vec, StringId paTypeNameId);

  }; // namespace

  constexpr CTypeEntry::CTypeEntry(StringId paTypeNameId, std::string_view paTypeHash) :
      mTypeNameId(paTypeNameId),
      mTypeHash(paTypeHash) {
  }

  CFBTypeEntry::CFBTypeEntry(StringId paTypeNameId, std::string_view paTypeHash, TFunctionBlockCreateFunc paCreateFB) :
      CTypeEntry(paTypeNameId, paTypeHash),
      mFBCreationFunc(paCreateFB) {
    addFBType(this);
  }

  CAdapterTypeEntry::CAdapterTypeEntry(StringId paTypeNameId,
                                       std::string_view paTypeHash,
                                       TAdapterCreateFunc paCreateAdapter) :
      CTypeEntry(paTypeNameId, paTypeHash),
      mAdapterCreationFunc(paCreateAdapter) {
    addAdapterType(this);
  }

  CDataTypeEntry::CDataTypeEntry(StringId paTypeNameId,
                                 std::string_view paTypeHash,
                                 TDataTypeCreateFunc paCreateDT,
                                 size_t paSize) :
      CTypeEntry(paTypeNameId, paTypeHash),
      mDTCreateFunc(paCreateDT),
      mSize(paSize) {
    addDataType(this);
  }

  CGlobalConstEntry::CGlobalConstEntry(StringId paTypeNameId, std::string_view paTypeHash) :
      CTypeEntry(paTypeNameId, paTypeHash) {
    addGlobalConstType(this);
  }

  CAdapter *createAdapter(StringId paInstanceNameId,
                          StringId paAdapterTypeId,
                          CFBContainer &paContainer,
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

  CFunctionBlock *createFB(StringId paInstanceNameId,
                           StringId paFBTypeId,
                           std::string_view paTypeHash,
                           CFBContainer &paContainer,
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

  CFunctionBlock *createFB(StringId paInstanceNameId, StringId paFBTypeId, CFBContainer &paContainer) {
    EMGMResponse errorMSG;
    return createFB(paInstanceNameId, paFBTypeId, std::string_view{}, paContainer, errorMSG);
  }

  bool deleteFB(CFunctionBlock *paFBToDelete) {
    paFBToDelete->deinitialize();
    delete paFBToDelete;
    return true;
  }

  CIEC_ANY *createDataTypeInstance(StringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG) {
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

  CIEC_ANY *createDataTypeInstance(StringId paDTNameId, TForteByte *paDataBuf) {
    EMGMResponse errorMSG;
    return createDataTypeInstance(paDTNameId, paDataBuf, errorMSG);
  }

  namespace {

    template<typename T>
    T *findTypeEntry(std::vector<T *> &vec, const StringId paTypeNameId) {
      auto it = std::lower_bound(vec.begin(), vec.end(), paTypeNameId,
                                 [](T *paTypeEnry, const StringId paId) { return paTypeEnry->getTypeNameId() < paId; });
      if (it != vec.end() && (*it)->getTypeNameId() == paTypeNameId) {
        return *it;
      }
      return nullptr;
    }

    template<typename T>
    T *findGenericTypeEntry(std::vector<T *> &vec, const StringId paTypeNameId) {
      std::size_t underScore = getFirstNonTypeNameUnderscorePos(paTypeNameId);
      if (underScore == std::string_view::npos) {
        // We found no underscore in the type name, so attempt the full name for a match
        underScore = paTypeNameId.size();
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

      return findTypeEntry(vec, StringId::lookup(genFBName));
    }

  } // namespace

  CFBTypeEntry *getFBTypeEntry(const StringId paTypeNameId) {
    if (const auto entry = findTypeEntry(getFBTypeLib(), paTypeNameId); entry != nullptr) {
      return entry;
    }
    return findGenericTypeEntry(getFBTypeLib(), paTypeNameId);
  }

  CAdapterTypeEntry *getAdapterTypeEntry(const StringId paTypeNameId) {
    return findTypeEntry(getAdapterTypeLib(), paTypeNameId);
  }

  CDataTypeEntry *getDataTypeEntry(const StringId paTypeNameId) {
    return findTypeEntry(getDataTypeLib(), paTypeNameId);
  }

  CGlobalConstEntry *getGlobalConstTypeEntry(const StringId paTypeNameId) {
    return findTypeEntry(getGlobalConstTypeLib(), paTypeNameId);
  }

  const std::vector<CFBTypeEntry *> &getFBTypeEntries() {
    return getFBTypeLib();
  }

  const std::vector<CAdapterTypeEntry *> &getAdapterTypeEntries() {
    return getAdapterTypeLib();
  }

  const std::vector<CDataTypeEntry *> &getDataTypeEntries() {
    return getDataTypeLib();
  }

  const std::vector<CGlobalConstEntry *> &getGlobalConstEntries() {
    return getGlobalConstTypeLib();
  }

  namespace {

    CFunctionBlock *createGenericFB(const StringId paInstanceNameId,
                                    const StringId paFBTypeId,
                                    CFBContainer &paContainer,
                                    EMGMResponse &paErrorMSG) {
      CFBTypeEntry *typeEntry = findGenericTypeEntry(getFBTypeLib(), paFBTypeId);
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
        if (result != std::string_view::npos && util::isDigit(paTypeName[result + 1])) {
          // only when the element after the underscore is a digit it is a correct type name
          break;
        }
      } while (result < paTypeName.size());

      return result;
    }

    template<typename T>
    void sortedTypeEntryInsert(std::vector<T *> &paTypeLib, T *paTypeEntry) {
      auto pos =
          std::lower_bound(paTypeLib.begin(), paTypeLib.end(), paTypeEntry, [](const T *paLeft, const T *paRight) {
            return paLeft->getTypeNameId() < paRight->getTypeNameId();
          });

      if (pos != paTypeLib.end() && (*pos)->getTypeNameId() == paTypeEntry->getTypeNameId()) {
        // entry with the same name is already in the typelib
        return;
      }

      paTypeLib.insert(pos, paTypeEntry);
    }

    void addFBType(CFBTypeEntry *paFBTypeEntry) {
      sortedTypeEntryInsert(getFBTypeLib(), paFBTypeEntry);
    }

    void addAdapterType(CAdapterTypeEntry *paAdapterTypeEntry) {
      sortedTypeEntryInsert(getAdapterTypeLib(), paAdapterTypeEntry);
    }

    void addDataType(CDataTypeEntry *paDTEntry) {
      sortedTypeEntryInsert(getDataTypeLib(), paDTEntry);
    }

    void addGlobalConstType(CGlobalConstEntry *paGlobalConstTypeEntry) {
      sortedTypeEntryInsert(getGlobalConstTypeLib(), paGlobalConstTypeEntry);
    }

  }; // namespace
} // namespace forte
