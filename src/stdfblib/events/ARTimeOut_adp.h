/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "core/adapter.h"
#include "core/datatypes/forte_time.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

class FORTE_ARTimeOut : public forte::CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_ARTimeOut)

  private:
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    public:
      static const TEventID scmEventTimeOutID = 0;

    private:
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    public:
      static const TEventID scmEventSTARTID = 0;
      static const TEventID scmEventSTOPID = 1;

    private:
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

  protected:
    static const SFBInterfaceSpec scmFBInterfaceSpecSocket;

    static const SFBInterfaceSpec scmFBInterfaceSpecPlug;

    void setInitialValues() override;
  public:
    CIEC_TIME var_DT;

    TEventID evt_TimeOut() {
      return getParentAdapterListEventID() + scmEventTimeOutID;
    }

    TEventID evt_START() {
      return getParentAdapterListEventID() + scmEventSTARTID;
    }

    TEventID evt_STOP() {
      return getParentAdapterListEventID() + scmEventSTOPID;
    }

    virtual ~FORTE_ARTimeOut() override = default;

  protected:
    FORTE_ARTimeOut(forte::core::CFBContainer &paContainer,
                  const SFBInterfaceSpec &paInterfaceSpec,
                  const CStringDictionary::TStringId paInstanceNameId,
                  TForteUInt8 paParentAdapterlistID);
};

class FORTE_ARTimeOut_Plug final : public FORTE_ARTimeOut {
  public:
    FORTE_ARTimeOut_Plug(CStringDictionary::TStringId paInstanceNameId,
                        forte::core::CFBContainer &paContainer,
                        TForteUInt8 paParentAdapterlistID);
    ~FORTE_ARTimeOut_Plug() override = default;

    CEventConnection conn_TimeOut;

    CDataConnection *conn_DT;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};

class FORTE_ARTimeOut_Socket final : public FORTE_ARTimeOut {
  public:
    FORTE_ARTimeOut_Socket(CStringDictionary::TStringId paInstanceNameId,
                        forte::core::CFBContainer &paContainer,
                        TForteUInt8 paParentAdapterlistID);
    ~FORTE_ARTimeOut_Socket() override = default;

    CEventConnection conn_START;
    CEventConnection conn_STOP;

    COutDataConnection<CIEC_TIME> conn_DT;

  private:
    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;
};
