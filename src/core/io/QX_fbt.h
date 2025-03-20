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

#pragma once

#include "outputfb.h"
#include "forte_bool.h"

/*! /brief generic class for QX function blocks providing access to one boolean physical output
 *
 */
class FORTE_QX final : public forte::core::io::COutputFB<CIEC_BOOL> {
  DECLARE_FIRMWARE_FB(FORTE_QX)

public:
  FORTE_QX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

};



