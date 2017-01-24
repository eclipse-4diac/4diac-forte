/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtcontrol GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
  *    Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_H_
#define _ANY_H_

#include <string.h>
#include "../typelib.h"

#if (!defined FORTE_LITTLE_ENDIAN) && (!defined FORTE_BIG_ENDIAN)
#  error "Endianess is not defined!"
#endif

/*!\ingroup COREDTS  CIEC_ANY represents the IEC_ANY data type according to IEC 61131.
 */

/*!\ingroup CORE \defgroup COREDTS Core Data Types
 * \brief Implementation of the core IEC 61131-3 data types.
 *
 */

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
    const static CTypeLib::CDataTypeEntry csm_oFirmwareDataTypeEntry_CIEC_ANY;

#ifdef FORTE_STATIC_LIB
    static int dummyInit();
#endif


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

    /*! \brief Get the pointer to the union char array
     *
     *  This command returns the pointer to the union char array
     *  \return Returns TForteByte*
     */

    TForteByte* getDataPtr(){
      return m_uAnyData.m_AData;
    }

    /*! \brief Get a const pointer to the union char array
     *
     *  \return Returns const TForteByte*
     */

    const TForteByte* getConstDataPtr() const{
      return m_uAnyData.m_AData;
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
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const;

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
    virtual int getToStringBufferSize();

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

    bool isForced(){
      return m_bForced;
    }

    void setForced(bool pa_bForced){
      m_bForced = pa_bForced;
    }

//    friend class CDataConnection;
//    friend class CInternal2InterfaceDataConnection;
//    friend class CIEC_ARRAY;

  protected:
    CIEC_ANY() :
        m_bForced(false){
      setLargestUInt(0);
    }

    /*! \brief copy the union data
     *
     * To be used for efficiently implementing assignment operators where it is
     * known that this can be done safely.
     */
    inline
    void setValueSimple(const CIEC_ANY &pa_roValue){
      m_uAnyData = pa_roValue.m_uAnyData;
    }

    /*! \brief Get Method for complex datatypes
     *  A virtual function for datatypes who can't be copied by the union assignment
     */

    void setTBOOL8(bool src){
      m_uAnyData.m_nLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT32(TForteUInt32 src){ //also used for TForteDWord
      m_uAnyData.m_nLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT16(TForteUInt16 src){ //also used for TForteWord
      m_uAnyData.m_nLargestUInt = TLargestUIntValueType(src);
    }

    void setTUINT8(TForteUInt8 src){ //also used for TForteByte
      m_uAnyData.m_nLargestUInt = TLargestUIntValueType(src);
    }

    void setTINT32(TForteInt32 src){
      m_uAnyData.m_nLargestInt = TLargestIntValueType(src);
    }

    void setTINT16(TForteInt16 src){
      m_uAnyData.m_nLargestInt = TLargestIntValueType(src);
    }

    void setTINT8(TForteInt8 src){
      m_uAnyData.m_nLargestInt = TLargestIntValueType(src);
    }

#ifdef FORTE_USE_REAL_DATATYPE
    void setTFLOAT(TForteFloat src){
      m_uAnyData.m_fFloat = TForteFloat(src);
    }
#ifdef FORTE_USE_64BIT_DATATYPES
    void setTDFLOAT(TForteDFloat src){
      m_uAnyData.m_fDFloat = TForteDFloat(src);
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES

    void setTUINT64(TForteUInt64 src){ //also used for LWORD
      m_uAnyData.m_nLargestUInt = TLargestUIntValueType(src);
    }

    void setTINT64(TForteInt64 src){
      m_uAnyData.m_nLargestInt = TLargestIntValueType(src);
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_BIG_ENDIAN
    bool getTBOOL8() const{
      return (m_uAnyData.m_nLargestUInt != 0);
    }
    TForteUInt32 getTUINT32() const{ //also used for TForteDWord
      return (TForteUInt32)m_uAnyData.m_nLargestUInt;
    }

    TForteUInt16 getTUINT16() const{ //also used for TForteWord
      return (TForteUInt16)m_uAnyData.m_nLargestUInt;
    }

    TForteUInt8 getTUINT8() const{ //also used for TForteByte
      return (TForteUInt8)m_uAnyData.m_nLargestUInt;
    }

    TForteInt32 getTINT32() const{
      return (TForteInt32)m_uAnyData.m_nLargestInt;
    }

    TForteInt16 getTINT16() const{
      return (TForteInt16)m_uAnyData.m_nLargestInt;
    }

    TForteInt8 getTINT8() const{
      return (TForteInt8)m_uAnyData.m_nLargestInt;
    }

#ifdef FORTE_USE_64BIT_DATATYPES
    TForteUInt64 getTUINT64() const{ //also used for LWORD
      return (TForteUInt64)m_uAnyData.m_nLargestUInt;
    }
    TForteInt64 getTINT64() const{
      return (TForteInt64)m_uAnyData.m_nLargestInt;
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#else
#ifdef FORTE_LITTLE_ENDIAN
    bool getTBOOL8() const{
      return m_uAnyData.m_bBool;
    }
    TForteUInt32 getTUINT32() const{ //also used for TForteDWord
      return m_uAnyData.m_nUInt32;
    }
    TForteUInt16 getTUINT16() const{ //also used for TForteWord
      return m_uAnyData.m_nUInt16;
    }
    TForteUInt8 getTUINT8() const{ //also used for TForteByte
      return m_uAnyData.m_nUInt8;
    }
    TForteInt32 getTINT32() const{
      return m_uAnyData.m_nInt32;
    }
    TForteInt16 getTINT16() const{
      return m_uAnyData.m_nInt16;
    }
    TForteInt8 getTINT8() const{
      return m_uAnyData.m_nInt8;
    }

#ifdef FORTE_USE_64BIT_DATATYPES
    TForteUInt64 getTUINT64() const{ //also used for LWORD
      return m_uAnyData.m_nUInt64;
    }
    TForteInt64 getTINT64() const{
      return m_uAnyData.m_nInt64;
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#else
#error Endianess not defined!
#endif //#ifdef FORTE_BIG_ENDIAN
#endif //#ifdef FORTE_LITTLE_ENDIAN
#ifdef FORTE_USE_REAL_DATATYPE //!< get-Methods are Big/Little Endian independent
    TForteFloat getTFLOAT() const{
      return (TForteFloat) m_uAnyData.m_fFloat;
    }
#ifdef FORTE_USE_64BIT_DATATYPES
    TForteDFloat getTDFLOAT() const{
      return TForteDFloat(m_uAnyData.m_fDFloat);
    }
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE
    TLargestUIntValueType getLargestUInt() const{
      return m_uAnyData.m_nLargestUInt;
    }

    void setLargestUInt(TLargestUIntValueType m_nVal){
      m_uAnyData.m_nLargestUInt = m_nVal;
    }

    TLargestIntValueType getLargestInt() const{
      return m_uAnyData.m_nLargestInt;
    }

    void setLargestInt(TLargestIntValueType m_nVal){
      m_uAnyData.m_nLargestInt = m_nVal;
    }

    TForteByte *getGenData(){
      return m_uAnyData.m_pGenData;
    }

    const TForteByte *getGenData() const{
      return m_uAnyData.m_pGenData;
    }

    void setGenData(TForteByte *pa_pGenData){
      m_uAnyData.m_pGenData = pa_pGenData;
    }

    static CStringDictionary::TStringId parseTypeName(const char *pa_pacValue, const char *pa_pacHashPos);

  private:
    const static int scm_nMaxTypeNameLength = 14;
    static const char * const scm_acAnyToStringResponse;

    //!declared but undefined copy constructor as we don't want ANYs to be directly copied.
    CIEC_ANY(const CIEC_ANY&);

    //!declared but undefined copy constructor as we don't want ANYs to be directly assigned. Can result in problems for more complicated data types (e.g., string)
    CIEC_ANY& operator =(const CIEC_ANY& pa_roValue);

    bool m_bForced;

    //Anonymous union holding the data value of our IEC data type
    union UAnyData{
        bool m_bBool;

        TForteByte m_cByte;
        TForteWord m_cWord;
        TForteDWord m_cDWord;

        TForteInt8 m_nInt8;
        TForteInt16 m_nInt16;
        TForteInt32 m_nInt32;

        TForteUInt8 m_nUInt8;
        TForteUInt16 m_nUInt16;
        TForteUInt32 m_nUInt32;

#ifdef FORTE_USE_REAL_DATATYPE
        TForteFloat m_fFloat;
#ifdef FORTE_USE_64BIT_DATATYPES
        TForteDFloat m_fDFloat;
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
        TForteInt64 m_nInt64;
        TForteUInt64 m_nUInt64;
        TForteByte m_AData[sizeof(TForteUInt64)]; //!< For data extraction in big endian machines
#else
        TForteByte m_AData[sizeof(TForteUInt32)]; //!< For data extraction in big endian machines
#endif //#ifdef FORTE_USE_64BIT_DATATYPES
        TLargestUIntValueType m_nLargestUInt;
        TLargestIntValueType m_nLargestInt;
        /*! \brief A pointer to general data that can be used for data types needing other data than that contained in the union
         *
         * This is needed as the current design does not allow that the size of data types when created is different from
         * the size of the CIEC_ANY class. This data value will be used for example by string or array.
         */
        TForteByte *m_pGenData;
    };

    UAnyData m_uAnyData;

};

/*!\brief Type for handling CIEC_ANY pointers
 */
typedef CIEC_ANY* TIEC_ANYPtr;
typedef const CIEC_ANY* TConstIEC_ANYPtr;

typedef CIEC_ANY IIEC_ANY; //TODO: for legacy support, will be deleted with next major release

#endif /*_MANY_H_*/
