/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN
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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *    Ingo Hegny, Monika Wenger, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteDFloat constructor explicit, 
 *      adds implicit cast constructor and operator=, removed built-in type operator=,
 *      added castable CIEC types operator=
 *    Martin Jobst - add equals function
 *                 - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_LREAL_H_
#define _FORTE_LREAL_H_

#include "forte_any_real.h"
#include "forte_real.h"
#include "forte_dint.h"
#include "forte_int.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_usint.h"

/*!\ingroup COREDTS CIEC_LREAL represents the lreal data type according to IEC 61131.
 */
class CIEC_LREAL final : public CIEC_ANY_REAL{
  DECLARE_FIRMWARE_DATATYPE(LREAL)

  public:
    typedef TForteDFloat TValueType;

    CIEC_LREAL() = default;

    CIEC_LREAL(const CIEC_LREAL& paValue) :
        CIEC_ANY_REAL() {
      setValueSimple(paValue);
    }

    CIEC_LREAL(const CIEC_REAL& paValue) :
        CIEC_ANY_REAL() {
      setTDFLOAT(static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(paValue)));
    }

    CIEC_LREAL(const CIEC_DINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_LREAL(const CIEC_INT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_LREAL(const CIEC_SINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_LREAL(const CIEC_UDINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_LREAL(const CIEC_UINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    CIEC_LREAL(const CIEC_USINT& paValue) :
        CIEC_ANY_REAL() {
      setValue(paValue);
    }

    explicit CIEC_LREAL(const TValueType paValue) {
      setTDFLOAT(paValue);
    }

    ~CIEC_LREAL() override = default;

    CIEC_LREAL& operator =(const CIEC_LREAL &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_REAL &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_DINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_INT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_SINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_UDINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_UINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_USINT &paValue){
      setValue(paValue);
      return *this;
    }

    CIEC_LREAL operator-() const {
      return CIEC_LREAL(-1.0 * *this);
    }

    /*! \brief Converts CIEC_LREAL to elementary 64 bit float
     *
     *   Conversion operator for converting CIEC_LREAL to elementary 64 bit float
     */

    operator TForteDFloat() const{
      return getTDFLOAT();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LREAL;
    }

    void setValue(const CIEC_ANY& paValue) override;

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
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the buffer String
     *   \param paBufferSize size of the buffer thats available for this function
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      if(paOther.getDataTypeID() == CIEC_ANY::e_LREAL) {
        return getTDFLOAT() == static_cast<const CIEC_LREAL&>(paOther).getTDFLOAT();
      }
      return false;
    }

    /*! \brief Casts TForteDFloat values to CIEC data types
     *  \param paSrcValue Source value for the cast
     *  \param paDestValue Destination CIEC object of the cast
     */
    static void castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue);
};

inline CIEC_LREAL operator ""_LREAL(unsigned long long int paValue) {
  return CIEC_LREAL(static_cast<CIEC_LREAL::TValueType>(paValue));
}

inline CIEC_LREAL operator ""_LREAL(long double paValue) {
  return CIEC_LREAL(static_cast<CIEC_LREAL::TValueType>(paValue));
}

#endif /*_FORTE_LREAL_H_*/
