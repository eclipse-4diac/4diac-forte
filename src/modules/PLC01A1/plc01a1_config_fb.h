/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once
#define _PLC01A1_H_

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_uint.h"
#include "forte_wstring.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include <io/configFB/io_configFB_controller.h>

class PLC01A1ConfigFB : public forte::core::io::IOConfigFBController {
  DECLARE_FIRMWARE_FB(PLC01A1ConfigFB)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_WSTRING &IN1() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  CIEC_WSTRING &IN2() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  };

  CIEC_WSTRING &IN3() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  };

  CIEC_WSTRING &IN4() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  };

  CIEC_WSTRING &IN5() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  };

  CIEC_WSTRING &IN6() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  };

  CIEC_WSTRING &IN7() {
    return *static_cast<CIEC_WSTRING*>(getDI(7));
  };

  CIEC_WSTRING &IN8() {
    return *static_cast<CIEC_WSTRING*>(getDI(8));
  };

  CIEC_WSTRING &OUT1() {
    return *static_cast<CIEC_WSTRING*>(getDI(9));
  };

  CIEC_WSTRING &OUT2() {
    return *static_cast<CIEC_WSTRING*>(getDI(10));
  };

  CIEC_WSTRING &OUT3() {
    return *static_cast<CIEC_WSTRING*>(getDI(11));
  };

  CIEC_WSTRING &OUT4() {
    return *static_cast<CIEC_WSTRING*>(getDI(12));
  };

  CIEC_WSTRING &OUT5() {
    return *static_cast<CIEC_WSTRING*>(getDI(13));
  };

  CIEC_WSTRING &OUT6() {
    return *static_cast<CIEC_WSTRING*>(getDI(14));
  };

  CIEC_WSTRING &OUT7() {
    return *static_cast<CIEC_WSTRING*>(getDI(15));
  };

  CIEC_WSTRING &OUT8() {
    return *static_cast<CIEC_WSTRING*>(getDI(16));
  };

  CIEC_UINT &UpdateInterval() {
    return *static_cast<CIEC_UINT*>(getDI(17));
  };
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };
    static const TEventID scmEventINITID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const CStringDictionary::TStringId scmEventInputTypeIds[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;


virtual void setInitialValues();

  protected:
    forte::core::io::IODeviceController* createDeviceController(CDeviceExecution &paDeviceExecution);
  
    void setConfig();

    virtual void onStartup();
    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(PLC01A1ConfigFB, forte::core::io::IOConfigFBController){}

    CIEC_BOOL var_QI;
    CIEC_WSTRING var_IN1;
    CIEC_WSTRING var_IN2;
    CIEC_WSTRING var_IN3;
    CIEC_WSTRING var_IN4;
    CIEC_WSTRING var_IN5;
    CIEC_WSTRING var_IN6;
    CIEC_WSTRING var_IN7;
    CIEC_WSTRING var_IN8;
    CIEC_WSTRING var_OUT1;
    CIEC_WSTRING var_OUT2;
    CIEC_WSTRING var_OUT3;
    CIEC_WSTRING var_OUT4;
    CIEC_WSTRING var_OUT5;
    CIEC_WSTRING var_OUT6;
    CIEC_WSTRING var_OUT7;
    CIEC_WSTRING var_OUT8;
    CIEC_UINT var_UpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_WSTRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_IN1;
    CDataConnection *conn_IN2;
    CDataConnection *conn_IN3;
    CDataConnection *conn_IN4;
    CDataConnection *conn_IN5;
    CDataConnection *conn_IN6;
    CDataConnection *conn_IN7;
    CDataConnection *conn_IN8;
    CDataConnection *conn_OUT1;
    CDataConnection *conn_OUT2;
    CDataConnection *conn_OUT3;
    CDataConnection *conn_OUT4;
    CDataConnection *conn_OUT5;
    CDataConnection *conn_OUT6;
    CDataConnection *conn_OUT7;
    CDataConnection *conn_OUT8;
    CDataConnection *conn_UpdateInterval;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_WSTRING &paIN1, const CIEC_WSTRING &paIN2, const CIEC_WSTRING &paIN3, const CIEC_WSTRING &paIN4, const CIEC_WSTRING &paIN5, const CIEC_WSTRING &paIN6, const CIEC_WSTRING &paIN7, const CIEC_WSTRING &paIN8, const CIEC_WSTRING &paOUT1, const CIEC_WSTRING &paOUT2, const CIEC_WSTRING &paOUT3, const CIEC_WSTRING &paOUT4, const CIEC_WSTRING &paOUT5, const CIEC_WSTRING &paOUT6, const CIEC_WSTRING &paOUT7, const CIEC_WSTRING &paOUT8, const CIEC_UINT &paUpdateInterval, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_IN1 = paIN1;
      var_IN2 = paIN2;
      var_IN3 = paIN3;
      var_IN4 = paIN4;
      var_IN5 = paIN5;
      var_IN6 = paIN6;
      var_IN7 = paIN7;
      var_IN8 = paIN8;
      var_OUT1 = paOUT1;
      var_OUT2 = paOUT2;
      var_OUT3 = paOUT3;
      var_OUT4 = paOUT4;
      var_OUT5 = paOUT5;
      var_OUT6 = paOUT6;
      var_OUT7 = paOUT7;
      var_OUT8 = paOUT8;
      var_UpdateInterval = paUpdateInterval;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_WSTRING &paIN1, const CIEC_WSTRING &paIN2, const CIEC_WSTRING &paIN3, const CIEC_WSTRING &paIN4, const CIEC_WSTRING &paIN5, const CIEC_WSTRING &paIN6, const CIEC_WSTRING &paIN7, const CIEC_WSTRING &paIN8, const CIEC_WSTRING &paOUT1, const CIEC_WSTRING &paOUT2, const CIEC_WSTRING &paOUT3, const CIEC_WSTRING &paOUT4, const CIEC_WSTRING &paOUT5, const CIEC_WSTRING &paOUT6, const CIEC_WSTRING &paOUT7, const CIEC_WSTRING &paOUT8, const CIEC_UINT &paUpdateInterval, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      evt_INIT(paQI, paIN1, paIN2, paIN3, paIN4, paIN5, paIN6, paIN7, paIN8, paOUT1, paOUT2, paOUT3, paOUT4, paOUT5, paOUT6, paOUT7, paOUT8, paUpdateInterval, paQO, paSTATUS);
    }
};

