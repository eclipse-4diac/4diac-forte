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
 *      - fixes behavior for getToStringBufferSize
 *******************************************************************************/
#ifndef _ANY_STR_H_
#define _ANY_STR_H_

#include "forte_any_elementary.h"

/*!\ingroup COREDTS IIEC_ANY_STRING represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_STRING : public CIEC_ANY_ELEMENTARY{
  DECLARE_FIRMWARE_DATATYPE(ANY_STRING)

  public:
    static const unsigned int scm_unMaxStringLen = 65534; //save one for the \0 needed for allocated length

    virtual ~CIEC_ANY_STRING();

    CIEC_ANY_STRING(const CIEC_ANY_STRING& paValue) :
        CIEC_ANY_ELEMENTARY(){
      this->assign(paValue.getValue(), paValue.length());
    }

    CIEC_ANY_STRING &operator=(const CIEC_ANY_STRING& paValue){
      if(this != &paValue){
        this->assign(paValue.getValue(), paValue.length());
      }
      return *this;
    }

    /*! \brief Operator: CIEC_STRING data type = string data type
     *
     *   This command implements the assignment operator for the C++ datatype STRING
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_pacValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_ANY_STRING& operator =(const char* const pa_pacValue);

    /*! \brief Get-Method for CIEC_ANY_STRING
     *
     *   With this command the value of the actual object can be read.
     *   \param - No parameters necessary.
     *   \return Can be the following response:
     *     - Actual value of the object.
     */

    char* getValue(void){
      return ((char *) ((0 != getGenData()) ? reinterpret_cast<char*>(getGenData() + 4) : sm_acNullString));
    }

    const char *getValue(void) const{
      return (const char *) ((0 != getGenData()) ? reinterpret_cast<const char*>(getGenData() + 4) : sm_acNullString);
    }

    TForteUInt16 length() const{
      return (0 != getGenData()) ? (*((TForteUInt16 *) (getGenData()))) : static_cast<TForteUInt16>(0);
    }

    void clear(){
      assign("", 0);
    }

    bool empty() const{
      return length() == 0;
    }

    /*! Assign arbitrary data (can contain '0x00')
     */
    void assign(const char *pa_poData, TForteUInt16 pa_nLen);

    /*! Append arbitrary data (can contain '0x00')
     */
    void append(const char *pa_poData, TForteUInt16 pa_nLen);

    /*! Append arbitrary data (can contain '0x00')
     */
    void append(const char *pa_poData);

    /*! Try to reserve enough space to hold a string with given length.
     *  After this function the string will be at least of the size given.
     */
    void reserve(TForteUInt16 pa_nRequestedSize);

    /*! Retrieve the current allocated size
     *
     * @return number of bytes that this string has allocated for use
     */
    TForteUInt16 getCapacity() const{
      return (0 != getGenData()) ? (*((TForteUInt16 *) (getGenData() + 2))) : static_cast<TForteUInt16>(0);
    }

#ifdef FORTE_UNICODE_SUPPORT
    /*! \brief Converts a UTF-8 encoded string to a variable
     *
     *   This command implements a conversion function from a UTF-16
     *   encoded string (found e.g. in XML to the internal
     *   ISO 10646 Row 00 encoding.
     *   \param pa_pacBuffer  Reference to the given UTF-8 encoded byte array
     *   \param pa_nLen  Length to read (-1 for null-terminated)
     *   \param pa_bUnescape  Handle $-escapes and delimiter characters at the beginning and end
     *   \return Can be the following response:
     *   \return number of bytes used from srcString
     *       -1 on error
     */
    virtual int fromUTF8(const char *, int, bool){return 0;}

    /*! \brief Converts the variable to a UTF-8 representation
     *
     *   This command implements a conversion function from a WSTRING
     *   to a UTF-8 encoding, usable e.g. for the serialization.
     *   \param pa_pacBuffer  Reference to the output buffer. If 0, only the needed size will be computed.
     *   \param pa_nBufferSize  Size of the provided buffer.
     *   \param pa_bEscape  Produce $-escapes and delimiter characters at the beginning and end
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toUTF8(char*, size_t, bool) const {
      return 0;
    }
#endif

  protected:
    static char sm_acNullString[];

    /*! \brief Determines the source length of a potentially escaped string
     *
     *   If the given string starts with a delimiter, the method searches for the ending
     *   delimiter and return the length including the delimiters.
     *   If the string does not start with the delimiter, this method is equivalent to strlen.
     *
     *   Note: the returned length can be > scm_unMaxStringLen, so even larger strings
     *   can be truncated and their end still be found
     *
     *   \param pa_pacValue  Source string
     *   @param pa_cDelimiter string delimiting character (i.e., ' for STRING, " for WSTRING)
     *   \return length of the string, -1 if it starts with a delimiter but does not end with one
     */
    static int determineEscapedStringLength(const char *pa_pacValue, char pa_cDelimiter);

    static bool handleDollarEscapedChar(const char **pa_pacValue, bool pa_bWide, TForteUInt16 &pa_rnValue);
    // Use null as destination for just determining the need of escaping
    static int dollarEscapeChar(char *pa_pacValue, char pa_cValue, unsigned int pa_nBufferSize);
    static bool parseEscapedHexNum(const char **pa_pacValue, bool pa_bWide, TForteUInt16 &pa_rnValue);

    /*! \brief Unescape the input string.
     *
     *  As the unescaping never makes the string longer, it can be used in-place
     *  @param pa_pacValue  the source trong to unescape from
     *  @param pa_cDelimiter string delimiting character (i.e., ' for STRING, " for WSTRING)
     *  @return on success number of bytes take from src string
     *          -1 on error
     */
    int unescapeFromString(const char *pa_pacValue, char pa_cDelimiter);

    void setLength(TForteUInt16 pa_unVal){
      TForteByte *pBuf = getGenData();
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = pa_unVal;
      }
    }

    void setAllocatedLength(TForteUInt16 pa_unVal){
      TForteByte *pBuf = getGenData();
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf + 2)) = pa_unVal;
      }
    }

    CIEC_ANY_STRING(){
    }
};

#endif /*_MANY_STR_H_*/
