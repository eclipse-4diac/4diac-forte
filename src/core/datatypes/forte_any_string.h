/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *   2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Stanislav Meduna, Monika Wenger, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixes behavior for getToStringBufferSize, removed built-in type operator=
 *******************************************************************************/
#ifndef _ANY_STR_H_
#define _ANY_STR_H_

#include "forte_any_chars.h"

/*!\ingroup COREDTS IIEC_ANY_STRING represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_STRING : public CIEC_ANY_CHARS {
  public:
    static constexpr size_t scmMaxStringLen = 65534; //STRING: 0 is reserved, so 65534 is max / WSTRING: save one for the \0 needed for allocated length

    ~CIEC_ANY_STRING() override;

    CIEC_ANY_STRING(const CIEC_ANY_STRING& paValue) :
        CIEC_ANY_CHARS(){
      this->assign(paValue.getValue(), paValue.length());
    }

    CIEC_ANY_STRING &operator=(const CIEC_ANY_STRING& paValue){
      if(this != &paValue){
        this->assign(paValue.getValue(), paValue.length());
      }
      return *this;
    }

    /*! \brief Get-Method for CIEC_ANY_STRING
     *
     *   With this command the value of the actual object can be read.
     *   \param - No parameters necessary.
     *   \return Can be the following response:
     *     - Actual value of the object.
     */

    virtual char* getValue() {
      return ((char *) ((nullptr != getGenData()) ? reinterpret_cast<char*>(getGenData() + 4) : smNullString));
    }

    virtual const char *getValue() const {
      return (const char *) ((nullptr != getGenData()) ? reinterpret_cast<const char*>(getGenData() + 4) : smNullString);
    }

    virtual TForteUInt16 length() const {
      return (nullptr != getGenData()) ? (*((TForteUInt16 *) (getGenData()))) : static_cast<TForteUInt16>(0);
    }

    virtual void clear() {
      assign("", 0);
    }

    virtual bool empty() const{
      return length() == 0;
    }

    /*! Assign arbitrary data (can contain '0x00')
     */
    virtual void assign(const char *paData, TForteUInt16 paLen);

    /*! Append arbitrary data (can contain '0x00')
     */
    virtual void append(const char *paData, TForteUInt16 paLen);

    /*! Append data, cannot contain '0x00' as this is used to identify the end of the cstring
     */
    virtual void append(const char *paData);

    /*! Try to reserve enough space to hold a string with given length.
     *  After this function the string will be at least of the size given.
     */
    virtual void reserve(const TForteUInt16 paRequestedSize);

    /*! Retrieve the current allocated size
     *
     * @return number of bytes that this string has allocated for use
     */
    TForteUInt16 getCapacity() const {
      return (nullptr != getGenData()) ? (*((TForteUInt16 *)(getGenData() + 2))) : static_cast<TForteUInt16>(0);
    }

#ifdef FORTE_UNICODE_SUPPORT
    /*! \brief Converts the variable to a UTF-8 representation
     *
     *   This command implements a conversion function from a WSTRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param paBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param paBufferSize  Size of the provided buffer.
     *   \param paEscape  Produce $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toUTF8(char*, size_t, bool) const {
      return 0;
    }
#endif

  protected:
    static char smNullString[];

    /*! \brief Determines the source length of a potentially escaped string
     *
     *   If the given string starts with a delimiter, the method searches for the ending
     *   delimiter and return the length including the delimiters.
     *   If the string does not start with the delimiter, this method is equivalent to strlen.
     *
     *   Note: the returned length can be > scmMaxStringLen, so even larger strings
     *   can be truncated and their end still be found
     *
     *   \param paValue  Source string
     *   @param paDelimiter string delimiting character (i.e., ' for STRING, " for WSTRING)
     *   \return length of the string, -1 if it starts with a delimiter but does not end with one
     */
    static int determineEscapedStringLength(const char *paValue, char paDelimiter);

    static bool handleDollarEscapedChar(const char **paSymbol, bool paWide, TForteUInt16 &paValue);
    // Use null as destination for just determining the need of escaping
    static int dollarEscapeChar(char *paValue, char paSymbol, unsigned int paBufferSize, const EDataTypeID paTypeID);
    static bool parseEscapedHexNum(const char **paSymbol, bool paWide, TForteUInt16 &paValue);

    /*! \brief Unescape the input string.
     *
     *  As the unescaping never makes the string longer, it can be used in-place
     *  @param paValue  the source trong to unescape from
     *  @param paDelimiter string delimiting character (i.e., ' for STRING, " for WSTRING)
     *  @return on success number of bytes take from src string
     *          -1 on error
     */
    int unescapeFromString(const char *paValue, char paDelimiter);

    void setLength(TForteUInt16 paVal){
      TForteByte *pBuf = getGenData();
      if(nullptr != pBuf){
        *((TForteUInt16 *) (pBuf)) = paVal;
      }
    }

    void setAllocatedLength(TForteUInt16 paVal){
      TForteByte *pBuf = getGenData();
      if(nullptr != pBuf){
        *((TForteUInt16 *) (pBuf + 2)) = paVal;
      }
    }

    CIEC_ANY_STRING() = default;
};

#endif /*_MANY_STR_H_*/
