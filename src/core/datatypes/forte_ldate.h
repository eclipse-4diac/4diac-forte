/*******************************************************************************
 * Copyright (c) 2008, 2023 nxtControl GmbH, ACIN, fortiss GmbH
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
 *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteUInt64 constructor explicit,
 *        removed built-in type operator=, removed operator++
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_LDATE_H_
#define _FORTE_LDATE_H_

#include "forte_any_date.h"
#include "forte_date.h"

/*!\ingroup COREDTS CIEC_DATE represents the time data types according to IEC 61131.
 */

class CIEC_LDATE final : public CIEC_ANY_DATE {
  DECLARE_FIRMWARE_DATATYPE(LDATE)
  public:
    CIEC_LDATE() = default;

    CIEC_LDATE(const CIEC_LDATE& paValue) :
        CIEC_ANY_DATE() {
      setValueSimple(paValue);
    }

    CIEC_LDATE(const CIEC_DATE& paValue) :
        CIEC_ANY_DATE() {
      setValue(paValue);
    }

    explicit CIEC_LDATE(const TValueType paValue) {
      setTUINT64(paValue);
    }

    ~CIEC_LDATE() override = default;

    CIEC_LDATE& operator =(const CIEC_LDATE &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LDATE& operator =(const CIEC_DATE &paValue) {
      setValue(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_TIME to unsigned 64 bit integer
     *
     *   Conversion operator for converting CIEC_TIME to unsigned 64 bit integer
     */
    operator TForteUInt64() const {
      return getTUINT64();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LDATE;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC 61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the given String
     *   \return Can be the following response:
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *paValue) override;
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC 61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the provided buffer
     *   \param paBufferSize Size of the provided buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;
};

inline CIEC_LDATE operator ""_LDATE(unsigned long long int paValue) {
  return CIEC_LDATE(static_cast<CIEC_LDATE::TValueType>(paValue));
}

#endif /*_FORTE_LDATE_H_*/
