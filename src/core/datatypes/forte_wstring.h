/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH
 *   2018 TU Wien/ACIN
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
 *      - fixes behavior for getToStringBufferSize
 *******************************************************************************/
#ifndef _FORTE_WSTRING_H_
#define _FORTE_WSTRING_H_

#include "forte_any_string.h"

#ifdef FORTE_USE_WSTRING_DATATYPE

/*!\ingroup COREDTS \brief CIEC_WSTRING represents the wide string data type according to IEC 61131.
 *
 *  "wstring" is implemented like the "string" - the following first implementation
 *  "typedef std::basic_string<wchar_t> TWSTRING; didn't work well"
 */
class CIEC_WSTRING : public CIEC_ANY_STRING{
  DECLARE_FIRMWARE_DATATYPE(WSTRING)
  public:
    CIEC_WSTRING(){
    }

    CIEC_WSTRING(const CIEC_WSTRING& pa_roValue) :
        CIEC_ANY_STRING(){
      (*this) = pa_roValue.getValue();
    }

    CIEC_WSTRING(const char* pa_pacValue){
      (*this) = pa_pacValue;
    }

    virtual ~CIEC_WSTRING(){
    }

    CIEC_WSTRING &operator =(const char* const pa_pacValue){
      CIEC_ANY_STRING::operator =(pa_pacValue);
      return *this;
    }

    /*! \brief Converts a UTF-8 encoded string to a WSTRING (ISO 10646 Row 00 internally)
     *
     *   This command implements a conversion function from a UTF-8
     *   encoded string (found e.g. in XML to the internal
     *   ISO 10646 Row 00 encoding.
     *   \param pa_pacBuffer  Reference to the given UTF-8 encoded byte array
     *   \param pa_nLen  Length to read (-1 for null-terminated)
     *   \param pa_bUnescape  Handle $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used from srcString
     *       -1 on error
     */
    virtual int fromUTF8(const char *pa_pacValue, int pa_nLen, bool pa_bUnescape);

    /*! \brief Converts the WSTRING to a UTF-8 representation
     *
     *   This command implements a conversion function from a WSTRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param pa_pacBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param pa_nBufferSize  Size of the provided buffer.
     *   \param pa_bEscape  Produce $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const;

    /*! \brief Converts a UTF-16 encoded string to a WSTRING (UTF-8 internally)
     *
     *   This command implements a conversion function from a UTF-16
     *   encoded string (found e.g. in serialized WSTRING type) to the internal
     *   UTF-8 encoding.
     *   \param pa_pacBuffer  Reference to the given UTF-16 encoded byte array
     *   \param pa_nBufferLen  Length of the provided byte array
     *   \return Can be the following response:
     *     - false....conversion was not successful - something went wrong!
     *     -  true....conversion was successful.
     */
    bool fromUTF16(const TForteByte *pa_pacBuffer, unsigned int pa_nBufferLen);

    /*! \brief Converts the WSTRING to a UTF-16 representation
     *
     *   This command implements a conversion function from a WSTRING
     *   to a big-endian UTF-16 encoding, usable e.g. for the serialization.
     *   \param pa_pacBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param pa_nBufferSize  Size of the provided buffer.
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    int toUTF16(TForteByte* pa_pacBuffer, unsigned int pa_nBufferSize) const;

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_WSTRING;
    }

    virtual void setValue(const CIEC_ANY& pa_roValue){
      if(pa_roValue.getDataTypeID() == CIEC_ANY::e_WSTRING){
        const CIEC_WSTRING &roSrc(static_cast<const CIEC_WSTRING &>(pa_roValue));
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
     *   \param pa_acValue          Pointer to char-array for the result
     *   \param pa_nBufferSize      Size of the buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    /*! \brief Returns the amount of bytes needed to create the IEC 61131 literal string
     *
     * IEC 61131 literal strings needs to delimit all non Common_Char_Value literals and special
     * symbols, like line-feed, into escaped symbol sequences. For single byte strings the special
     * symbols are represented as dollar escaped strings followed by two hex digits. For double byte strings
     * is is dollar followed by four hex digits, according to the IEC 61131 standard and its EBNF.
     *
     * \return Needed buffer size for literal string without type delarator e.g., WSTRING#
     */
    virtual size_t getToStringBufferSize() const;

  protected:

  private:
};

inline
bool operator ==(const CIEC_WSTRING &pa_roLeft, const CIEC_WSTRING &pa_roRight){
  return (0 == strcmp(pa_roLeft.getValue(), pa_roRight.getValue()));
}

inline
bool operator !=(const CIEC_WSTRING &pa_roLeft, const CIEC_WSTRING &pa_roRight){
  return !(pa_roLeft == pa_roRight);
}

#endif

#endif /*_FORTE_WSTRING_H_*/
