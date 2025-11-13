/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_elementary.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_ANY_CHARS represents ANY_CHARS data types according to
   *  IEC 61131.
   */
  class CIEC_ANY_CHARS : public CIEC_ANY_ELEMENTARY {
    public:
      CIEC_ANY_CHARS(const CIEC_ANY_CHARS &paVal) : CIEC_ANY_ELEMENTARY() {
        setLargestUInt(paVal.getLargestUInt());
      }

      ~CIEC_ANY_CHARS() override = default;

      EDataTypeID getDataTypeID() const override {
        return e_ANY;
      }

    protected:
      CIEC_ANY_CHARS() = default;

      static void dollarEscapeChar(std::string &paTargetBuf, char paSymbol, const EDataTypeID paTypeID);
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_CHARS> : std::true_type {};
} // namespace forte
