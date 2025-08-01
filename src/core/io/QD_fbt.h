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
 *   Jose Cabral - Modification to double
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include "core/io/outputfb.h"
#include "core/datatypes/forte_dword.h"

/*! /brief generic class for QD function blocks providing access to one double word physical output
 */
class FORTE_QD final : public forte::core::io::COutputFB<CIEC_DWORD> {
    DECLARE_FIRMWARE_FB(FORTE_QD)

  public:
    FORTE_QD(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);
};
