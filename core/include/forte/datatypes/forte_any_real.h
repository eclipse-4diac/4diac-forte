/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Monika Wenger,
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_num.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_ANY_REAL represents any real data types according to IEC 61131.

   */
  class CIEC_ANY_REAL : public CIEC_ANY_NUM {
    public:
      ~CIEC_ANY_REAL() override = default;

    protected:
      CIEC_ANY_REAL() = default;

      static void normalizeToStringRepresentation(std::string &paTargetBuf, size_t paStartPos);
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_REAL> : std::true_type {};
} // namespace forte
