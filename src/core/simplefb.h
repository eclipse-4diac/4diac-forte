/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 *               2022 - 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - multi algorithm support
 *      - account for data type size in FB initialization
 *******************************************************************************/

#pragma once

#include "basefb.h"

class CSimpleFB : public CBaseFB {
  protected:
    CSimpleFB(forte::core::CFBContainer &paContainer,
              const SFBInterfaceSpec &paInterfaceSpec,
              CStringDictionary::TStringId paInstanceNameId,
              std::span<const CStringDictionary::TStringId> paVarInternalNames) :
        CBaseFB(paContainer, paInterfaceSpec, paInstanceNameId, paVarInternalNames) {
    }

    void setInitialValues() override = 0;
};
