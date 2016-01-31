/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Rene Smodic, Alois Zoitl, Gerhard Ebenhofer
  *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger, Stanislav Meduna,
  *    Matthias Plasch
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_STRING_H_
#define _FORTE_STRING_H_

#include "forte_any_string.h"
#include <string.h>
#include <stdlib.h>

namespace forte {
  namespace com_infra {
    class CFBDKASN1ComLayer;
  }
}

/*!\ingroup COREDTS CIEC_STRING represents the string data type according to IEC 61131.
 */
class CIEC_STRING : public CIEC_ANY_STRING{
  DECLARE_FIRMWARE_DATATYPE(STRING)
    ;
  public:
    CIEC_STRING() {
    }

    CIEC_STRING(const char* pa_pacValue) {
      (*this) = pa_pacValue;
    }

    virtual ~CIEC_STRING(){
    }

    CIEC_STRING &operator =(const char* const pa_pacValue) {
      CIEC_ANY_STRING::operator =(pa_pacValue);
      return *this;
    }

    virtual EDataTypeID getDataTypeID() const{
          return CIEC_ANY::e_STRING;
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
    virtual int toString(char* pa_acValue, unsigned int pa_nBufferSize) const ;

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
    virtual int fromUTF8(const char *pa_pacValue, int pa_nLen, bool pa_bUnescape);
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
    virtual int toUTF8(char* pa_pacBuffer, unsigned int pa_nBufferSize, bool pa_bEscape) const;
#endif
  protected:


    virtual void setValue(const CIEC_ANY &pa_roValue){
      if (pa_roValue.getDataTypeID() == CIEC_ANY::e_STRING) {
        const CIEC_STRING &roSrc(static_cast<const CIEC_STRING &> (pa_roValue));
        this->assign(roSrc.getValue(), roSrc.length());
      }
    }

  private:
};

inline
bool operator ==(const CIEC_STRING &pa_roLeft, const CIEC_STRING &pa_roRight) {
  return (0 == strcmp(pa_roLeft.getValue(), pa_roRight.getValue()));
}

inline
bool operator !=(const CIEC_STRING &pa_roLeft, const CIEC_STRING &pa_roRight) {
  return !(pa_roLeft ==pa_roRight);
}

#endif /*_FORTE_STRING_H_*/
