/*******************************************************************************
 * Copyright (c) 2005, 2023 Pr3factor GmbH, ACIN, fortiss GmbH
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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteFloat constructor explicit,
 *        removed built-in type operator=, added castable CIEC types operator=
 *    Martin Jobst - add equals function
 *                 - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_REAL_H_
#define _FORTE_REAL_H_

#include "forte_any_real.h"

#include "forte_string.h"
#include "forte_wstring.h"
#include "forte_int.h"
#include "forte_sint.h"
#include "forte_uint.h"
#include "forte_usint.h"

/*!\ingroup COREDTS CIEC_REAL represents the real data type according to IEC 61131.
 */
class CIEC_REAL final : public CIEC_ANY_REAL{
  DECLARE_FIRMWARE_DATATYPE(REAL)

  public:
    typedef TForteFloat TValueType;

    CIEC_REAL() = default;

    CIEC_REAL(const CIEC_REAL& paValue) :
        CIEC_ANY_REAL() {
      setValueSimple(paValue);
    }

    CIEC_REAL(const CIEC_SINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_REAL(const CIEC_INT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_REAL(const CIEC_USINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_REAL(const CIEC_UINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    explicit CIEC_REAL(const TValueType paValue) {
      setTFLOAT(paValue);
    }

    ~CIEC_REAL() override = default;

    CIEC_REAL& operator =(const CIEC_REAL &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_REAL& operator =(const CIEC_INT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_REAL& operator =(const CIEC_UINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_REAL& operator =(const CIEC_SINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_REAL& operator =(const CIEC_USINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_REAL operator-() const {
      return CIEC_REAL(-1.0f * *this);
    }

    /*! \brief Converts CIEC_REAL to elementary 32 bit float
     *
     *   Conversion operator for converting CIEC_REAL to elementary 32 bit float
     */
    operator TForteFloat() const{
      return getTFLOAT();
    }

    void setValue(const CIEC_ANY& paValue) override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_REAL;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC 61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *paValue) override;
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC 61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the buffer String
     *   \param paBufferSize size of the buffer thats available for this function
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      if(paOther.getDataTypeID() == CIEC_ANY::e_REAL) {
        return getTFLOAT() == static_cast<const CIEC_REAL&>(paOther).getTFLOAT();
      }
      return false;
    }

    /*! \brief Casts TForteFloat values to CIEC data types
     *  \param paSrcValue Source value for the cast
     *  \param paDestValue Destination CIEC object of the cast
     */
    static void castRealData(const CIEC_REAL &paSrcValue, CIEC_ANY &paDestValue);
};

inline CIEC_REAL operator ""_REAL(unsigned long long int paValue) {
  return CIEC_REAL(static_cast<CIEC_REAL::TValueType>(paValue));
}

inline CIEC_REAL operator ""_REAL(long double paValue) {
  return CIEC_REAL(static_cast<CIEC_REAL::TValueType>(paValue));
}

#endif /*_FORTE_REAL_H_*/
