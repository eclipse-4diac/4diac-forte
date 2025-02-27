/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#include "PortAdapter_adp.h"
#include "forte_dword.h"
#include "forte_wstring.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "funcbloc.h"

class FORTE_Port final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_Port)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const int scmPortInAdapterAdpNum = 0;
    static const SAdapterInstanceDef scmAdapterInstances[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    // The (maximum) number  of IO pins available on a GPIO port.
    static constexpr size_t pin_cnt = 16;
    std::array<CIEC_STRING *, pin_cnt> mRegistered;

    void deregister_handles();
    void register_handles();

  public:
    FORTE_Port(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    bool initialize() override;

    CIEC_STRING var_Pin0;
    CIEC_STRING var_Pin1;
    CIEC_STRING var_Pin2;
    CIEC_STRING var_Pin3;
    CIEC_STRING var_Pin4;
    CIEC_STRING var_Pin5;
    CIEC_STRING var_Pin6;
    CIEC_STRING var_Pin7;
    CIEC_STRING var_Pin8;
    CIEC_STRING var_Pin9;
    CIEC_STRING var_Pin10;
    CIEC_STRING var_Pin11;
    CIEC_STRING var_Pin12;
    CIEC_STRING var_Pin13;
    CIEC_STRING var_Pin14;
    CIEC_STRING var_Pin15;

    FORTE_PortAdapter var_PortInAdapter;

    CDataConnection *conn_Pin0;
    CDataConnection *conn_Pin1;
    CDataConnection *conn_Pin2;
    CDataConnection *conn_Pin3;
    CDataConnection *conn_Pin4;
    CDataConnection *conn_Pin5;
    CDataConnection *conn_Pin6;
    CDataConnection *conn_Pin7;
    CDataConnection *conn_Pin8;
    CDataConnection *conn_Pin9;
    CDataConnection *conn_Pin10;
    CDataConnection *conn_Pin11;
    CDataConnection *conn_Pin12;
    CDataConnection *conn_Pin13;
    CDataConnection *conn_Pin14;
    CDataConnection *conn_Pin15;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CAdapter *getAdapterUnchecked(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};

