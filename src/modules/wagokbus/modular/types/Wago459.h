/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago459
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

class FORTE_Wago459: public WagoSlaveBase{
  DECLARE_FIRMWARE_FB(FORTE_Wago459)

  public:
    FORTE_Wago459(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~FORTE_Wago459() override = default;

    CIEC_BOOL var_QI;
    CIEC_STRING var_AnalogInput_1;
    CIEC_STRING var_AnalogInput_2;
    CIEC_STRING var_AnalogInput_3;
    CIEC_STRING var_AnalogInput_4;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_WSTRING var_conn_STATUS;

    CEventConnection conn_MAPO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_AnalogInput_1;
    CDataConnection *conn_AnalogInput_2;
    CDataConnection *conn_AnalogInput_3;
    CDataConnection *conn_AnalogInput_4;

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

    void evt_MAP(const CIEC_BOOL &paQI, const CIEC_STRING &paAnalogInput_1, const CIEC_STRING &paAnalogInput_2, const CIEC_STRING &paAnalogInput_3, const CIEC_STRING &paAnalogInput_4, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_AnalogInput_1 = paAnalogInput_1;
      var_AnalogInput_2 = paAnalogInput_2;
      var_AnalogInput_3 = paAnalogInput_3;
      var_AnalogInput_4 = paAnalogInput_4;
      executeEvent(scmEventMAPID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paAnalogInput_1, const CIEC_STRING &paAnalogInput_2, const CIEC_STRING &paAnalogInput_3, const CIEC_STRING &paAnalogInput_4, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      evt_MAP(paQI, paAnalogInput_1, paAnalogInput_2, paAnalogInput_3, paAnalogInput_4, paQO, paSTATUS);
    }

  protected:

    INIT_HANLDLES(0, 0, 4, 0)

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
