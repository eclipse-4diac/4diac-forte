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
 *   Alois Zoitl, Waldemar Eisenmenger
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/
#include "forte/io/QX_fbt.h"

using namespace forte::literals;

namespace forte::io {
  DEFINE_FIRMWARE_FB(FORTE_QX, "QX"_STRID)

  FORTE_QX::FORTE_QX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      COutputFB<CIEC_BOOL>(paContainer, paInstanceNameId) {
  }
} // namespace forte::io
