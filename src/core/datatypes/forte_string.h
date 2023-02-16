/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH
 *               2018 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 *               2022 Martin Erich Jobst
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
 *    Martin Melik Merkumians - make const char* constructor explicit, adds copy
 *      constructor, removed built-in type operator=
 *    Martin Jobst
 *      - add compare operators
 *******************************************************************************/
#ifndef _FORTE_STRING_H_
#define _FORTE_STRING_H_

#include "forte_any_string.h"
#include "forte_any_int.h"
#include "forte_char.h"

#include "devlog.h"

#include <string.h>
#include <stdlib.h>

/*!\ingroup COREDTS CIEC_STRING represents the string data type according to IEC 61131.
 */
class CIEC_STRING : public CIEC_ANY_STRING{
  DECLARE_FIRMWARE_DATATYPE(STRING)

  public:
    using value_type = CIEC_CHAR;
    using reference = value_type &;
    using const_reference = const value_type &;

    CIEC_STRING() = default;

    CIEC_STRING(const CIEC_STRING& paValue) = default;

    CIEC_STRING(const CIEC_CHAR &paValue) {
      const TForteChar symbol = static_cast<TForteChar>(paValue);
      assign(reinterpret_cast<const char*>(&symbol), 1);
    }

    explicit CIEC_STRING(const char* paValue){
      fromCharString(paValue);
    }

    ~CIEC_STRING() override = default;

    CIEC_STRING &operator =(const CIEC_STRING& paValue) = default;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_STRING;
    }

    class PARTIAL_ACCESS_TYPE : public CIEC_CHAR {
      using value_type = CIEC_CHAR::TValueType;
      using pointer_type = CIEC_CHAR::TValueType*;

      public:
        PARTIAL_ACCESS_TYPE(char *paValue) {
          value = paValue;
          if (value != nullptr) {
            setChar(*paValue);
          } else {
            setChar('\0');
          }
        }

        operator value_type() const {
          return getChar8();
        }

        PARTIAL_ACCESS_TYPE& operator=(const CIEC_CHAR& paValue) {
          if (value != nullptr) {
            *value = static_cast<value_type>(paValue);
          }
            return *this;
        }

      private:
        char *value;
    };

    [[nodiscard]] PARTIAL_ACCESS_TYPE at(const intmax_t paIndex) {
      if(paIndex < 1) {
        DEVLOG_ERROR("String index start at 1!\n");
        return PARTIAL_ACCESS_TYPE(nullptr);
      }
      if(paIndex > length()) {
        DEVLOG_ERROR("String index %d outside of length!\n", paIndex);
        return PARTIAL_ACCESS_TYPE(nullptr);
      }
      return PARTIAL_ACCESS_TYPE(getValue() + paIndex - 1);
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
      return CIEC_CHAR(*(getValue() + paIndex - 1));
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
     *   This command implements a conversion function from IEC61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_pacValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *pa_pacValue) override;

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

    /*! \brief Converts a UTF-8 encoded string to a STRING (ISO 10646 Row 00 internally)
     *
     *   This command implements a conversion function from a UTF-8
     *   encoded string (found e.g. in XML to the internal
     *   ISO 10646 Row 00 encoding.
     *   \param pa_pacValue  Reference to the given UTF-8 encoded byte array
     *   \param pa_nLen  Length to read (-1 for null-terminated)
     *   \param pa_bUnescape  Handle $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used from srcString
     *       -1 on error
     */
#ifdef FORTE_UNICODE_SUPPORT
    int fromUTF8(const char *pa_pacValue, int pa_nLen, bool pa_bUnescape) override;
#endif
    /*! \brief Converts the STRING to a UTF-8 representation
     *
     *   This command implements a conversion function from a STRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param pa_pacBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param pa_nBufferSize  Size of the provided buffer.
     *   \param pa_bEscape  Produce $-escapes and delimiter characters at the beginning and end
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
     * \return Needed buffer size for literal string without type delarator e.g., STRING#
     */
    size_t getToStringBufferSize() const override;

  protected:
    void setValue(const CIEC_ANY &pa_roValue) override {
      if(pa_roValue.getDataTypeID() == CIEC_ANY::e_STRING){
        const CIEC_STRING &roSrc(static_cast<const CIEC_STRING &>(pa_roValue));
        this->assign(roSrc.getValue(), roSrc.length());
      }
    }
  private:
};

inline
bool operator ==(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return (0 == strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator !=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return !(paLeft == paRight);
}

inline
bool operator >(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return (0 < strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator <(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return (0 > strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator >=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return (0 <= strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator <=(const CIEC_STRING &paLeft, const CIEC_STRING &paRight){
  return (0 >= strcmp(paLeft.getValue(), paRight.getValue()));
}

#endif /*_FORTE_STRING_H_*/
