/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: NVS
 *** Description: Load and Store Data from NVS by Key
 *** Version:
 ***     1.0: 2022-09-23/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_any_elementary_variant.h"
#include "forte_bool.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"




#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"



class FORTE_NVS final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_NVS)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventSETID = 1;
    static const TEventID scmEventGETID = 2;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventSETOID = 1;
    static const TEventID scmEventGETOID = 2;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void executeSet();
    void executeGet();
    void executeInit();

    // Handle will automatically close when going out of scope or when it's reset.
    static std::shared_ptr<nvs::NVSHandle> handle;

    const char *key;

    static const CIEC_STRING scmOK;
    static const CIEC_STRING scmNotInitialised;
    static const CIEC_STRING scmInitialised;
    static const CIEC_STRING scmERR;
    static const CIEC_STRING scmERR_NVS_NOT_FOUND;



    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_NVS(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_KEY;
    CIEC_ANY_ELEMENTARY_VARIANT var_VALUE;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;
    CIEC_ANY_ELEMENTARY_VARIANT var_VALUEO;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;
    CIEC_ANY_ELEMENTARY_VARIANT var_conn_VALUEO;

    CEventConnection conn_INITO;
    CEventConnection conn_SETO;
    CEventConnection conn_GETO;

    CDataConnection *conn_QI;
    CDataConnection *conn_KEY;
    CDataConnection *conn_VALUE;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;
    CDataConnection conn_VALUEO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paKEY, const CIEC_ANY_ELEMENTARY &paVALUE, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS, CIEC_ANY_ELEMENTARY &paVALUEO) {
      var_QI = paQI;
      var_KEY = paKEY;
      var_VALUE = paVALUE;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
      paVALUEO.setValue(var_VALUEO.unwrap());
    }

    void evt_SET(const CIEC_BOOL &paQI, const CIEC_STRING &paKEY, const CIEC_ANY_ELEMENTARY &paVALUE, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS, CIEC_ANY_ELEMENTARY &paVALUEO) {
      var_QI = paQI;
      var_KEY = paKEY;
      var_VALUE = paVALUE;
      executeEvent(scmEventSETID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
      paVALUEO.setValue(var_VALUEO.unwrap());
    }

    void evt_GET(const CIEC_BOOL &paQI, const CIEC_STRING &paKEY, const CIEC_ANY_ELEMENTARY &paVALUE, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS, CIEC_ANY_ELEMENTARY &paVALUEO) {
      var_QI = paQI;
      var_KEY = paKEY;
      var_VALUE = paVALUE;
      executeEvent(scmEventGETID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
      paVALUEO.setValue(var_VALUEO.unwrap());
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paKEY, const CIEC_ANY_ELEMENTARY &paVALUE, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS, CIEC_ANY_ELEMENTARY &paVALUEO) {
      evt_INIT(paQI, paKEY, paVALUE, paQO, paSTATUS, paVALUEO);
    }
};

