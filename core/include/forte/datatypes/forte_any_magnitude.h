/*******************************************************************************
 * Copyright (c) 2005 - 2011 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Monika Wenger,
 *    Martin Melik Markumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_elementary.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_ANY_MAGNITUDE represents the magnitude data types according to
   *  IEC 61131.

   */
  class CIEC_ANY_MAGNITUDE : public CIEC_ANY_ELEMENTARY {
    public:
      ~CIEC_ANY_MAGNITUDE() override = default;

      EDataTypeID getDataTypeID() const override {
        return e_ANY;
      }

    protected:
      CIEC_ANY_MAGNITUDE() = default;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_MAGNITUDE> : std::true_type {};
} // namespace forte
