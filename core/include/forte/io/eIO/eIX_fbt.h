/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "forte/io/eIO/einputfb.h"
#include "forte/datatypes/forte_bool.h"

namespace forte::io {
  /*! /brief generic class for eIX function blocks providing (event-triggerd) access to one boolean physical input
   *
   */
  class FORTE_eIX final : public CeInputFB<CIEC_BOOL> {
      DECLARE_FIRMWARE_FB(FORTE_eIX)

    public:
      FORTE_eIX(StringId paInstanceNameId, CFBContainer &paContainer);
  };
} // namespace forte::io
