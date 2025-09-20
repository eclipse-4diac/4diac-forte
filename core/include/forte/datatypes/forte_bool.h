/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN,
 *                          Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Martin Melik-Merkiumians, Monika Wenger
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make bool constructor explicit,
 *                  removed built-in type operator=
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_BOOL represents the bool data type according to IEC 61131.
   */
  class CIEC_BOOL : public CIEC_ANY_BIT {
      DECLARE_FIRMWARE_DATATYPE(BOOL)

    public:
      using TValueType = bool;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 1U;

      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      CIEC_BOOL() {
        setTBOOL8(false);
      }

      CIEC_BOOL(const CIEC_BOOL &paValue) : CIEC_ANY_BIT() {
        *this = paValue;
      }

      explicit CIEC_BOOL(const TValueType paValue) {
        setTBOOL8(paValue);
      }

      ~CIEC_BOOL() override = default;

      CIEC_BOOL &operator=(const CIEC_BOOL &paValue) {
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

      EDataTypeID getDataTypeID() const override final {
        return e_BOOL;
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
       *   \param paTargetBuf Reference to the provided buffer
       */
      void toString(std::string &paTargetBuf) const override;

    private:
  };

  const CIEC_BOOL true_BOOL = CIEC_BOOL(true);
  const CIEC_BOOL false_BOOL = CIEC_BOOL(false);

  inline CIEC_BOOL operator""_BOOL(unsigned long long int paValue) {
    return CIEC_BOOL(paValue);
  }

  template<>
  struct CDataTypeTrait<CIEC_BOOL> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_BOOL;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_BOOL> : public forte::templates::numeric_limits<forte::CIEC_BOOL> {
      static constexpr size_t bitLength = 1U;
  };
} // namespace std
