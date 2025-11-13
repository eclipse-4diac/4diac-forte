/*******************************************************************************
 * Copyright (c) 2005, 2013 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *      Monika Wenger, Stansilav Meduna
 *                - initial implementation and rework communication infrastructure
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"

namespace forte {
  /*!\ingroup COREDTS IIEC_ANY_ELEMENTARY represents the elementary data types according to
   *  IEC 61131.
   */
  class CIEC_ANY_ELEMENTARY : public CIEC_ANY {
    public:
      CIEC_ANY_ELEMENTARY(const CIEC_ANY_ELEMENTARY &paVal) : CIEC_ANY() {
        setLargestUInt(paVal.getLargestUInt());
      }

      ~CIEC_ANY_ELEMENTARY() override = default;

      EDataTypeID getDataTypeID() const override {
        return e_ANY;
      }

      void toString(std::string &paTargetBuf) const override;
      int fromString(const char *paValue) override;

      static EDataTypeID getElementaryDataTypeId(StringId paTypeNameId);

    protected:
      CIEC_ANY_ELEMENTARY() = default;

    private:
      bool isTypeSpecifier(const char *paValue, const char *paHashPosition) const;
      bool isCastable(StringId paTypeNameId) const;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_ELEMENTARY> : std::true_type {};
} // namespace forte
