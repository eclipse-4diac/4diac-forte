/*******************************************************************************
 * Copyright (c) 2024 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_SPLIT_fbt
 *******************************************************************************/

#pragma once

#include "genfb.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class GEN_E_SPLIT final : public CGenFunctionBlock<CFunctionBlock> {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT)

  private:
    static const TEventID scmEventEIID = 0;
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TForteInt16 scmEOWithIndexes[];
    std::unique_ptr<CStringDictionary::TStringId[]> scmEventOutputNames;


    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

  public:
    GEN_E_SPLIT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);



    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

};

