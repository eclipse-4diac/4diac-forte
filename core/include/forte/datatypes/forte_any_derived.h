/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_ANY_DERIVED represents the user defined data types according to
   *  IEC 61131.
   */
  class CIEC_ANY_DERIVED : public CIEC_ANY {
    public:
      ~CIEC_ANY_DERIVED() override = default;

    protected:
      CIEC_ANY_DERIVED() = default;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_DERIVED> : std::true_type {};
} // namespace forte
