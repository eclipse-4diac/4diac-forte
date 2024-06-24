/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: X20AI4622
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2011-08-01/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#pragma once

#include "PowerlinkFunctionBlockAI.h"
#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include "forte_string.h"
#include "forte_int.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_X20AI4622 final : public PowerlinkFunctionBlockAI {
  DECLARE_FIRMWARE_FB(FORTE_X20AI4622)

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
    FORTE_X20AI4622(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_USINT var_CNID;
    CIEC_UINT var_MODID;

    CIEC_BOOL var_QO;
    CIEC_USINT var_CNIDO;
    CIEC_STRING var_STATUS;
    CIEC_INT var_AI01;
    CIEC_INT var_AI02;
    CIEC_INT var_AI03;
    CIEC_INT var_AI04;

    CIEC_BOOL var_conn_QO;
    CIEC_USINT var_conn_CNIDO;
    CIEC_STRING var_conn_STATUS;
    CIEC_INT var_conn_AI01;
    CIEC_INT var_conn_AI02;
    CIEC_INT var_conn_AI03;
    CIEC_INT var_conn_AI04;

    CEventConnection conn_INITO;
    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_CNID;
    CDataConnection *conn_MODID;

    CDataConnection conn_QO;
    CDataConnection conn_CNIDO;
    CDataConnection conn_STATUS;
    CDataConnection conn_AI01;
    CDataConnection conn_AI02;
    CDataConnection conn_AI03;
    CDataConnection conn_AI04;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_INT &paAI01, CIEC_INT &paAI02, CIEC_INT &paAI03, CIEC_INT &paAI04) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
      paAI01 = var_AI01;
      paAI02 = var_AI02;
      paAI03 = var_AI03;
      paAI04 = var_AI04;
    }

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_INT &paAI01, CIEC_INT &paAI02, CIEC_INT &paAI03, CIEC_INT &paAI04) {
      var_QI = paQI;
      var_CNID = paCNID;
      var_MODID = paMODID;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paCNIDO = var_CNIDO;
      paSTATUS = var_STATUS;
      paAI01 = var_AI01;
      paAI02 = var_AI02;
      paAI03 = var_AI03;
      paAI04 = var_AI04;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_USINT &paCNID, const CIEC_UINT &paMODID, CIEC_BOOL &paQO, CIEC_USINT &paCNIDO, CIEC_STRING &paSTATUS, CIEC_INT &paAI01, CIEC_INT &paAI02, CIEC_INT &paAI03, CIEC_INT &paAI04) {
      evt_INIT(paQI, paCNID, paMODID, paQO, paCNIDO, paSTATUS, paAI01, paAI02, paAI03, paAI04);
    }
};


