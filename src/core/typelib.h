/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
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
 *******************************************************************************/
#ifndef _TYPELIB_H_
#define _TYPELIB_H_

#include <fortenew.h>
#include "mgmcmd.h"
#include <stringlist.h>
#include "./utils/staticassert.h"

//forward declaration of a few classes to reduce include file dependencies
class CFunctionBlock;
class CResource;
class CIEC_ANY;
class CAdapter;

//!\ingroup CORE Type for a function pointer which allows to create a functionblock instance
  typedef CFunctionBlock *(*TFunctionBlockCreateFunc)(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

//!\ingroup CORE Type for a function pointer which allows to create an adapter instance
  typedef CAdapter *(*TAdapterCreateFunc)(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug);


//!\ingroup CORE Type for a function pointer which allows to create a data type instance
  typedef CIEC_ANY *(*TDataTypeCreateFunc)(TForteByte *pa_acDataBuf);

#define FORTE_DUMMY_INIT_DEF(fbclass) int fbclass::dummyInit() {return 0; }
#define FORTE_DUMMY_INIT_DEC  static int dummyInit();


//!\ingroup CORE This define is used to create the definition necessary for generic FirmwareFunction blocks in order to get them automatically added to the FirmwareType list.
#define DECLARE_GENERIC_FIRMWARE_FB(fbclass) \
  private: \
    const static CTypeLib::CFBTypeEntry csm_oFirmwareFBEntry_##fbclass; \
  public:  \
    static CFunctionBlock *createFB(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes){ \
      return new fbclass( pa_nInstanceNameId, pa_poSrcRes);\
    }; \
    FORTE_DUMMY_INIT_DEC \
  private:


//!\ingroup CORE This define is used to create the definition necessary for FirmwareFunction blocks in order to get them automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_FB(fbclass) \
    DECLARE_GENERIC_FIRMWARE_FB(fbclass) \
  public: \
    virtual CStringDictionary::TStringId getFBTypeId(void) const;\
  private:


#define DEFINE_GENERIC_FIRMWARE_FB(fbclass, fbTypeNameId)\
  extern const CStringDictionary::TStringId g_nStringId##fbclass; \
  const CTypeLib::CFBTypeEntry fbclass::csm_oFirmwareFBEntry_##fbclass((fbTypeNameId), fbclass::createFB, 0); \
  FORTE_DUMMY_INIT_DEF(fbclass)

/*!\ingroup CORE This define is used to create the implementation for the above definition. The second parameter is needed for the
 * prebuild script that generates the constant string list.
 */
#define DEFINE_FIRMWARE_FB(fbclass, fbTypeNameId) \
    extern const CStringDictionary::TStringId g_nStringId##fbclass; \
    const CTypeLib::CFBTypeEntry fbclass::csm_oFirmwareFBEntry_##fbclass((fbTypeNameId), fbclass::createFB, &(fbclass::scm_stFBInterfaceSpec)); \
    FORTE_DUMMY_INIT_DEF(fbclass) \
    CStringDictionary::TStringId fbclass::getFBTypeId(void) const {return (fbTypeNameId); }

//!\ingroup CORE This define is used to create the definition necessary for Adapter types.
#define DECLARE_ADAPTER_TYPE(adapterclass) \
  private: \
    const static CTypeLib::CAdapterTypeEntry csm_oAdapterTypeEntry_##adapterclass; \
  public:  \
    static CAdapter *createAdapter(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug){\
      return new adapterclass(pa_nInstanceNameId, pa_poSrcRes, pa_bIsPlug);\
    }; \
    virtual CStringDictionary::TStringId getFBTypeId(void) const {return (csm_oAdapterTypeEntry_##adapterclass.getTypeNameId()); };\
    FORTE_DUMMY_INIT_DEC \
  private:

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_ADAPTER_TYPE(adapterclass, adapterTypeNameId)\
  const CTypeLib::CAdapterTypeEntry adapterclass::csm_oAdapterTypeEntry_##adapterclass((adapterTypeNameId), adapterclass::createAdapter, &(adapterclass::scm_stFBInterfaceSpecSocket)); \
  FORTE_DUMMY_INIT_DEF(adapterclass)

#define DEFINE_GENERIC_ADAPTER_TYPE(adapterclass, adapterTypeNameId)\
  const CTypeLib::CAdapterTypeEntry adapterclass::csm_oAdapterTypeEntry_##adapterclass((adapterTypeNameId), adapterclass::createAdapter, 0); \
  FORTE_DUMMY_INIT_DEF(adapterclass)

//!\ingroup CORE This define is used to create the definition necessary for Firmware datatype in order to get them automatically added to the FirmwareType list.
#define DECLARE_FIRMWARE_DATATYPE(datatypename) \
   public:  \
    static CIEC_ANY *createDataType(TForteByte *pa_acDataBuf){ \
      return (0 != pa_acDataBuf)  ? new(pa_acDataBuf)CIEC_##datatypename() : new CIEC_##datatypename;\
    }; \
    const static CTypeLib::CDataTypeEntry csm_oFirmwareDataTypeEntry_##datatypename; \
    virtual CIEC_ANY* clone(TForteByte *pa_acDataBuf) const { \
      FORTE_STATIC_ASSERT((sizeof(CIEC_ANY) == sizeof(CIEC_##datatypename)), DataTypeNotTheSameSizeAsANY); \
      return (0 != pa_acDataBuf)  ? new(pa_acDataBuf)CIEC_##datatypename(*this) : new CIEC_##datatypename(*this); } \
    virtual CStringDictionary::TStringId getTypeNameID() const { \
        return CIEC_##datatypename::csm_oFirmwareDataTypeEntry_##datatypename.getTypeNameId(); \
    }\
  FORTE_DUMMY_INIT_DEC \
  private:

//!\ingroup CORE This define is used to create the implementation for the above definition.
#define DEFINE_FIRMWARE_DATATYPE(datatypename, datatypenameid)\
  extern const CStringDictionary::TStringId g_nStringId##datatypename; \
  const CTypeLib::CDataTypeEntry CIEC_##datatypename::csm_oFirmwareDataTypeEntry_##datatypename((datatypenameid), CIEC_##datatypename::createDataType);\
  FORTE_DUMMY_INIT_DEF(CIEC_##datatypename)

struct SFBInterfaceSpec;

/*!\ingroup CORE \brief Class for storing the functionblock libraries.
 */
class CTypeLib{

  public:
//! The base class for all type entries in the type lib.
  class CTypeEntry{
    private:
      CStringDictionary::TStringId m_nTypeNameId;

    public:
      CTypeEntry *m_poNext; //!< a pointer to the next element in the list. Will be used to build single linked list of type entries.

      explicit CTypeEntry(CStringDictionary::TStringId pa_nTypeNameId);
      virtual ~CTypeEntry(void);

      CStringDictionary::TStringId getTypeNameId(void) const { return m_nTypeNameId; };
  };

  class CSpecTypeEntry : public CTypeEntry {
    private:
      const SFBInterfaceSpec* mSocketInterfaceSpec;
    public:
      CSpecTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, const SFBInterfaceSpec* paSocketInterfaceSpec);
      virtual ~CSpecTypeEntry(void);
      const SFBInterfaceSpec* getInterfaceSpec() const{ return mSocketInterfaceSpec; }
  };

//! The base class for all function block types entries in the type lib.
  class CFBTypeEntry : public CSpecTypeEntry{
    public:
      CFBTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, TFunctionBlockCreateFunc pa_pfuncCreateFB, const SFBInterfaceSpec* paSocketInterfaceSpec);
      virtual ~CFBTypeEntry(void);
      virtual CFunctionBlock *createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes){
              return m_pfuncFBCreationFunc( pa_nInstanceNameId, pa_poSrcRes);
            }
    private:
       TFunctionBlockCreateFunc m_pfuncFBCreationFunc;
  };

/*!\brief Class for adapter type entries in the type lib.
 */
      class CAdapterTypeEntry : public CSpecTypeEntry{
        public:
          CAdapterTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, TAdapterCreateFunc pa_pfuncCreateAdapter, const SFBInterfaceSpec* paSocketInterfaceSpec);
          virtual ~CAdapterTypeEntry(void);
          virtual CAdapter *createAdapterInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug){
            return m_pfuncAdapterCreationFunc( pa_nInstanceNameId, pa_poSrcRes, pa_bIsPlug);
          }
        private:
          TAdapterCreateFunc m_pfuncAdapterCreationFunc;
      };

//! The base class for all data type entries in the type lib.
  class CDataTypeEntry : public CTypeEntry{
    public:
      CDataTypeEntry(CStringDictionary::TStringId pa_nTypeNameId, TDataTypeCreateFunc pa_pfuncDTCreateFunc);
      virtual ~CDataTypeEntry(void);
      virtual CIEC_ANY *createDataTypeInstance(TForteByte *pa_acDataBuf){
        return m_pfuncDTCreateFunc(pa_acDataBuf);
      };
    protected:
       TDataTypeCreateFunc m_pfuncDTCreateFunc;

  };

public:
/*!\brief Create a new FB instance of given type and given instance name.
 *
 * \param pa_nInstanceNameId  StringId of instance name as this information can be stored within the resource
 * \param pa_nFBTypeId Type of the FB to create.
 * \param pa_poRes   Resource the FB is contained in.
 * \return On success a pointer to the new FB is returned, else the return value is 0 use getLastError for getting more information on the problem.
 *   possible error codes are:
 *    - e_UNSUPPORTED_TYPE   The requested FB type is not known to the typelib
 *    - e_INVALID_OPERATION The requested FB can not be created (e.g. out of memory)
 */
  static CFunctionBlock *createFB(CStringDictionary::TStringId pa_nInstanceNameId, CStringDictionary::TStringId pa_nFBTypeId, CResource *pa_poRes);

/*\brief Delete the given FB
 */
  static bool deleteFB(CFunctionBlock *pa_poFBToDelete);

  static CAdapter *createAdapter(CStringDictionary::TStringId pa_nInstanceNameId, CStringDictionary::TStringId pa_nFBTypeId, CResource *pa_poRes, bool pa_bIsPlug);

  /*!\brief Create an instance of an data type.
   *
   * @param pa_nDTNameId string id of the datatype to create
   * @param pa_acDataBuf buffer that the datatype should use. Has to be at least the size of CIEC_ANY
   * @return pointer to the create data type.
   */
  static CIEC_ANY *createDataTypeInstance(CStringDictionary::TStringId pa_nDTNameId, TForteByte *pa_acDataBuf);


/*!\brief Return a description of the last error that occured within the CTypeLib.
 *
 * e.g. Out of memory
 * \return Reference to the error string.
 */
  static EMGMResponse getLastError(void) { return m_eLastErrorMSG; };

/*!\brief add a Firmware FB type to the type lib (is mainly used by the corresponding entry class).
 */
  static void addFBType(CFBTypeEntry *pa_poFBTypeEntry);

  /*!\brief add a Firmware Adapter type to the type lib (is mainly used by the corresponding entry class).
   */
  static void addAdapterType(CAdapterTypeEntry *pa_poAdapterTypeEntry);

/*!\brief add a Firmware data type to the type lib (is mainly used by the corresponding entry class).
 */
  static void addDataType(CDataTypeEntry *pa_poDTEntry);
/*!\brief Get a pointer to the begin of the FB library list
 */
  static CTypeEntry *getFBLibStart() { return m_poFBLibStart; }

  /*!\brief Get a pointer to the begin of the FB library list
   */
    static CTypeEntry *getAdapterLibStart() { return m_poAdapterLibStart; }

  /*!\brief Get a pointer to the begin of the datatype library list
 */
  static CTypeEntry *getDTLibStart() { return m_poDTLibStart; }

  static CTypeEntry *findType(CStringDictionary::TStringId pa_nTypeId, CTypeEntry *pa_poListStart);

protected:
private:

/*!\brief Buffer for the last error that occurred.
 */
  static EMGMResponse m_eLastErrorMSG;

  static CFBTypeEntry *m_poFBLibStart, //!< pointer to the begin of the firmware fb library list
               *m_poFBLibEnd; //!<pointer to the end of the firmware fb library list

  static CAdapterTypeEntry *m_poAdapterLibStart, //!< pointer to the begin of the firmware adapter library list
               *m_poAdapterLibEnd; //!<pointer to the end of the firmware adapter library list

  static CDataTypeEntry *m_poDTLibStart, //!< pointer to the begin of the data type library
                 *m_poDTLibEnd; //!< pointer to the end of the data type library

  //! find the position of the first underscore that marks the end of the type name and the beginning of the generic part
  static const char* getFirstNonTypeNameUnderscorePos(const char* pa_acTypeName);
};

#endif /*TYPELIB_H_*/
