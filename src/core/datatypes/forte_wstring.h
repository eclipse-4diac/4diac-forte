/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, nxtControl GmbH, TU Wien/ACIN
 *                          Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny, Stanislav Meduna
 *    Martin Melik Merkumians, Matthias Plasch, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixed behavior for getToStringBufferSize, make const char*
 *        constructor explicit, removed built-in type operator=, added
 *        castable CIEC types operator=
 *    Martin Jobst
 *      - add compare operators
 *      - add equals function
 *      - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_WSTRING_H_
#define _FORTE_WSTRING_H_

#include "forte_any_string.h"
#include "forte_string.h"
#include "forte_wchar.h"

#include <codecvt>

#ifdef FORTE_USE_WSTRING_DATATYPE

/*!\ingroup COREDTS \brief CIEC_WSTRING represents the wide string data type according to IEC 61131.
 *
 *  "wstring" is implemented like the "string" - the following first implementation
 *  "typedef std::basic_string<wchar_t> TWSTRING; didn't work well"
 */
class CIEC_WSTRING final : public CIEC_ANY_STRING {
  DECLARE_FIRMWARE_DATATYPE(WSTRING)

  public:
    using TValueType = TForteWChar;
    CIEC_WSTRING() = default;

    CIEC_WSTRING(const CIEC_WSTRING& paValue) = default;

    CIEC_WSTRING(const CIEC_WCHAR &paValue) {
      TForteWChar value = static_cast<TForteWChar>(paValue);
      auto &converter = std::use_facet<std::codecvt<char16_t, char, std::mbstate_t>>(std::locale());
      std::mbstate_t mb{};
      char buf[converter.max_length()];
      const char16_t *from_next;
      char *to_next;
      converter.out(mb, &value, &value + 1, from_next, buf, buf + converter.max_length(), to_next);
      // error checking skipped for brevity
      std::size_t size = static_cast<size_t>(to_next - buf);
      assign(buf, static_cast<TForteUInt16>(size));
    }

    explicit CIEC_WSTRING(const char* paValue) {
      fromCharString(paValue);
    }

    CIEC_WSTRING(const char16_t *paValue, size_t paLength) {
      auto &converter = std::use_facet<std::codecvt<char16_t, char, std::mbstate_t>>(std::locale());
      std::mbstate_t mb{};
      char buf[paLength * converter.max_length()];
      const char16_t *from_next;
      char *to_next;
      converter.out(mb, paValue, paValue + paLength, from_next,
                    buf, buf + paLength * static_cast<size_t>(converter.max_length()), to_next);
      // error checking skipped for brevity
      std::size_t size = static_cast<size_t>(to_next - buf);
      assign(buf, static_cast<TForteUInt16>(size));
    }

    ~CIEC_WSTRING() override = default;

    CIEC_WSTRING &operator =(const CIEC_WSTRING &paValue) = default;
    
    CIEC_WSTRING &operator=(const CIEC_WCHAR &paValue) {
      *this = CIEC_WSTRING(paValue);
      return *this;
    }

    /*! \brief Converts a UTF-8 encoded string to a WSTRING (ISO 10646 Row 00 internally)
     *
     *   This command implements a conversion function from a UTF-8
     *   encoded string (found e.g. in XML to the internal
     *   ISO 10646 Row 00 encoding.
     *   \param paBuffer  Reference to the given UTF-8 encoded byte array
     *   \param paLen  Length to read (-1 for null-terminated)
     *   \param paUnescape  Handle $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used from srcString
     *       -1 on error
     */
    int fromUTF8(const char *paValue, int paLen, bool paUnescape);

    /*! \brief Converts the WSTRING to a UTF-8 representation
     *
     *   This command implements a conversion function from a WSTRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param paBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param paBufferSize  Size of the provided buffer.
     *   \param paEscape  Produce $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    int toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const override;

    /*! \brief Converts a UTF-16 encoded string to a WSTRING (UTF-8 internally)
     *
     *   This command implements a conversion function from a UTF-16
     *   encoded string (found e.g. in serialized WSTRING type) to the internal
     *   UTF-8 encoding.
     *   \param paBuffer  Reference to the given UTF-16 encoded byte array
     *   \param paBufferLen  Length of the provided byte array
     *   \return Can be the following response:
     *     - false....conversion was not successful - something went wrong!
     *     -  true....conversion was successful.
     */
    bool fromUTF16(const TForteByte *paBuffer, unsigned int paBufferLen);

    /*! \brief Converts a UTF-16 encoded string to a WSTRING (UTF-8 internally)
     *
     *   This command implements a conversion function from a UTF-16
     *   encoded string (found e.g. in serialized WSTRING type) to the internal
     *   UTF-8 encoding.
     *   \param paBuffer  Reference to the given UTF-16 encoded byte array
     *   \param paBufferLen  Length of the provided WChar array/data
     *   \return Can be the following response:
     *     - false....conversion was not successful - something went wrong!
     *     -  true....conversion was successful.
     */
    bool fromUTF16(const TForteWChar *paBuffer, unsigned int paBufferLen);

        /*! \brief Converts the WSTRING to a UTF-16 representation
         *
         *   This command implements a conversion function from a WSTRING
         *   to a big-endian UTF-16 encoding, usable e.g. for the serialization.
         *   \param paBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
         *   \param paBufferSize  Size of the provided buffer.
         *   \return number of bytes used in the buffer
         *           -1 on error
         */
        int toUTF16(TForteByte *paBuffer, unsigned int paBufferSize) const;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_WSTRING;
    }

    void setValue(const CIEC_ANY& paValue) override {
      if(paValue.getDataTypeID() == CIEC_ANY::e_WSTRING){
        const CIEC_WSTRING &roSrc(static_cast<const CIEC_WSTRING &>(paValue));
        this->assign(roSrc.getValue(), roSrc.length());
      }
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   It is expecting a UTF-8 string and will check whether its content is constrained
     *   to the basic multilingual plane.
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *paValue) override;

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

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      if(paOther.getDataTypeID() == CIEC_ANY::e_WSTRING) {
        return 0 == strcmp(getValue(), static_cast<const CIEC_WSTRING&>(paOther).getValue());
      }
      return false;
    }

    /*! \brief Returns the amount of bytes needed to create the IEC 61131 literal string
     *
     * IEC 61131 literal strings needs to delimit all non Common_Char_Value literals and special
     * symbols, like line-feed, into escaped symbol sequences. For single byte strings the special
     * symbols are represented as dollar escaped strings followed by two hex digits. For double byte strings
     * is is dollar followed by four hex digits, according to the IEC 61131 standard and its EBNF.
     *
     * \return Needed buffer size for literal string without type delarator e.g., WSTRING#
     */
    size_t getToStringBufferSize() const override;

  protected:
    void fromCharString(const char *const paValue);

  private:
};

inline
bool operator ==(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight) {
  return (0 == strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator !=(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight) {
  return !(paLeft == paRight);
}

inline
bool operator >(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight){
  return (0 < strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator <(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight){
  return (0 > strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator >=(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight){
  return (0 <= strcmp(paLeft.getValue(), paRight.getValue()));
}

inline
bool operator <=(const CIEC_WSTRING &paLeft, const CIEC_WSTRING &paRight){
  return (0 >= strcmp(paLeft.getValue(), paRight.getValue()));
}

inline CIEC_WSTRING operator ""_WSTRING(const char16_t *paValue, size_t paLength) {
  return CIEC_WSTRING(paValue, paLength);
}

#endif

#endif /*_FORTE_WSTRING_H_*/
