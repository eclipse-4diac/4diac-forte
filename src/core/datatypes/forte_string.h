/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, nxtControl GmbH, TU Wien/ACIN
 *               Primetals Technologies Austria GmbH, Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Rene Smodic, Alois Zoitl, Gerhard Ebenhofer
 *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger, Stanislav Meduna,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixes behavior for getToStringBufferSize
 *      - make const char* constructor explicit, adds copy
 *      constructor, removed built-in type operator=
 *      - changes storage to std::string
 * 
 *    Martin Jobst
 *      - add compare operators
 *      - add equals function
 *******************************************************************************/
#ifndef _FORTE_STRING_H_
#define _FORTE_STRING_H_

#include "forte_any_string.h"
#include "forte_any_int.h"
#include "forte_char.h"
#include "../../arch/forte_fileio.h"

#include "devlog.h"

#include <string>
#include <stdlib.h>

/*!\ingroup COREDTS CIEC_STRING represents the string data type according to IEC 61131.
 */
class CIEC_STRING : public CIEC_ANY_STRING {
  DECLARE_FIRMWARE_DATATYPE(STRING)

  // CIEC_STRING to CIEC_STRING comparison operators
  friend bool operator==(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return paLeft.getStorage() == paRight.getStorage();
  }

  friend bool operator!=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return !(paLeft == paRight);
  }

  friend bool operator>(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return paLeft.getStorage() > paRight.getStorage();
  }

  friend bool operator<(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return paLeft.getStorage() < paRight.getStorage();
  }

  friend bool operator>=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return paLeft.getStorage() >= paRight.getStorage();
  }

  friend bool operator<=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight) {
    return paLeft.getStorage() <= paRight.getStorage();
  }

  // CIEC_STRING to std::string and vice versa comparison operators
  friend bool operator==(const std::string &paLeft, const CIEC_STRING &paRight) {
    return paLeft == paRight.getStorage();
  }

  friend bool operator!=(const std::string &paLeft, const CIEC_STRING &paRight) {
    return !(paLeft == paRight);
  }

  friend bool operator>(const std::string &paLeft, const CIEC_STRING &paRight) {
    return paLeft > paRight.getStorage();
  }

  friend bool operator<(const std::string &paLeft, const CIEC_STRING &paRight) {
    return paLeft < paRight.getStorage();
  }

  friend bool operator>=(const std::string &paLeft, const CIEC_STRING &paRight) {
    return paLeft >= paRight.getStorage();
  }

  friend bool operator<=(const std::string &paLeft, const CIEC_STRING &paRight) {
    return paLeft <= paRight.getStorage();
  }

  friend bool operator==(const CIEC_STRING &paLeft, const std::string &paRight) {
    return paLeft.getStorage() == paRight;
  }

  friend bool operator!=(const CIEC_STRING &paLeft, const std::string &paRight) {
    return !(paLeft == paRight);
  }

  friend bool operator>(const CIEC_STRING &paLeft, const std::string &paRight) {
    return paLeft.getStorage() > paRight;
  }

  friend bool operator<(const CIEC_STRING &paLeft, const std::string &paRight) {
    return paLeft.getStorage() < paRight;
  }

  friend bool operator>=(const CIEC_STRING &paLeft, const std::string &paRight) {
    return paLeft.getStorage() >= paRight;
  }

  friend bool operator<=(const CIEC_STRING &paLeft, const std::string &paRight) {
    return paLeft.getStorage() <= paRight;
  }

  public:
    using storage_type = std::string;
    using value_type = CIEC_CHAR;
    using reference = value_type &;
    using const_reference = const value_type &;
    
    using TValueType = storage_type;
    using TSymbolType = TForteChar;

    CIEC_STRING() {
    };

    CIEC_STRING(const CIEC_STRING& paValue) : CIEC_ANY_STRING(), mValue(paValue.getStorage()) {
    };

    CIEC_STRING(const CIEC_CHAR &paValue) : mValue(1, static_cast<char>(static_cast<CIEC_CHAR::TValueType>(paValue))) {
    }

    CIEC_STRING(CIEC_STRING &&paValue) : mValue(std::move(paValue.mValue)) {};

    [[deprecated("Use ctor with explicit length parameter")]]
    explicit CIEC_STRING(const char* paValue);

    explicit CIEC_STRING(const char* paValue, const size_t paLength);

    explicit CIEC_STRING(std::string paValue) : mValue(std::move(paValue)) {};

    ~CIEC_STRING() override = default;

    CIEC_STRING &operator =(const CIEC_STRING& paValue) {
      mValue = paValue.getStorage();
      return *this;
    };

    CIEC_STRING &operator=(CIEC_STRING &&paValue) {
      getStorageMutable() = std::move(paValue.getStorageMutable());
      return *this;
    }

    CIEC_STRING &operator =(const CIEC_CHAR& paValue) {
      mValue = storage_type(1, static_cast<char>(static_cast<CIEC_CHAR::TValueType>(paValue)));
      return *this;
    };

    TForteUInt16 length() const override {
      return static_cast<TForteUInt16>(mValue.length()); // max length 65534, cast to silence a warning
    }

    void clear() override {
      mValue.clear();
    }

    bool empty() const override {
      return mValue.empty();
    }
    
    const char* c_str() const {
      return getStorage().c_str();
    }

    explicit operator std::string() const {
      return getStorage();
    }

    virtual size_t getMaximumLength() const {
      return scmMaxStringLen;
    }

    TForteUInt16 getCapacity() const override {
      return static_cast<TForteUInt16>(mValue.capacity());
    }

    void reserve(const TForteUInt16 paRequestedSize) override;

    void assign(const char *paData, const TForteUInt16 paLen) override;

    /*! Append arbitrary data (can contain '0x00')
     */
    void append(const char *paData, const TForteUInt16 paLen) override;

    /*! Append data, cannot contain '0x00' as this is used to identify the end of the cstring
     */
    void append(const char *paData) override;

    virtual void append(const CIEC_STRING &paValue);

    virtual void append(const std::string &paValue);

    int compare(const CIEC_STRING& paValue) const;

    [[deprecated("Don't use this anymore, use c_str() instead")]]
    virtual char* getValue() override {
      DEVLOG_ERROR("Attempt to call getValue dummy function in CIEC_STRING\n");
      return nullptr;
    }

    [[deprecated("Don't use this anymore, use c_str() instead")]]
    virtual const char* getValue() const override {
      DEVLOG_ERROR("Attempt to call const getValue dummy function in CIEC_STRING\n");
      return nullptr;
    }

    EDataTypeID getDataTypeID() const override final {
      return CIEC_ANY::e_STRING;
    }

    class PARTIAL_ACCESS_TYPE : public CIEC_CHAR {
      using value_type = CIEC_CHAR::TValueType;
      using pointer_type = CIEC_CHAR::TValueType*;

      public:
        using CIEC_CHAR::operator=;

        PARTIAL_ACCESS_TYPE(CIEC_STRING &paOriginalString, const size_t paIndex) : mOriginalString(paOriginalString), mIndex(paIndex) {
          if (mOriginalString.length() >= paIndex && paIndex > 0) {
            setChar(static_cast<TForteChar>(mOriginalString.getStorage()[paIndex - 1]));
          } else {
            setChar('\0');
          }
        }

        virtual ~PARTIAL_ACCESS_TYPE() {
          if (mIndex > 0 && mIndex < mOriginalString.getMaximumLength()) {
            const value_type value = operator value_type();
            if (mOriginalString.length() < mIndex && value != '\0') {
              mOriginalString.getStorageMutable().resize(mIndex);
              mOriginalString.getStorageMutable()[mIndex - 1] = static_cast<char>(value);
            } else if (mOriginalString.getStorage()[mIndex - 1] != value) {
              mOriginalString.getStorageMutable()[mIndex - 1] = static_cast<char>(value);
            }
          }
        }

        PARTIAL_ACCESS_TYPE &operator=(const PARTIAL_ACCESS_TYPE &paValue) {
          CIEC_CHAR::operator=(paValue);
          return *this;
        }

        operator value_type() const {
          return getChar8();
        }

      private:
        CIEC_STRING &mOriginalString;
        const size_t mIndex;
    };

    [[nodiscard]] PARTIAL_ACCESS_TYPE at(const intmax_t paIndex) {
      if(paIndex < 1) {
        DEVLOG_ERROR("String index start at 1!\n");
        return PARTIAL_ACCESS_TYPE(*this, 0);
      }
      const size_t index = static_cast<size_t>(paIndex);
      if(index > getMaximumLength()) {
        DEVLOG_ERROR("String index %zu outside of maximum length!\n", index);
      }
      return PARTIAL_ACCESS_TYPE(*this, index);
    }

    [[nodiscard]] value_type at(intmax_t paIndex) const {
      if(paIndex < 1) {
        DEVLOG_ERROR("String index start at 1!\n");
        return CIEC_CHAR('\0');
      }
      if(paIndex > length()) {
        DEVLOG_ERROR("String index %d outside of length!\n", paIndex);
        return CIEC_CHAR('\0');
      }
      return CIEC_CHAR(static_cast<CIEC_CHAR::TValueType>(getStorage()[static_cast<size_t>(paIndex - 1)]));
    }

    [[nodiscard]] PARTIAL_ACCESS_TYPE operator[](intmax_t paIndex) {
      return at(paIndex);
    }

    [[nodiscard]] value_type operator[](intmax_t paIndex) const {
      return at(paIndex);
    }

    [[nodiscard]] PARTIAL_ACCESS_TYPE at(const CIEC_ANY_INT &paIndex) {
      intmax_t index = paIndex.getSignedValue();
      return at(index);
    }

    [[nodiscard]] value_type at(const CIEC_ANY_INT &paIndex) const {
      intmax_t index = paIndex.getSignedValue();
      return at(index);
    }

    [[nodiscard]] PARTIAL_ACCESS_TYPE operator[](const CIEC_ANY_INT &paIndex) {
      intmax_t index = paIndex.getSignedValue();
      return operator[](index);
    }

    [[nodiscard]] value_type operator[](const CIEC_ANY_INT &paIndex) const {
      intmax_t index = paIndex.getSignedValue();
      return operator[](index);
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC 61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   Note that the allowed \0 symbol is escaped as $00 in an IEC 61131 literal, therefore
     *   \0 itself will not be part of the literal itself
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *paValue) override;

    static int determineEscapedStringLength(const char *paValue, char paDelimiter, const size_t paLength);

    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue          Pointer to char-array for the result
     *   \param paBufferSize      Size of the buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    const storage_type& getStorage() const {
      return mValue;
    }

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      if(paOther.getDataTypeID() == CIEC_ANY::e_STRING) {
        return *this == static_cast<const CIEC_STRING &>(paOther);
      }
      return false;
    }

    /*! \brief Converts the STRING to a UTF-8 representation
     *
     *   This command implements a conversion function from a STRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param paBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param paBufferSize  Size of the provided buffer.
     *   \param paEscape  Produce $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
#ifdef FORTE_UNICODE_SUPPORT
    int toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const override;
#endif

    /*! \brief Returns the amount of bytes needed to create the IEC 61131 literal string
     *
     * IEC 61131 literal strings needs to delimit all non Common_Char_Value literals and special
     * symbols, like line-feed, into escaped symbol sequences. For single byte strings the special
     * symbols are represented as dollar escaped strings followed by two hex digits. For double byte strings
     * is is dollar followed by four hex digits, according to the IEC 61131 standard and its EBNF.
     *
     * \return Needed buffer size for literal string without type specifier e.g., STRING#
     */
    size_t getToStringBufferSize() const override;

    void setValue(const CIEC_ANY &paValue) override {
      if(paValue.getDataTypeID() == CIEC_ANY::e_STRING){
        *this = static_cast<const CIEC_STRING &>(paValue);
      } else if(paValue.getDataTypeID() == CIEC_ANY::e_CHAR){
        *this = static_cast<const CIEC_CHAR &>(paValue);
      }
    }

    protected:
      storage_type& getStorageMutable();

      storage_type mValue;
};

inline CIEC_STRING operator ""_STRING(const char* paValue, size_t paLength) {
  return CIEC_STRING(paValue, paLength);
}

#endif /*_FORTE_STRING_H_*/
