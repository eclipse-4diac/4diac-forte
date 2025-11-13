/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, nxtcontrol GmbH, fortiss GmbH
 *                          TU Vienna/ACIN, Martin Erich Jobst,
 *                          Martin Melik Merkumians,
 *                          Primetals Technologies Austria
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna, Monika Wenger
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - templated cast factory function
 *   Martin Jobst - add equals function
 *                - add support for data types with different size
 *   Martin Melik Merkumians - Add specialized numeric_limits for IEC types
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include <bit>
#include <span>
#include <limits>
#include <type_traits>
#include "forte/typelib.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/config/forte_config.h"

/*!\ingroup COREDTS  CIEC_ANY represents the IEC_ANY data type according to IEC 61131.
 */

/*!\ingroup CORE \defgroup COREDTS Core Data Types
 * \brief Implementation of the core IEC 61131-3 data types.
 *
 */

namespace forte {
  class CIEC_ANY_REAL;
  class CIEC_ANY_BIT;

  class CIEC_ANY {
    public:
      /*! \ingroup COREDTS\brief Data type ID's are chosen according to IEC 61499-1 : Function Block
       *  -- Part 1 Architecture - Annex F (informative) Information exchange/F.3
       *  Transfer syntaxes (page 89).
       */
      enum EDataTypeID {
        e_ANY,
        e_BOOL,
        e_SINT,
        e_INT,
        e_DINT,
        e_LINT,
        e_USINT,
        e_UINT,
        e_UDINT,
        e_ULINT,
        e_BYTE,
        e_WORD,
        e_DWORD,
        e_LWORD,
        e_DATE,
        e_TIME_OF_DAY,
        e_DATE_AND_TIME,
        e_TIME,
        e_CHAR,
        e_WCHAR,
        e_LDATE,
        e_LTIME_OF_DAY,
        e_LDATE_AND_TIME,
        e_LTIME, // until here mem-copiable data types
        e_REAL,
        e_LREAL,
        e_STRING,
        e_WSTRING,
        e_DerivedData,
        e_DirectlyDerivedData,
        e_EnumeratedData,
        e_SubrangeData,
        e_ARRAY, // according to the compliance profile
        e_STRUCT,
        e_External = 256, // Base for CIEC_ANY based types outside of the forte base
        e_Max = 65535 // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
      };

      typedef TForteUInt64 TLargestUIntValueType;
      typedef TForteInt64 TLargestIntValueType;

