/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtcontrol GmbH, fortiss GmbH, 2018 TU Vienna/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - templated cast factory function
 *******************************************************************************/
#ifndef _ANY_H_
#define _ANY_H_

#include <string.h>
#include "../typelib.h"
#include "iec61131_cast_helper.h"

#if (!defined FORTE_LITTLE_ENDIAN) && (!defined FORTE_BIG_ENDIAN)
#  error "Endianess is not defined!"
#endif

/*!\ingroup COREDTS  CIEC_ANY represents the IEC_ANY data type according to IEC 61131.
 */

/*!\ingroup CORE \defgroup COREDTS Core Data Types
 * \brief Implementation of the core IEC 61131-3 data types.
 *
 */

class CIEC_ANY_REAL;
class CIEC_ANY_BIT;

class CIEC_ANY{
  public:
    /*! \ingroup COREDTS\brief Data type ID's are chosen according to IEC 61499-1 : Function Block
     *  -- Part 1 Architecture - Annex F (informative) Information exchange/F.3
     *  Transfer syntaxes (page 89).
     */
    enum EDataTypeID{
      e_ANY, e_BOOL, e_SINT, e_INT, e_DINT, e_LINT, e_USINT, e_UINT, e_UDINT, e_ULINT, e_BYTE, e_WORD, e_DWORD, e_LWORD, e_DATE, e_TIME_OF_DAY, e_DATE_AND_TIME, e_TIME, //until here simple Datatypes
      e_REAL,
      e_LREAL,
      e_STRING,
      e_WSTRING,
      e_DerivedData,
      e_DirectlyDerivedData,
      e_EnumeratedData,
      e_SubrangeData,
      e_ARRAY, //according to the compliance profile
      e_STRUCT,
      e_External = 256, // Base for CIEC_ANY based types outside of the forte base
      e_Max = 65535 // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
    };

    // ordering of sizes is according to EDataTypeID, that the ID can be used to get the proper value, +1 for \0
    const static TForteByte csmStringBufferSize[];

#ifdef FORTE_USE_64BIT_DATATYPES
    typedef TForteUInt64 TLargestUIntValueType;
    typedef TForteInt64 TLargestIntValueType;
#else
    typedef TForteUInt32 TLargestUIntValueType;
    typedef TForteInt32 TLargestIntValueType;
#endif

    /* the following functions have to be added by hand as they the default DECLARE_FIRMWARE_DATATYPE and DEFINE_FIRMWARE_DATATYPE
     * does not work here.
     */
    static CIEC_ANY *createDataType(TForteByte *pa_acDataBuf){
      return (0 != pa_acDataBuf) ? new (pa_acDataBuf) CIEC_ANY : new CIEC_ANY;
    }
    const static CTypeLib::CDataTypeEntry csmFirmwareDataTypeEntry_CIEC_ANY;

    static int dummyInit();

    template<typename U, typename T>
    static typename forte::core::mpl::implicit_or_explicit_cast<T, U>::type cast(const T paFromCast){
      U oToCast;
      if (forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value ||
          forte::core::mpl::is_base_of<CIEC_ANY_BIT, U>::value) {
        oToCast.setValueSimple(paFromCast);
      } else
      if(forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value){
        specialCast(paFromCast, oToCast);
      } else if(forte::core::mpl::is_base_of<CIEC_ANY_REAL, U>::value){
        oToCast.setValue(paFromCast);
      }
      else{
        oToCast.setValueSimple(paFromCast);
      }
      return oToCast;
    }

    CIEC_ANY() :
        mForced(false){
      setLargestUInt(0);
    }

    virtual ~CIEC_ANY(){
    }

    void saveAssign(const CIEC_ANY &pa_roValue);

    /*! \brief Set method for data type member value
     *
     *  The data type value is set through the copy assignment
     *
     */
    virtual void setValue(const CIEC_ANY &pa_roValue){
      setValueSimple(pa_roValue);
    }

    /*! \brief Makes a clone of the data type object
     *
     *   With this command a clone object of the actual data type object is created.
     *   This clone object is necessary for establishing data-connections.
     *   Pure virtual function implementation.
     */
    virtual CIEC_ANY* clone(TForteByte *pa_acDataBuf) const {
      return createDataType(pa_acDataBuf); //there is nothing to clone in any impl
    }

    /*! \brief Get data type id method
     *
     *   With this command the data type of the actual object can be identified.
     *   The data type identifier is unique for each data type within FORTE.
     */
    virtual EDataTypeID getDataTypeID() const {
      return CIEC_ANY::e_ANY;
    }

    virtual CStringDictionary::TStringId getTypeNameID() const {
      return csmFirmwareDataTypeEntry_CIEC_ANY.getTypeNameId();
    }

    /*! \brief Get the pointer to the union char array
     *
     *  This command returns the pointer to the union char array
     *  \return Returns TForteByte*
     */

    TForteByte* getDataPtr(){
      return mAnyData.mData;
    }

