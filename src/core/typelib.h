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
 *******************************************************************************/
#pragma once

#include "core/datatype.h"
#include "core/stringid.h"
#include "core/mgmcmd.h"
#include <string_view>

// forward declaration of a few classes to reduce include file dependencies
class CFunctionBlock;
class CResource;
class CIEC_ANY;

namespace forte {
  class CAdapter;
  namespace core {
    class CFBContainer;
  }
} // namespace forte

//!\ingroup CORE Type for a function pointer which allows to create a functionblock instance
typedef CFunctionBlock *(*TFunctionBlockCreateFunc)(forte::core::StringId paInstanceNameId,
                                                    forte::core::CFBContainer &paContainer);

//!\ingroup CORE Type for a function pointer which allows to create an adapter instance
typedef forte::CAdapter *(*TAdapterCreateFunc)(forte::core::StringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer,
                                               bool paIsPlug,
                                               TForteUInt8 paParentAdapterlistID);

//!\ingroup CORE Type for a function pointer which allows to create a data type instance
typedef CIEC_ANY *(*TDataTypeCreateFunc)(TForteByte *paDataBuf);

//!\ingroup CORE This define is used to create the definition necessary for generic FirmwareFunction blocks in order to
//! get them automatically added to the FirmwareType list.
#define DECLARE_GENERIC_FIRMWARE_FB(fbclass)                                                                           \
private:                                                                                                               \
  const static forte::core::CFBTypeEntry csmFirmwareFBEntry_##fbclass;                                                 \
                                                                                                                       \
public:                                                                                                                \
  static CFunctionBlock *createFB(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) {    \
    return new fbclass(paInstanceNameId, paContainer);                                                                 \
  };                                                                                                                   \
                                                                                                                       \
private:

//!\ingroup CORE This define is used to create the definition necessary for FirmwareFunction blocks in order to get them
//! automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_FB(fbclass)                                                                                   \
  DECLARE_GENERIC_FIRMWARE_FB(fbclass)                                                                                 \
public:                                                                                                                \
  forte::core::StringId getFBTypeId() const override;                                                                  \
                                                                                                                       \
private:

#define DEFINE_GENERIC_FIRMWARE_FB(fbclass, fbTypeNameId)                                                              \
  const forte::core::CFBTypeEntry fbclass::csmFirmwareFBEntry_##fbclass((fbTypeNameId), std::string_view{},            \
                                                                        fbclass::createFB);

#define GET_TYPE_HASH(_1, ...) _1

/*!\ingroup CORE This define is used to create the implementation for the above definition. The second parameter is
 * needed for the prebuild script that generates the constant string list.
 */
#define DEFINE_FIRMWARE_FB(fbclass, fbTypeNameId, ...)                                                                 \
  const forte::core::CFBTypeEntry fbclass::csmFirmwareFBEntry_##fbclass(                                               \
      (fbTypeNameId), GET_TYPE_HASH(__VA_ARGS__ __VA_OPT__(, ) std::string_view{}), fbclass::createFB);                \
  forte::core::StringId fbclass::getFBTypeId() const {                                                                 \
    return (fbTypeNameId);                                                                                             \
  }

//!\ingroup CORE This define is used to create the definition necessary for Adapter types.
#define DECLARE_ADAPTER_TYPE(adapterclass)                                                                             \
private:                                                                                                               \
  const static forte::core::CAdapterTypeEntry csmAdapterTypeEntry_##adapterclass;                                      \
                                                                                                                       \
public:                                                                                                                \
  static forte::CAdapter *createAdapter(forte::core::StringId paInstanceNameId,                                        \
                                        forte::core::CFBContainer &paContainer, bool paIsPlug,                         \
                                        TForteUInt8 paParentAdapterlistID);                                            \
  forte::core::StringId getFBTypeId() const override {                                                                 \
    return (csmAdapterTypeEntry_##adapterclass.getTypeNameId());                                                       \
  };                                                                                                                   \
                                                                                                                       \
private:

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_ADAPTER_TYPE(adapterclass, adapterTypeNameId, ...)                                                      \
  const forte::core::CAdapterTypeEntry adapterclass::csmAdapterTypeEntry_##adapterclass(                               \
      (adapterTypeNameId), GET_TYPE_HASH(__VA_ARGS__ __VA_OPT__(, ) std::string_view{}), adapterclass::createAdapter); \
  forte::CAdapter *adapterclass::createAdapter(forte::core::StringId paInstanceNameId,                                 \
                                               forte::core::CFBContainer &paContainer, bool paIsPlug,                  \
                                               TForteUInt8 paParentAdapterlistID) {                                    \
    if (paIsPlug) {                                                                                                    \
      return new adapterclass##_Plug(paInstanceNameId, paContainer, paParentAdapterlistID);                            \
    }                                                                                                                  \
    return new adapterclass##_Socket(paInstanceNameId, paContainer, paParentAdapterlistID);                            \
  };

#define DEFINE_GENERIC_ADAPTER_TYPE(adapterclass, adapterTypeNameId)                                                   \
  const forte::core::CAdapterTypeEntry adapterclass::csmAdapterTypeEntry_##adapterclass(                               \
      (adapterTypeNameId), std::string_view{}, adapterclass::createAdapter, 0);

