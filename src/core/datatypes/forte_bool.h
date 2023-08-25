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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik-Merkiumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make bool constructor explicit,
 *        removed built-in type operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_BOOL_H_
#define _FORTE_BOOL_H_

#include "forte_any_bit.h"

/*!\ingroup COREDTS CIEC_BOOL represents the bool data type according to IEC 61131.
 */
class CIEC_BOOL : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(BOOL)

  public:

    typedef bool TValueType;

    static constexpr TValueType scmMinVal = std::numeric_limits<CIEC_BOOL::TValueType>::min();
    static constexpr TValueType scmMaxVal = std::numeric_limits<CIEC_BOOL::TValueType>::max();

    CIEC_BOOL() {
      setTBOOL8(false);
    }

    CIEC_BOOL(const CIEC_BOOL& paValue) :
        CIEC_ANY_BIT() {
      *this = paValue;
    }

    explicit CIEC_BOOL(const TValueType paValue) {
      setTBOOL8(paValue);
    }

    ~CIEC_BOOL() override = default;

    CIEC_BOOL& operator =(const CIEC_BOOL &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_BOOL to elementary bool
     *
     *   Conversion operator for converting CIEC_BOOL to elementary bool
     */
    operator bool() const {
      return (0 != getLargestUInt());
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_BOOL;
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
     *   This function is necessary for communication with a proper engineering systems.
     *   \param paValue Pointer to the provided buffer
     *   \param paBufferSize Size of the provided buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

  private:
};

const CIEC_BOOL true_BOOL = CIEC_BOOL(true);
const CIEC_BOOL false_BOOL = CIEC_BOOL(false);

inline CIEC_BOOL operator ""_BOOL(unsigned long long int paValue) {
  return CIEC_BOOL(paValue);
}

#endif /*_FORTE_BOOL_H_*/
