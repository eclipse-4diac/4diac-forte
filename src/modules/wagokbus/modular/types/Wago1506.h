/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1506
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#pragma once

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include <forte_string.h>
#include "WagoBusAdapter.h"
#include "wagoSlaveBase.h"

class FORTE_Wago1506: public WagoSlaveBase {
  DECLARE_FIRMWARE_FB(FORTE_Wago1506)

  public:
    FORTE_Wago1506(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~FORTE_Wago1506() override = default;

    CIEC_BOOL var_QI;
    CIEC_STRING var_DigitalInput_1;
    CIEC_STRING var_DigitalInput_2;
    CIEC_STRING var_DigitalInput_3;
    CIEC_STRING var_DigitalInput_4;
    CIEC_STRING var_DigitalInput_5;
    CIEC_STRING var_DigitalInput_6;
    CIEC_STRING var_DigitalInput_7;
    CIEC_STRING var_DigitalInput_8;
    CIEC_STRING var_DigitalOutput_1;
    CIEC_STRING var_DigitalOutput_2;
    CIEC_STRING var_DigitalOutput_3;
    CIEC_STRING var_DigitalOutput_4;
    CIEC_STRING var_DigitalOutput_5;
    CIEC_STRING var_DigitalOutput_6;
    CIEC_STRING var_DigitalOutput_7;
    CIEC_STRING var_DigitalOutput_8;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_WSTRING var_conn_STATUS;

    CEventConnection conn_MAPO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_DigitalInput_1;
    CDataConnection *conn_DigitalInput_2;
    CDataConnection *conn_DigitalInput_3;
    CDataConnection *conn_DigitalInput_4;
    CDataConnection *conn_DigitalInput_5;
    CDataConnection *conn_DigitalInput_6;
    CDataConnection *conn_DigitalInput_7;
    CDataConnection *conn_DigitalInput_8;
    CDataConnection *conn_DigitalOutput_1;
    CDataConnection *conn_DigitalOutput_2;
    CDataConnection *conn_DigitalOutput_3;
    CDataConnection *conn_DigitalOutput_4;
    CDataConnection *conn_DigitalOutput_5;
    CDataConnection *conn_DigitalOutput_6;
    CDataConnection *conn_DigitalOutput_7;
    CDataConnection *conn_DigitalOutput_8;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    FORTE_WagoBusAdapter &var_BusAdapterIn() {
      return *static_cast<FORTE_WagoBusAdapter*>(mAdapters[0]);
    };

    FORTE_WagoBusAdapter &var_BusAdapterOut() {
      return *static_cast<FORTE_WagoBusAdapter*>(mAdapters[1]);
    };

    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_MAP(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalInput_1, const CIEC_STRING &paDigitalInput_2, const CIEC_STRING &paDigitalInput_3, const CIEC_STRING &paDigitalInput_4, const CIEC_STRING &paDigitalInput_5, const CIEC_STRING &paDigitalInput_6, const CIEC_STRING &paDigitalInput_7, const CIEC_STRING &paDigitalInput_8, const CIEC_STRING &paDigitalOutput_1, const CIEC_STRING &paDigitalOutput_2, const CIEC_STRING &paDigitalOutput_3, const CIEC_STRING &paDigitalOutput_4, const CIEC_STRING &paDigitalOutput_5, const CIEC_STRING &paDigitalOutput_6, const CIEC_STRING &paDigitalOutput_7, const CIEC_STRING &paDigitalOutput_8, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_DigitalInput_1 = paDigitalInput_1;
      var_DigitalInput_2 = paDigitalInput_2;
      var_DigitalInput_3 = paDigitalInput_3;
      var_DigitalInput_4 = paDigitalInput_4;
      var_DigitalInput_5 = paDigitalInput_5;
      var_DigitalInput_6 = paDigitalInput_6;
      var_DigitalInput_7 = paDigitalInput_7;
      var_DigitalInput_8 = paDigitalInput_8;
      var_DigitalOutput_1 = paDigitalOutput_1;
      var_DigitalOutput_2 = paDigitalOutput_2;
      var_DigitalOutput_3 = paDigitalOutput_3;
      var_DigitalOutput_4 = paDigitalOutput_4;
      var_DigitalOutput_5 = paDigitalOutput_5;
      var_DigitalOutput_6 = paDigitalOutput_6;
      var_DigitalOutput_7 = paDigitalOutput_7;
      var_DigitalOutput_8 = paDigitalOutput_8;
      executeEvent(scmEventMAPID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paDigitalInput_1, const CIEC_STRING &paDigitalInput_2, const CIEC_STRING &paDigitalInput_3, const CIEC_STRING &paDigitalInput_4, const CIEC_STRING &paDigitalInput_5, const CIEC_STRING &paDigitalInput_6, const CIEC_STRING &paDigitalInput_7, const CIEC_STRING &paDigitalInput_8, const CIEC_STRING &paDigitalOutput_1, const CIEC_STRING &paDigitalOutput_2, const CIEC_STRING &paDigitalOutput_3, const CIEC_STRING &paDigitalOutput_4, const CIEC_STRING &paDigitalOutput_5, const CIEC_STRING &paDigitalOutput_6, const CIEC_STRING &paDigitalOutput_7, const CIEC_STRING &paDigitalOutput_8, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      evt_MAP(paQI, paDigitalInput_1, paDigitalInput_2, paDigitalInput_3, paDigitalInput_4, paDigitalInput_5, paDigitalInput_6, paDigitalInput_7, paDigitalInput_8, paDigitalOutput_1, paDigitalOutput_2, paDigitalOutput_3, paDigitalOutput_4, paDigitalOutput_5, paDigitalOutput_6, paDigitalOutput_7, paDigitalOutput_8, paQO, paSTATUS);
    }

  protected:

    INIT_HANLDLES(8, 8, 0, 0)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventMAPID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventMAPOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const int scmBusAdapterInAdpNum = 0;
    static const int scmBusAdapterOutAdpNum = 1;
    static const SAdapterInstanceDef scmAdapterInstances[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;
};
