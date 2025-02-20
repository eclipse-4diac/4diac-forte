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
 *    Alois Zoitl, Gunnar Grabmair, Rene Smodic, Gerhard Ebenhofer,
 *    Martin Melik Merkumians, Ingo Hegny, Micheal Hofmann
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add support for data types with different size
 *******************************************************************************/
#ifndef _TYPELIB_H_
#define _TYPELIB_H_

#include "fortenew.h"
#include "mgmcmd.h"
#include "stringlist.h"

//forward declaration of a few classes to reduce include file dependencies
class CFunctionBlock;
class CResource;
class CIEC_ANY;
class CAdapter;

namespace forte {
  namespace core {
    class CFBContainer;
  }
}

//!\ingroup CORE Type for a function pointer which allows to create a functionblock instance
  typedef CFunctionBlock *(*TFunctionBlockCreateFunc)(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

//!\ingroup CORE Type for a function pointer which allows to create an adapter instance
  typedef CAdapter *(*TAdapterCreateFunc)(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer, bool paIsPlug);


//!\ingroup CORE Type for a function pointer which allows to create a data type instance
  typedef CIEC_ANY *(*TDataTypeCreateFunc)(TForteByte *paDataBuf);

#define FORTE_DUMMY_INIT_DEF(fbclass) int fbclass::dummyInit() {return 0; }
#define FORTE_DUMMY_INIT_DEC  static int dummyInit();


//!\ingroup CORE This define is used to create the definition necessary for generic FirmwareFunction blocks in order to get them automatically added to the FirmwareType list.
#define DECLARE_GENERIC_FIRMWARE_FB(fbclass) \
  private: \
    const static CTypeLib::CFBTypeEntry csmFirmwareFBEntry_##fbclass; \
  public:  \
    static CFunctionBlock *createFB(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer){ \
      return new fbclass( paInstanceNameId, paContainer);\
    }; \
    FORTE_DUMMY_INIT_DEC \
  private:


//!\ingroup CORE This define is used to create the definition necessary for FirmwareFunction blocks in order to get them automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_FB(fbclass) \
    DECLARE_GENERIC_FIRMWARE_FB(fbclass) \
  public: \
    CStringDictionary::TStringId getFBTypeId() const override;\
  private:

    //const SFBInterfaceSpec& getFBInterfaceSpec() const override { return scmFBInterfaceSpec;}

#define DEFINE_GENERIC_FIRMWARE_FB(fbclass, fbTypeNameId)\
  extern const CStringDictionary::TStringId g_nStringId##fbclass; \
  const CTypeLib::CFBTypeEntry fbclass::csmFirmwareFBEntry_##fbclass((fbTypeNameId), fbclass::createFB, 0); \
  FORTE_DUMMY_INIT_DEF(fbclass)

/*!\ingroup CORE This define is used to create the implementation for the above definition. The second parameter is needed for the
 * prebuild script that generates the constant string list.
 */
#define DEFINE_FIRMWARE_FB(fbclass, fbTypeNameId) \
    extern const CStringDictionary::TStringId g_nStringId##fbclass; \
    const CTypeLib::CFBTypeEntry fbclass::csmFirmwareFBEntry_##fbclass((fbTypeNameId), fbclass::createFB, &(fbclass::scmFBInterfaceSpec)); \
    FORTE_DUMMY_INIT_DEF(fbclass) \
    CStringDictionary::TStringId fbclass::getFBTypeId() const {return (fbTypeNameId); }

//!\ingroup CORE This define is used to create the definition necessary for Adapter types.
#define DECLARE_ADAPTER_TYPE(adapterclass) \
  private: \
    const static CTypeLib::CAdapterTypeEntry csmAdapterTypeEntry_##adapterclass; \
  public:  \
    static CAdapter *createAdapter(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer, bool paIsPlug){\
      return new adapterclass(paInstanceNameId, paContainer, paIsPlug);\
    }; \
    CStringDictionary::TStringId getFBTypeId() const override {return (csmAdapterTypeEntry_##adapterclass.getTypeNameId()); };\
    FORTE_DUMMY_INIT_DEC \
  private:

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_ADAPTER_TYPE(adapterclass, adapterTypeNameId)\
  const CTypeLib::CAdapterTypeEntry adapterclass::csmAdapterTypeEntry_##adapterclass((adapterTypeNameId), adapterclass::createAdapter, &(adapterclass::scmFBInterfaceSpecSocket)); \
  FORTE_DUMMY_INIT_DEF(adapterclass)

#define DEFINE_GENERIC_ADAPTER_TYPE(adapterclass, adapterTypeNameId)\
  const CTypeLib::CAdapterTypeEntry adapterclass::csmAdapterTypeEntry_##adapterclass((adapterTypeNameId), adapterclass::createAdapter, 0); \
  FORTE_DUMMY_INIT_DEF(adapterclass)

//!\ingroup CORE This define is used to create the definition necessary for Firmware datatype in order to get them automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_DATATYPE(datatypename) \
   public:  \
    static CIEC_ANY *createDataType(TForteByte *paDataBuf){ \
      return (0 != paDataBuf) ? new(paDataBuf)CIEC_##datatypename() : new CIEC_##datatypename; \
    }; \
    const static CTypeLib::CDataTypeEntry csmFirmwareDataTypeEntry_##datatypename; \
    size_t getSizeof() const override { \
      return sizeof(CIEC_##datatypename); \
    } \
    CIEC_ANY* clone(TForteByte *paDataBuf) const override { \
      return (0 != paDataBuf) ? new(paDataBuf)CIEC_##datatypename(*this) : new CIEC_##datatypename(*this); } \
    CStringDictionary::TStringId getTypeNameID() const override { \
        return CIEC_##datatypename::csmFirmwareDataTypeEntry_##datatypename.getTypeNameId(); \
    } \
  FORTE_DUMMY_INIT_DEC \
  private:

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_FIRMWARE_DATATYPE(datatypename, datatypenameid)\
  const CTypeLib::CDataTypeEntry CIEC_##datatypename::csmFirmwareDataTypeEntry_##datatypename((datatypenameid), CIEC_##datatypename::createDataType, sizeof(CIEC_##datatypename));\
  FORTE_DUMMY_INIT_DEF(CIEC_##datatypename)

struct SFBInterfaceSpec;

/*!\ingroup CORE \brief Class for storing the functionblock libraries.
 */
class CTypeLib{

  public:
//! The base class for all type entries in the type lib.
  class CTypeEntry{
    private:
      CStringDictionary::TStringId mTypeNameId;

    public:
      CTypeEntry *mNext; //!< a pointer to the next element in the list. Will be used to build single linked list of type entries.

      explicit CTypeEntry(CStringDictionary::TStringId paTypeNameId);
      virtual ~CTypeEntry();

      CStringDictionary::TStringId getTypeNameId() const { return mTypeNameId; };

      const char * getTypeName() const { return CStringDictionary::get(getTypeNameId()); };
  };

  class CSpecTypeEntry : public CTypeEntry {
    private:
      const SFBInterfaceSpec* mSocketInterfaceSpec;
    public:
      CSpecTypeEntry(CStringDictionary::TStringId paTypeNameId, const SFBInterfaceSpec* paSocketInterfaceSpec);
      ~CSpecTypeEntry() override;
      const SFBInterfaceSpec* getInterfaceSpec() const{ return mSocketInterfaceSpec; }
  };

//! The base class for all function block types entries in the type lib.
  class CFBTypeEntry : public CSpecTypeEntry{
    public:
      CFBTypeEntry(CStringDictionary::TStringId paTypeNameId, TFunctionBlockCreateFunc pa_pfuncCreateFB, const SFBInterfaceSpec* paSocketInterfaceSpec);
      ~CFBTypeEntry() override;
      virtual CFunctionBlock *createFBInstance(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer){
              return m_pfuncFBCreationFunc( paInstanceNameId, paContainer);
            }
    private:
       TFunctionBlockCreateFunc m_pfuncFBCreationFunc;
  };

/*!\brief Class for adapter type entries in the type lib.
 */
      class CAdapterTypeEntry : public CSpecTypeEntry{
        public:
          CAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId, TAdapterCreateFunc pa_pfuncCreateAdapter, const SFBInterfaceSpec* paSocketInterfaceSpec);
          ~CAdapterTypeEntry() override;
          virtual CAdapter *createAdapterInstance(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer, bool paIsPlug){
            return m_pfuncAdapterCreationFunc( paInstanceNameId, paContainer, paIsPlug);
          }
        private:
          TAdapterCreateFunc m_pfuncAdapterCreationFunc;
      };

//! The base class for all data type entries in the type lib.
  class CDataTypeEntry : public CTypeEntry{
    public:
      CDataTypeEntry(CStringDictionary::TStringId paTypeNameId, TDataTypeCreateFunc paDTCreateFunc, size_t paSize);
      ~CDataTypeEntry() override;
      virtual CIEC_ANY *createDataTypeInstance(TForteByte *paDataBuf){
        return mDTCreateFunc(paDataBuf);
      };
      [[nodiscard]] size_t getSize() const {
        return mSize;
      }
    protected:
      TDataTypeCreateFunc mDTCreateFunc;
      size_t mSize;
  };

public:
/*!\brief Create a new FB instance of given type and given instance name.
 *
 * \param paInstanceNameId  StringId of instance name as this information can be stored within the resource
 * \param paFBTypeId Type of the FB to create.
 * \param paContainer   FB container the FB is contained in.
 * \return On success a pointer to the new FB is returned, else the return value is 0 use getLastError for getting more information on the problem.
 *   possible error codes are:
 *    - UnsupportedType   The requested FB type is not known to the typelib
 *    - InvalidOperation The requested FB can not be created (e.g. out of memory)
 */
  static CFunctionBlock *createFB(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paFBTypeId, forte::core::CFBContainer &paContainer);

/*\brief Delete the given FB
 */
  static bool deleteFB(CFunctionBlock *paFBToDelete);

  static CAdapter *createAdapter(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paFBTypeId, forte::core::CFBContainer &paContainer, bool paIsPlug);

  /*!\brief Create an instance of an data type.
   *
   * @param paDTNameId string id of the datatype to create
   * @param paDataBuf buffer that the datatype should use. Has to be at least the size of CIEC_ANY
   * @return pointer to the create data type.
   */
  static CIEC_ANY *createDataTypeInstance(CStringDictionary::TStringId paDTNameId, TForteByte *paDataBuf);


/*!\brief Return a description of the last error that occured within the CTypeLib.
 *
 * e.g. Out of memory
 * \return Reference to the error string.
 */
  static EMGMResponse getLastError() { return mLastErrorMSG; };

/*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
 */
  static void addFBType(CFBTypeEntry *paFBTypeEntry);

  /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
   */
  static void addAdapterType(CAdapterTypeEntry *paAdapterTypeEntry);

/*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
 */
  static void addDataType(CDataTypeEntry *paDTEntry);
/*!\brief Get a pointer to the begin of the FB library list
 */
  static CTypeEntry *getFBLibStart() { return mFBLibStart; }

  /*!\brief Get a pointer to the begin of the FB library list
   */
    static CTypeEntry *getAdapterLibStart() { return mAdapterLibStart; }

  /*!\brief Get a pointer to the begin of the datatype library list
 */
  static CTypeEntry *getDTLibStart() { return mDTLibStart; }

  static CTypeEntry *findType(CStringDictionary::TStringId paTypeId, CTypeEntry *paListStart);

protected:
private:

/*!\brief Buffer for the last error that occurred.
 */
  static EMGMResponse mLastErrorMSG;

  static CFBTypeEntry *mFBLibStart, //!< pointer to the begin of the firmware fb library list
               *mFBLibEnd; //!<pointer to the end of the firmware fb library list

  static CAdapterTypeEntry *mAdapterLibStart, //!< pointer to the begin of the firmware adapter library list
               *mAdapterLibEnd; //!<pointer to the end of the firmware adapter library list

  static CDataTypeEntry *mDTLibStart, //!< pointer to the begin of the data type library
                 *mDTLibEnd; //!< pointer to the end of the data type library

  static CFunctionBlock *createGenericFB(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paFBTypeId, forte::core::CFBContainer &paContainer);

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic part
  static const char* getFirstNonTypeNameUnderscorePos(const char* paTypeName);
};

#endif /*TYPELIB_H_*/
