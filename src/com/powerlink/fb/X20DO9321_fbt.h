/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: X20DO9321
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2012-01-30/4DIAC-IDE - 4DIAC-Consortium -
 ***     1.0: 2011-05-31/FA,TS - AIT -
 *************************************************************************/

#pragma once

#include "PowerlinkFunctionBlockDO.h"
#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_X20DO9321 final : public PowerlinkFunctionBlockDO {
  DECLARE_FIRMWARE_FB(FORTE_X20DO9321)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_X20DO9321(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_CNID;
    CIEC_UINT var_MODID;
    CIEC_BOOL var_DO01;
    CIEC_BOOL var_DO02;
    CIEC_BOOL var_DO03;
    CIEC_BOOL var_DO04;
    CIEC_BOOL var_DO05;
    CIEC_BOOL var_DO06;
    CIEC_BOOL var_DO07;
    CIEC_BOOL var_DO08;
    CIEC_BOOL var_DO09;
    CIEC_BOOL var_DO10;
    CIEC_BOOL var_DO11;
    CIEC_BOOL var_DO12;

    CIEC_BOOL var_QO;
    CIEC_USINT var_CNIDO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_USINT var_conn_CNIDO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_CNID;
    CDataConnection *conn_MODID;
    CDataConnection *conn_DO01;
    CDataConnection *conn_DO02;
    CDataConnection *conn_DO03;
    CDataConnection *conn_DO04;
    CDataConnection *conn_DO05;
    CDataConnection *conn_DO06;
    CDataConnection *conn_DO07;
    CDataConnection *conn_DO08;
    CDataConnection *conn_DO09;
    CDataConnection *conn_DO10;
    CDataConnection *conn_DO11;
    CDataConnection *conn_DO12;

    CDataConnection conn_QO;
    CDataConnection conn_CNIDO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, const CIEC_BOOL &paDO01, const CIEC_BOOL &paDO02, const CIEC_BOOL &paDO03, const CIEC_BOOL &paDO04, const CIEC_BOOL &paDO05, const CIEC_BOOL &paDO06, const CIEC_BOOL &paDO07, const CIEC_BOOL &paDO08, const CIEC_BOOL &paDO09, const CIEC_BOOL &paDO10, const CIEC_BOOL &paDO11, const CIEC_BOOL &paDO12, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      var_DO01 = paDO01;
      var_DO02 = paDO02;
      var_DO03 = paDO03;
      var_DO04 = paDO04;
      var_DO05 = paDO05;
      var_DO06 = paDO06;
      var_DO07 = paDO07;
      var_DO08 = paDO08;
      var_DO09 = paDO09;
      var_DO10 = paDO10;
      var_DO11 = paDO11;
      var_DO12 = paDO12;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
    }

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, const CIEC_BOOL &paDO01, const CIEC_BOOL &paDO02, const CIEC_BOOL &paDO03, const CIEC_BOOL &paDO04, const CIEC_BOOL &paDO05, const CIEC_BOOL &paDO06, const CIEC_BOOL &paDO07, const CIEC_BOOL &paDO08, const CIEC_BOOL &paDO09, const CIEC_BOOL &paDO10, const CIEC_BOOL &paDO11, const CIEC_BOOL &paDO12, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      var_DO01 = paDO01;
      var_DO02 = paDO02;
      var_DO03 = paDO03;
      var_DO04 = paDO04;
      var_DO05 = paDO05;
      var_DO06 = paDO06;
      var_DO07 = paDO07;
      var_DO08 = paDO08;
      var_DO09 = paDO09;
      var_DO10 = paDO10;
      var_DO11 = paDO11;
      var_DO12 = paDO12;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, const CIEC_BOOL &paDO01, const CIEC_BOOL &paDO02, const CIEC_BOOL &paDO03, const CIEC_BOOL &paDO04, const CIEC_BOOL &paDO05, const CIEC_BOOL &paDO06, const CIEC_BOOL &paDO07, const CIEC_BOOL &paDO08, const CIEC_BOOL &paDO09, const CIEC_BOOL &paDO10, const CIEC_BOOL &paDO11, const CIEC_BOOL &paDO12, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paCNID, paMODID, paDO01, paDO02, paDO03, paDO04, paDO05, paDO06, paDO07, paDO08, paDO09, paDO10, paDO11, paDO12, paQO, paCNIDO, paSTATUS);
    }
};


