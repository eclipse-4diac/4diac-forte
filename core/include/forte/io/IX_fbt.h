/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include "forte/io/inputfb.h"
#include "forte/datatypes/forte_bool.h"

namespace forte::io {
  /*! /brief generic class for IX function blocks providing access to one boolean physical input
   *
   */
  class FORTE_IX final : public CInputFB<CIEC_BOOL> {
      DECLARE_FIRMWARE_FB(FORTE_IX)

    public:
      FORTE_IX(StringId paInstanceNameId, CFBContainer &paContainer);
  };
} // namespace forte::io
