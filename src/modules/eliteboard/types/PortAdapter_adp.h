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

#include "adapter.h"
#include "typelib.h"
#include "forte_dword.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

class FORTE_PortAdapter final : public CAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_PortAdapter)

  private:
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    public:
      static const TEventID scmEventMAPOID = 0;

    private:
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const CStringDictionary::TStringId scmEventInputTypeIds[];


    public:
      static const TEventID scmEventMAPID = 0;

    private:
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];


    static const SFBInterfaceSpec scmFBInterfaceSpecSocket;

    static const SFBInterfaceSpec scmFBInterfaceSpecPlug;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
  public:
    CIEC_DWORD &var_GPIO_Port_Addr() {
      return *static_cast<CIEC_DWORD*>((isSocket()) ? getDO(0) : getDI(0));
    }

    TEventID evt_MAPO() {
      return mParentAdapterListEventID + scmEventMAPOID;
    }

    TEventID evt_MAP() {
      return mParentAdapterListEventID + scmEventMAPID;
    }

    FORTE_PortAdapter(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug) :
        CAdapter(paContainer, scmFBInterfaceSpecSocket, paAdapterInstanceName, scmFBInterfaceSpecPlug, paIsPlug) {
    };

    virtual ~FORTE_PortAdapter() = default;
};