    /*! \brief Get a const pointer to the union char array
     *
     *  \return Returns const TForteByte*
     */

    const TForteByte* getConstDataPtr() const{
      return mAnyData.mData;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   Pure virtual function implementation.
     *   \param pa_pacValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *pa_pacValue);

    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   Pure virtual function implementation.
     *   \param pa_pacValue buffer for storing the string representation
     *   \param pa_nBufferSize size in bytes available in the buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    /*! \brief determine whether we can cast the source to the destination and what kind of cast it is
     */
    static bool isCastable(EDataTypeID pa_eSource, EDataTypeID pa_eDestination, bool &pa_rbUpCast, bool &pa_rbDownCast);
    static bool isCastable(EDataTypeID pa_eSource, EDataTypeID pa_eDestination){
      bool bUpCast, bDownCast;
      return isCastable(pa_eSource, pa_eDestination, bUpCast, bDownCast);
    }

    /*! \brief perform special cast operation that can not be directly handled by the data types
     */
    static void specialCast(const CIEC_ANY &pa_roSrcValue, CIEC_ANY &pa_roDstValue);

    /*! \brief calculates buffer size needed for toString conversion
         */
    virtual size_t getToStringBufferSize() const;

#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
    /*! \brief the following methods have to be implemented if a custom datatype is added to the forte which is not supported by the default seralize mechanism. */
    /*! \brief returns the required size for serialization */
    virtual unsigned int getRequiredSerializationSize() const{
      return 0;
    }
    ;
    /*! \brief returns the tag of the datatype for serialization */
    virtual TForteByte getTag() const{
      return 0xFF;
    }
    ;
    /*! \brief serialize the custom type */
    virtual int serializeCustomType(TForteByte*, int) const{
      return -1;
    }
    ;
    /*! \brief deserialize the tag */
    virtual bool deserializeTag(const TForteByte){
      return false;
    }
    ;
    /*! \brief deserialize the custom type*/
    virtual int deserializeCustomType(const TForteByte*, int){
      return -1;
    }
    ;
#endif

    bool isForced() const{
      return mForced;
    }

    void setForced(bool pa_bForced){
      mForced = pa_bForced;
    }

  protected:
    /*! \brief copy the union data
     *
     * To be used for efficiently implementing assignment operators where it is
     * known that this can be done safely.
     */
    inline
    void setValueSimple(const CIEC_ANY &pa_roValue){
      mAnyData = pa_roValue.mAnyData;
    }

    /*! \brief Get Method for complex datatypes
     *  A virtual function for datatypes who can't be copied by the union assignment
     */