      template<typename U, typename T>
      static auto cast(const T paFromCast) -> typename mpl::implicit_or_explicit_cast_t<T, U> {
        U oToCast;
        // If interacting with integers, add or remove sign extension
        if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T> && std::is_base_of_v<CIEC_ANY_INT, U>) {
          oToCast.setValueSimple(
              U(static_cast<typename U::TValueType>(static_cast<typename T::TValueType>(paFromCast))));
        } else if constexpr (std::is_base_of_v<CIEC_ANY_INT, T> && std::is_base_of_v<CIEC_ANY_BIT, U>) {
          typename T::TValueType fromValue = static_cast<typename T::TValueType>(paFromCast);
          typename std::make_unsigned_t<typename T::TValueType> fromValueUnsigned =
              static_cast<std::make_unsigned_t<typename T::TValueType>>(fromValue);
          typename U::TValueType toValue = static_cast<typename U::TValueType>(fromValueUnsigned);
          oToCast.setValueSimple(U(toValue));
        } else if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T> && // special cast binary to bool
                             std::is_base_of_v<CIEC_ANY_BIT, U>) {
          if constexpr (std::is_base_of_v<CIEC_BOOL, U>) { // reinterpret C/C++ bool to binary transfer
            oToCast.setValueSimple(U(static_cast<typename T::TValueType>(paFromCast) % 2 == 1 ? true : false));
          } else {
            oToCast.setValueSimple(
                U(static_cast<typename U::TValueType>(static_cast<typename T::TValueType>(paFromCast))));
          }
        } else if constexpr (std::is_base_of_v<CIEC_ANY_REAL, T>) {
          specialCast(paFromCast, oToCast);
        } else if constexpr (std::is_base_of_v<CIEC_ANY_REAL, U>) {
          oToCast.setValue(paFromCast);
        } else {
          oToCast.setValueSimple(paFromCast);
        }
        return oToCast;
      }

      CIEC_ANY() {
        setLargestUInt(0);
      }

      virtual ~CIEC_ANY() = default;

      /*! \brief Set method for data type member value
       *
       *  The data type value is set through the copy assignment
       *
       */
      virtual void setValue(const CIEC_ANY &paValue) {
        setValueSimple(paValue);
      }

      /*! \brief Reset the value for to the default initial value
       */
      virtual void reset() = 0;

      /**
       * @brief Unwrap ANY value if inside a container
       * @return The unwrapped value or this value if not in a container
       */
      [[nodiscard]] virtual CIEC_ANY &unwrap() {
        return *this;
      }

      [[nodiscard]] virtual const CIEC_ANY &unwrap() const {
        return *this;
      }

      /*! \brief Get the allocation size of the data type object
       *
       * @return The allocation size in bytes
       */
      virtual size_t getSizeof() const {
        return sizeof(CIEC_ANY);
      }

      /*! \brief Makes a clone of the data type object
       *
       *   With this command a clone object of the actual data type object is created.
       *   This clone object is necessary for establishing data-connections.
       *   Pure virtual function implementation.
       */
      virtual CIEC_ANY *clone(TForteByte *paDataBuf) const = 0;

      /*! \brief Get data type id method
       *
       *   With this command the data type of the actual object can be identified.
       *   The data type identifier is unique for each data type within FORTE.
       */
      virtual EDataTypeID getDataTypeID() const {
        return e_ANY;
      }

      virtual StringId getTypeNameID() const;

      /*! \brief Get the pointer to the union char array
       *
       *  This command returns the pointer to the union char array
       *  \return Returns TForteByte*
       */

      TForteByte *getDataPtr() {
        return mAnyData.mData;
      }

      /*! \brief Get a const pointer to the union char array
       *
       *  \return Returns const TForteByte*
       */

      const TForteByte *getConstDataPtr() const {
        return mAnyData.mData;
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
       *   data type (string format) to a C++ conform type.
       *   This function is necessary for communication with a proper engineering system.
       *   Pure virtual function implementation.
       *   \param paValue string buffer
       *   \return number of bytes taken used from the buffer
       *        -1 on on error
       */
      virtual int fromString(const char *paValue) = 0;

      /*! \brief Converts data type value to string
       *
       *   This command implements a conversion function from C++ data type
       *   to IEC61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   Pure virtual function implementation.
       *   \param paValue buffer for storing the string representation
       */
      virtual void toString(std::string &paTargetBuf) const = 0;

      /*! \brief Compare for equality
       *
       * \param paOther The other value
       * \return whether the values are equal
       * \note This method does not perform implicit or explicit type promotion when comparing for equality
       *       (e.g., <code>CIEC_INT(0).equals(CIEC_SINT(0))</code> yields <code>false</code>).
       */
      [[nodiscard]] virtual bool equals(const CIEC_ANY &paOther) const {
        if (getDataTypeID() == paOther.getDataTypeID()) {
          return mAnyData.mLargestUInt == paOther.mAnyData.mLargestUInt;
        }
        return false;
      }

      /*! \brief determine whether we can cast the source to the destination and what kind of cast it is
       */
      static bool isCastable(EDataTypeID paSource, EDataTypeID paDestination, bool &paUpCast, bool &paDownCast);
      static bool isCastable(EDataTypeID paSource, EDataTypeID paDestination) {
        bool bUpCast, bDownCast;
        return isCastable(paSource, paDestination, bUpCast, bDownCast);
      }

      /*! \brief perform special cast operation that can not be directly handled by the data types
       */
      static void specialCast(const CIEC_ANY &paSrcValue, CIEC_ANY &paDstValue);

      virtual size_t getIECMemorySize() const {
        const EDataTypeID typeId = getDataTypeID();
        return typeId > (sizeof(csmDataLengthLookup) / sizeof(size_t)) ? 0 : csmDataLengthLookup[typeId];
      }

      virtual CIEC_ANY *getVar(const std::span<const StringId> paNameList) {
        if (paNameList.empty()) {
          return this;
        }
        return nullptr;
      }

#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
      /*! \brief the following methods have to be implemented if a custom datatype is added to the forte which is not
       * supported by the default seralize mechanism. */
      /*! \brief returns the required size for serialization */
      virtual unsigned int getRequiredSerializationSize() const {
        return 0;
      }

      /*! \brief returns the tag of the datatype for serialization */
      virtual TForteByte getTag() const {
        return 0xFF;
      }

      /*! \brief serialize the custom type */
      virtual int serializeCustomType(TForteByte *, int) const {
        return -1;
      }

      /*! \brief deserialize the tag */
      virtual bool deserializeTag(const TForteByte) {
        return false;
      }

      /*! \brief deserialize the custom type*/
      virtual int deserializeCustomType(const TForteByte *, int) {
        return -1;
      }

#endif

    protected:
      /*! \brief copy the union data
       *
       * To be used for efficiently implementing assignment operators where it is
       * known that this can be done safely.
       */
      inline void setValueSimple(const CIEC_ANY &paValue) {
        mAnyData = paValue.mAnyData;
      }

      /*! \brief Get Method for complex datatypes
       *  A virtual function for datatypes who can't be copied by the union assignment
       */

      void setTBOOL8(bool src) {
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTUINT32(TForteUInt32 src) { // also used for TForteDWord
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTUINT16(TForteUInt16 src) { // also used for TForteWord
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTUINT8(TForteUInt8 src) { // also used for TForteByte
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTINT32(TForteInt32 src) {
        mAnyData.mLargestInt = TLargestIntValueType(src);
      }

      void setTINT16(TForteInt16 src) {
        mAnyData.mLargestInt = TLargestIntValueType(src);
      }

      void setTINT8(TForteInt8 src) {
        mAnyData.mLargestInt = TLargestIntValueType(src);
      }

      void setChar(TForteChar src) {
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setChar16(TForteWChar src) {
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTFLOAT(TForteFloat src) {
        mAnyData.mFloat = TForteFloat(src);
      }

      void setTDFLOAT(TForteDFloat src) {
        mAnyData.mDFloat = TForteDFloat(src);
      }

      void setTUINT64(TForteUInt64 src) { // also used for LWORD
        mAnyData.mLargestUInt = TLargestUIntValueType(src);
      }

      void setTINT64(TForteInt64 src) {
        mAnyData.mLargestInt = TLargestIntValueType(src);
      }

      bool getTBOOL8() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mBool;
        } else {
          return (mAnyData.mLargestUInt != 0);
        }
      }

      TForteUInt32 getTUINT32() const { // also used for TForteDWord
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mUInt32;
        } else {
          return static_cast<TForteUInt32>(mAnyData.mLargestUInt);
        }
      }

      TForteUInt16 getTUINT16() const { // also used for TForteWord
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mUInt16;
        } else {
          return static_cast<TForteUInt16>(mAnyData.mLargestUInt);
        }
      }

      TForteUInt8 getTUINT8() const { // also used for TForteByte
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mUInt8;
        } else {
          return static_cast<TForteUInt8>(mAnyData.mLargestUInt);
        }
      }

      TForteInt32 getTINT32() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mInt32;
        } else {
          return static_cast<TForteInt32>(mAnyData.mLargestInt);
        }
      }

      TForteInt16 getTINT16() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mInt16;
        } else {
          return static_cast<TForteInt16>(mAnyData.mLargestInt);
        }
      }

      TForteInt8 getTINT8() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mInt8;
        } else {
          return static_cast<TForteInt8>(mAnyData.mLargestInt);
        }
      }

      TForteChar getChar8() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mChar8;
        } else {
          return static_cast<TForteChar>(mAnyData.mLargestInt);
        }
      }

      TForteWChar getChar16() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mWChar16;
        } else {
          return static_cast<TForteWChar>(mAnyData.mLargestInt);
        }
      }

      TForteUInt64 getTUINT64() const { // also used for LWORD
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mUInt64;
        } else {
          return static_cast<TForteUInt64>(mAnyData.mLargestUInt);
        }
      }

      TForteInt64 getTINT64() const {
        if constexpr (std::endian::native == std::endian::little) {
          return mAnyData.mInt64;
        } else {
          return static_cast<TForteInt64>(mAnyData.mLargestInt);
        }
      }

      //!< get-Methods are Big/Little Endian independent
      TForteFloat getTFLOAT() const {
        return (TForteFloat) mAnyData.mFloat;
      }

      TForteDFloat getTDFLOAT() const {
        return TForteDFloat(mAnyData.mDFloat);
      }

      TLargestUIntValueType getLargestUInt() const {
        return mAnyData.mLargestUInt;
      }

      void setLargestUInt(TLargestUIntValueType paVal) {
        mAnyData.mLargestUInt = paVal;
      }

      TLargestIntValueType getLargestInt() const {
        return mAnyData.mLargestInt;
      }

      void setLargestInt(TLargestIntValueType paVal) {
        mAnyData.mLargestInt = paVal;
      }

      TForteByte *getGenData() {
        return mAnyData.mGenData;
      }

      const TForteByte *getGenData() const {
        return mAnyData.mGenData;
      }

      void setGenData(TForteByte *paGenData) {
        mAnyData.mGenData = paGenData;
      }

      static StringId parseTypeName(const char *paValue, const char *paHashPos);

    public:
      CIEC_ANY(const CIEC_ANY &) = delete;
      CIEC_ANY &operator=(const CIEC_ANY &paValue) = delete;

    private:
      // Anonymous union holding the data value of our IEC data type
      union UAnyData {
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

          TForteChar mChar8;
          TForteWChar mWChar16;

          TForteFloat mFloat;
          TForteDFloat mDFloat;

          TForteInt64 mInt64;
          TForteUInt64 mUInt64;
          TForteByte mData[sizeof(TForteUInt64)]; //!< For data extraction in big endian machines
          TLargestUIntValueType mLargestUInt;
          TLargestIntValueType mLargestInt;
          /*! \brief A pointer to general data that can be used for data types needing other data than that contained in
           * the union
           *
           * This is needed as the current design does not allow that the size of data types when created is different
           * from the size of the CIEC_ANY class. This data value will be used for example by string or array.
           */
          TForteByte *mGenData;
      };

      UAnyData mAnyData;

      constexpr static size_t csmDataLengthLookup[] = {0, 1, 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8, 8,
                                                       8, 8, 8, 8, 8, 8, 8, 1, 2, 4, 8, 0, 0, 0};
  };

  namespace templates {
    template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<CIEC_ANY, T>>>
    struct numeric_limits {
        static constexpr bool is_specialized = true;

        static T min() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::min());
        }

        static T max() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::max());
        }

        static T lowest() noexcept {
          return min();
        }

        static constexpr int digits = std::numeric_limits<typename T::TValueType>::digits;
        static constexpr int digits10 = std::numeric_limits<typename T::TValueType>::digits10;
        static constexpr int max_digits10 = std::numeric_limits<typename T::TValueType>::max_digits10;

        static constexpr bool is_signed = std::numeric_limits<typename T::TValueType>::is_signed;
        static constexpr bool is_integer = std::numeric_limits<typename T::TValueType>::is_integer;
        static constexpr bool is_exact = std::numeric_limits<typename T::TValueType>::is_exact;
        static constexpr int radix = std::numeric_limits<typename T::TValueType>::radix;

        static T epsilon() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::epsilon());
        }

        static T round_error() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::round_error());
        }

        static constexpr int min_exponent = std::numeric_limits<typename T::TValueType>::min_exponent;
        static constexpr int min_exponent10 = std::numeric_limits<typename T::TValueType>::min_exponent;
        static constexpr int max_exponent = std::numeric_limits<typename T::TValueType>::max_exponent;
        static constexpr int max_exponent10 = std::numeric_limits<typename T::TValueType>::max_exponent10;

        static constexpr bool has_infinity = std::numeric_limits<typename T::TValueType>::has_infinity;
        static constexpr bool has_quiet_NaN = std::numeric_limits<typename T::TValueType>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = std::numeric_limits<typename T::TValueType>::has_signaling_NaN;
        static constexpr std::float_denorm_style has_denorm = std::numeric_limits<typename T::TValueType>::has_denorm;
        static constexpr bool has_denorm_loss = std::numeric_limits<typename T::TValueType>::has_denorm_loss;

        static T infinity() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::infinity());
        }

        static T quiet_NaN() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::quiet_NaN());
        }

        static T signaling_NaN() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::signaling_NaN());
        }

        static T denorm_min() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::denorm_min());
        }

        static constexpr bool is_iec559 = std::numeric_limits<typename T::TValueType>::is_iec559;
        static constexpr bool is_bounded = std::numeric_limits<typename T::TValueType>::is_bounded;
        static constexpr bool is_modulo = std::numeric_limits<typename T::TValueType>::is_modulo;

        static constexpr bool traps = std::numeric_limits<typename T::TValueType>::traps;
        static constexpr bool tinyness_before = std::numeric_limits<typename T::TValueType>::tinyness_before;
        static constexpr std::float_round_style round_style = std::numeric_limits<typename T::TValueType>::round_style;
    };

  } // namespace templates

  template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<CIEC_ANY, T>>>
  struct CDataTypeTrait {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_Max;
      static constexpr StringId scmDataTypeName{};
  };

  /*!
   * \brief A type trait marking generic data types
   * \tparam T The data type
   */
  template<typename T>
  struct is_generic_datatype : std::false_type {};

  template<>
  struct is_generic_datatype<CIEC_ANY> : std::true_type {};

  /*!
   * \brief Helper variable template for is_generic_datatype
   * \tparam T The data type
   */
  template<typename T>
  inline constexpr bool is_generic_datatype_v = is_generic_datatype<T>::value;

  /*!
   * \brief A concept for generic data types
   * \tparam T The data type
   */
  template<typename T>
  concept generic_datatype = is_generic_datatype_v<std::remove_cvref_t<T>>;
} // namespace forte
