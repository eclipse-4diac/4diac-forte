/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gunnar Grabmair, Rene Smodic, Gerhard Ebenhofer,
 *     Martin Melik Merkumians, Ingo Hegny, Micheal Hofmann
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - add support for data types with different size
 *   Alois Zoitl  - reworked and modernized, added support for type hashes
 *                - split into internal and public api
 *   Pedro Ricardo C. Souza - registry for types defined at run time
 *******************************************************************************/

#pragma once

#include "forte/typelib.h"
#include "forte/stringid.h"
#include <vector>

#ifdef FORTE_DYNAMIC_TYPE_LOAD
#include <atomic>
#include <memory>
#include <string_view>
#endif

namespace forte {
  class CAdapter;
  class CFBContainer;
  class CFunctionBlock;
  class CResource;
  class CIEC_ANY;

  /*!\brief Create a new FB instance of given type and given instance name.
   *
   * \param paInstanceNameId  StringId of instance name as this information can be stored within the resource
   * \param paFBTypeId Type of the FB to create.
   * \param paContainer   FB container the FB is contained in.
   * \return On success a pointer to the new FB is returned, else the return value is 0 use getLastError for getting
   * more information on the problem. possible error codes are:
   *    - UnsupportedType   The requested FB type is not known to the typelib
   *    - InvalidOperation The requested FB can not be created (e.g. out of memory)
   */
  CFunctionBlock *createFB(StringId paInstanceNameId,
                           StringId paFBTypeId,
                           std::string_view paTypeHash,
                           CFBContainer &paContainer,
                           EMGMResponse &paErrorMSG);

  CFunctionBlock *createFB(StringId paInstanceNameId, StringId paFBTypeId, CFBContainer &paContainer);

  /*\brief Delete the given FB
   */
  bool deleteFB(CFunctionBlock *paFBToDelete);

  CAdapter *createAdapter(StringId paInstanceNameId,
                          StringId paAdapterTypeId,
                          CFBContainer &paContainer,
                          bool paIsPlug,
                          TForteUInt8 paParentAdapterlistID,
                          EMGMResponse &paErrorMSG);

  CIEC_ANY *createDataTypeInstance(StringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG);

  CFBTypeEntry *getFBTypeEntry(StringId paTypeNameId);
  CAdapterTypeEntry *getAdapterTypeEntry(StringId paTypeNameId);
  CGlobalConstEntry *getGlobalConstTypeEntry(StringId paTypeNameId);

  const std::vector<CFBTypeEntry *> &getFBTypeEntries();
  const std::vector<CAdapterTypeEntry *> &getAdapterTypeEntries();
  const std::vector<CDataTypeEntry *> &getDataTypeEntries();
  const std::vector<CGlobalConstEntry *> &getGlobalConstEntries();

#ifdef FORTE_DYNAMIC_TYPE_LOAD
  /*!\brief Base of the type entries created at run time (e.g. from a management command)
   *
   * Unlike the firmware entries these are owned by shared pointers. Instances keep their entry alive, so a type
   * can be replaced while old instances still use the old definition.
   */
  class CRuntimeTypeEntry : public CTypeEntry {
    public:
      virtual ~CRuntimeTypeEntry() = default;

      //! the source the type was created from, used to recognise a repeated definition
      [[nodiscard]] virtual std::string_view getDefinition() const = 0;

      [[nodiscard]] std::size_t getInstanceCount() const {
        return mInstanceCount.load();
      }

      void addInstance() const {
        ++mInstanceCount;
      }

      void removeInstance() const {
        --mInstanceCount;
      }

    protected:
      /*!\brief Sets name and hash of the type
       *
       * @param paTypeHash the version of the definition that type queries report and instance creation checks. Only
       *                   the view is kept, so the text has to stay valid as long as the entry.
       */
      CRuntimeTypeEntry(StringId paTypeNameId, std::string_view paTypeHash);

    private:
      mutable std::atomic<std::size_t> mInstanceCount{0};
  };

  class CRuntimeFBTypeEntry : public CRuntimeTypeEntry {
    public:
      virtual CFunctionBlock *createFBInstance(StringId paInstanceNameId, CFBContainer &paContainer) const = 0;

    protected:
      using CRuntimeTypeEntry::CRuntimeTypeEntry;
  };

  class CRuntimeAdapterTypeEntry : public CRuntimeTypeEntry {
    public:
      virtual CAdapter *createAdapterInstance(StringId paInstanceNameId,
                                              CFBContainer &paContainer,
                                              bool paIsPlug,
                                              TForteUInt8 paParentAdapterlistID) const = 0;

    protected:
      using CRuntimeTypeEntry::CRuntimeTypeEntry;
  };

  /*!\brief Adds or replaces a run-time FB type
   *
   * @return Ready when added, replaced, or when the same definition is already registered;
   *         InvalidOperation when a firmware type has that name;
   *         InvalidState when a different definition is registered and still has instances
   */
  EMGMResponse registerRuntimeFBType(std::shared_ptr<const CRuntimeFBTypeEntry> paEntry);
  EMGMResponse registerRuntimeAdapterType(std::shared_ptr<const CRuntimeAdapterTypeEntry> paEntry);

  std::shared_ptr<const CRuntimeFBTypeEntry> getRuntimeFBTypeEntry(StringId paTypeNameId);
  std::shared_ptr<const CRuntimeAdapterTypeEntry> getRuntimeAdapterTypeEntry(StringId paTypeNameId);

  //! sorted names of the run-time types
  std::vector<StringId> getRuntimeFBTypeNames();
  std::vector<StringId> getRuntimeAdapterTypeNames();
#endif // FORTE_DYNAMIC_TYPE_LOAD
} // namespace forte