//!\ingroup CORE This define is used to create the definition necessary for Firmware datatype in order to get them
//! automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_DATATYPE(datatypename)                                                                        \
public:                                                                                                                \
  static CIEC_ANY *createDataType(TForteByte *paDataBuf) {                                                             \
    return (0 != paDataBuf) ? new (paDataBuf) CIEC_##datatypename() : new CIEC_##datatypename;                         \
  };                                                                                                                   \
  size_t getSizeof() const override {                                                                                  \
    return sizeof(CIEC_##datatypename);                                                                                \
  }                                                                                                                    \
  CIEC_ANY *clone(TForteByte *paDataBuf) const override {                                                              \
    return (0 != paDataBuf) ? new (paDataBuf) CIEC_##datatypename(*this) : new CIEC_##datatypename(*this);             \
  }                                                                                                                    \
  forte::core::StringId getTypeNameID() const override {                                                               \
    return CIEC_##datatypename::csmFirmwareDataTypeEntry_##datatypename.getTypeNameId();                               \
  }                                                                                                                    \
                                                                                                                       \
private:                                                                                                               \
  const static forte::core::CDataTypeEntry csmFirmwareDataTypeEntry_##datatypename;

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_FIRMWARE_DATATYPE(datatypename, datatypenameid, ...)                                                    \
  const forte::core::CDataTypeEntry CIEC_##datatypename::csmFirmwareDataTypeEntry_##datatypename(                      \
      (datatypenameid), GET_TYPE_HASH(__VA_ARGS__ __VA_OPT__(, ) std::string_view{}),                                  \
      CIEC_##datatypename::createDataType, sizeof(CIEC_##datatypename));

#define DECLARE_FIRMWARE_GLOBAL_CONST()                                                                                \
private:                                                                                                               \
  const static forte::core::CGlobalConstEntry csmGlobalConstEntry;

#define DEFINE_FIRMWARE_GLOBAL_CONST(gcClass, gcTypeNameId, ...)                                                       \
  const forte::core::CGlobalConstEntry gcClass::csmGlobalConstEntry(                                                   \
      (gcTypeNameId), GET_TYPE_HASH(__VA_ARGS__ __VA_OPT__(, ) std::string_view{}));

struct SFBInterfaceSpec;

namespace forte::core {

  //! The base class for all type entries in the type lib.
  class CTypeEntry {

    public:
      constexpr forte::core::StringId getTypeNameId() const {
        return mTypeNameId;
      }

      const char *getTypeName() const {
        return getTypeNameId().data();
      }

      constexpr std::string_view getTypeHash() const {
        return mTypeHash;
      }

    protected:
      constexpr explicit CTypeEntry(forte::core::StringId paTypeNameId, std::string_view paTypeHash);

    private:
      forte::core::StringId mTypeNameId;
      std::string_view mTypeHash;
  };

  //! The base class for all function block types entries in the type lib.
  class CFBTypeEntry : public CTypeEntry {
    public:
      CFBTypeEntry(forte::core::StringId paTypeNameId,
                   std::string_view paTypeHash,
                   TFunctionBlockCreateFunc paCreateFB);

      CFunctionBlock *createFBInstance(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) {
        return mFBCreationFunc(paInstanceNameId, paContainer);
      }

    private:
      TFunctionBlockCreateFunc mFBCreationFunc;
  };

  /*!\brief Class for adapter type entries in the type lib.
   */
  class CAdapterTypeEntry : public CTypeEntry {
    public:
      CAdapterTypeEntry(forte::core::StringId paTypeNameId,
                        std::string_view paTypeHash,
                        TAdapterCreateFunc paCreateAdapter);

      forte::CAdapter *createAdapterInstance(forte::core::StringId paInstanceNameId,
                                             forte::core::CFBContainer &paContainer,
                                             bool paIsPlug,
                                             TForteUInt8 paParentAdapterlistID) {
        return mAdapterCreationFunc(paInstanceNameId, paContainer, paIsPlug, paParentAdapterlistID);
      }

    private:
      TAdapterCreateFunc mAdapterCreationFunc;
  };

  //! The base class for all data type entries in the type lib.
  class CDataTypeEntry : public CTypeEntry {
    public:
      CDataTypeEntry(forte::core::StringId paTypeNameId,
                     std::string_view paTypeHash,
                     TDataTypeCreateFunc paCreateDT,
                     size_t paSize);

      CIEC_ANY *createDataTypeInstance(TForteByte *paDataBuf) {
        return mDTCreateFunc(paDataBuf);
      }

      [[nodiscard]] constexpr size_t getSize() const {
        return mSize;
      }

    protected:
      TDataTypeCreateFunc mDTCreateFunc;
      size_t mSize;
  };

  class CGlobalConstEntry : public CTypeEntry {
    public:
      CGlobalConstEntry(forte::core::StringId paTypeNameId, std::string_view paTypeHash);
  };

  /*!\brief Create an instance of an data type.
   *
   * @param paDTNameId string id of the datatype to create
   * @param paDataBuf buffer that the datatype should use. Has to be at least the size of CIEC_ANY
   * This is indicated with a return value of nullptr.
   * @return pointer to the create data type.
   */
  CIEC_ANY *createDataTypeInstance(forte::core::StringId paDTNameId, TForteByte *paDataBuf);

  CDataTypeEntry *getDataTypeEntry(forte::core::StringId paTypeNameId);

} // namespace forte::core