    void setTBOOL8(bool src){
      mAnyData.mLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT32(TForteUInt32 src){ //also used for TForteDWord
      mAnyData.mLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT16(TForteUInt16 src){ //also used for TForteWord
      mAnyData.mLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT8(TForteUInt8 src){ //also used for TForteByte
      mAnyData.mLargestUInt = TLargestUIntValueType(src);
    }

    void setTINT32(TForteInt32 src){
      mAnyData.mLargestInt = TLargestIntValueType(src);
    }

    void setTINT16(TForteInt16 src){
      mAnyData.mLargestInt = TLargestIntValueType(src);
    }

    void setTINT8(TForteInt8 src){
      mAnyData.mLargestInt = TLargestIntValueType(src);
    }

#ifdef FORTE_USE_REAL_DATATYPE
    void setTFLOAT(TForteFloat src){
      mAnyData.mFloat = TForteFloat(src);
    }
#endif //#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_LREAL_DATATYPE
    void setTDFLOAT(TForteDFloat src){
      mAnyData.mDFloat = TForteDFloat(src);
    }
#endif //#ifdef FORTE_USE_LREAL_DATATYPE

#ifdef FORTE_USE_64BIT_DATATYPES

    void setTUINT64(TForteUInt64 src){ //also used for LWORD
      mAnyData.mLargestUInt = TLargestUIntValueType(src);
    }

    void setTINT64(TForteInt64 src){
      mAnyData.mLargestInt = TLargestIntValueType(src);
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_BIG_ENDIAN
    bool getTBOOL8() const{
      return (mAnyData.mLargestUInt != 0);
    }
    TForteUInt32 getTUINT32() const{ //also used for TForteDWord
      return (TForteUInt32)mAnyData.mLargestUInt;
    }

    TForteUInt16 getTUINT16() const{ //also used for TForteWord
      return (TForteUInt16)mAnyData.mLargestUInt;
    }

    TForteUInt8 getTUINT8() const{ //also used for TForteByte
      return (TForteUInt8)mAnyData.mLargestUInt;
    }

    TForteInt32 getTINT32() const{
      return (TForteInt32)mAnyData.mLargestInt;
    }

    TForteInt16 getTINT16() const{
      return (TForteInt16)mAnyData.mLargestInt;
    }

    TForteInt8 getTINT8() const{
      return (TForteInt8)mAnyData.mLargestInt;
    }

#ifdef FORTE_USE_64BIT_DATATYPES
    TForteUInt64 getTUINT64() const{ //also used for LWORD
      return (TForteUInt64)mAnyData.mLargestUInt;
    }
    TForteInt64 getTINT64() const{
      return (TForteInt64)mAnyData.mLargestInt;
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#else
#ifdef FORTE_LITTLE_ENDIAN
    bool getTBOOL8() const{
      return mAnyData.mBool;
    }
    TForteUInt32 getTUINT32() const{ //also used for TForteDWord
      return mAnyData.mUInt32;
    }
    TForteUInt16 getTUINT16() const{ //also used for TForteWord
      return mAnyData.mUInt16;
    }
    TForteUInt8 getTUINT8() const{ //also used for TForteByte
      return mAnyData.mUInt8;
    }
    TForteInt32 getTINT32() const{
      return mAnyData.mInt32;
    }
    TForteInt16 getTINT16() const{
      return mAnyData.mInt16;
    }
    TForteInt8 getTINT8() const{
      return mAnyData.mInt8;
    }

#ifdef FORTE_USE_64BIT_DATATYPES
    TForteUInt64 getTUINT64() const{ //also used for LWORD
      return mAnyData.mUInt64;
    }
    TForteInt64 getTINT64() const{
      return mAnyData.mInt64;
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#else
#error Endianess not defined!
#endif //#ifdef FORTE_BIG_ENDIAN
#endif //#ifdef FORTE_LITTLE_ENDIAN
#ifdef FORTE_USE_REAL_DATATYPE //!< get-Methods are Big/Little Endian independent
    TForteFloat getTFLOAT() const{
      return (TForteFloat) mAnyData.mFloat;
    }
#endif //#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_LREAL_DATATYPE
    TForteDFloat getTDFLOAT() const{
      return TForteDFloat(mAnyData.mDFloat);
    }
#endif //#ifdef FORTE_USE_LREAL_DATATYPE

    TLargestUIntValueType getLargestUInt() const{
      return mAnyData.mLargestUInt;
    }

    void setLargestUInt(TLargestUIntValueType paVal){
      mAnyData.mLargestUInt = paVal;
    }

    TLargestIntValueType getLargestInt() const{
      return mAnyData.mLargestInt;
    }

    void setLargestInt(TLargestIntValueType paVal){
      mAnyData.mLargestInt = paVal;
    }

    TForteByte *getGenData(){
      return mAnyData.mGenData;
    }

    const TForteByte *getGenData() const{
      return mAnyData.mGenData;
    }

    void setGenData(TForteByte *paGenData){
      mAnyData.mGenData = paGenData;
    }

    static CStringDictionary::TStringId parseTypeName(const char *pa_pacValue, const char *pa_pacHashPos);

  private:
    const static int scmMaxTypeNameLength = 14;
    static const char scmAnyToStringResponse[];

    //!declared but undefined copy constructor as we don't want ANYs to be directly copied.
    CIEC_ANY(const CIEC_ANY&);

    //!declared but undefined copy constructor as we don't want ANYs to be directly assigned. Can result in problems for more complicated data types (e.g., string)
    CIEC_ANY& operator =(const CIEC_ANY& pa_roValue);

    bool mForced;

    //Anonymous union holding the data value of our IEC data type
    union UAnyData{
        bool mBool;

        TForteByte mByte;
        TForteWord mWord;
        TForteDWord mDWord;

        TForteInt8 mInt8;
        TForteInt16 mInt16;
        TForteInt32 mInt32;

        TForteUInt8 mUInt8;
        TForteUInt16 mUInt16;
        TForteUInt32 mUInt32;

#ifdef FORTE_USE_REAL_DATATYPE
        TForteFloat mFloat;
#endif //#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_LREAL_DATATYPE
        TForteDFloat mDFloat;
#endif //#ifdef FORTE_USE_LREAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
        TForteInt64 mInt64;
        TForteUInt64 mUInt64;
        TForteByte mData[sizeof(TForteUInt64)]; //!< For data extraction in big endian machines
#else
        TForteByte mData[sizeof(TForteUInt32)]; //!< For data extraction in big endian machines
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
        TLargestUIntValueType mLargestUInt;
        TLargestIntValueType mLargestInt;
        /*! \brief A pointer to general data that can be used for data types needing other data than that contained in the union
         *
         * This is needed as the current design does not allow that the size of data types when created is different from
         * the size of the CIEC_ANY class. This data value will be used for example by string or array.
         */
        TForteByte *mGenData;
    };

    UAnyData mAnyData;

};

/*!\brief Type for handling CIEC_ANY pointers
 */
typedef CIEC_ANY* TIEC_ANYPtr;
typedef const CIEC_ANY* TConstIEC_ANYPtr;

typedef CIEC_ANY IIEC_ANY; //TODO: for legacy support, will be deleted with next major release

#endif /*_MANY_H_*/
