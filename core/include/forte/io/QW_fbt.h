/*************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz  - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include "forte/io/outputfb.h"
#include "forte/datatypes/forte_word.h"

namespace forte::io {
  /*! /brief generic class for QW function blocks providing access to one word physical output
   *
   */
  class FORTE_QW final : public COutputFB<CIEC_WORD> {
      DECLARE_FIRMWARE_FB(FORTE_QW)

    public:
      FORTE_QW(StringId paInstanceNameId, CFBContainer &paContainer);
  };
} // namespace forte::io
