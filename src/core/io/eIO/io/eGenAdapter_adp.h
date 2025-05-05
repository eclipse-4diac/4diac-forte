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

#include "core/adapter.h"
#include "core/typelib.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

class FORTE_eGenAdapter final : public CAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_eGenAdapter)

  private:

    static const SFBInterfaceSpec scmFBInterfaceSpecSocket;

    static const SFBInterfaceSpec scmFBInterfaceSpecPlug;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
  public:
    FORTE_eGenAdapter(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug) :
        CAdapter(paContainer, scmFBInterfaceSpecSocket, paAdapterInstanceName, scmFBInterfaceSpecPlug, paIsPlug) {
    };

    virtual ~FORTE_eGenAdapter() = default;

    /* MODIFIED */
    CFunctionBlock *parentFB;

    void setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID) override {
      CAdapter::setParentFB(paParentFB, paParentAdapterlistID);
      parentFB = paParentFB;
    }
};

