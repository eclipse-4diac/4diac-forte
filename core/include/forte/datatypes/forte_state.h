/*******************************************************************************
 * Copyright (c) 2022, 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_int.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_EVENT represents the STATE data type according to IEC 61131.
   */
  class CIEC_STATE final : public CIEC_ANY_INT {
      DECLARE_FIRMWARE_DATATYPE(STATE)

    public:
      typedef TForteUInt16 TValueType;

      static const TValueType scmMinVal = 0;
      static const TValueType scmMaxVal;

      CIEC_STATE() = default;

      CIEC_STATE(const CIEC_STATE &paValue) : CIEC_ANY_INT() {
        setValueSimple(paValue);
      }

      explicit CIEC_STATE(const TValueType paValue) {
        setTUINT16(paValue);
      }

      ~CIEC_STATE() override = default;

      CIEC_STATE &operator=(const CIEC_STATE &paValue) {
        // Simple value assignment - no self assignment check needed
        setValueSimple(paValue);
        return *this;
      }

      CIEC_STATE &operator=(const TForteUInt16 paValue) {
        // Simple value assignment - no self assignment check needed
        setTUINT16(paValue);
        return *this;
      }

      /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
       *
       *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
       */
      operator TForteUInt16() const {
        return getTUINT16();
      }

      EDataTypeID getDataTypeID() const override {
        return e_UINT;
      }
  };
} // namespace forte
