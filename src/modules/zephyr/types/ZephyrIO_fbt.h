/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ZephyrIO
 *** Description: Template for Modular IO with boards running Zephyr OS
 *** Version:
 ***     1.0: 2024-01-12/Dirk Kaar -  -
 *************************************************************************/

#pragma once

#include <types/ZephyrIOBase.h>

class FORTE_ZephyrIO final : public FORTE_ZephyrIOBase {
  DECLARE_FIRMWARE_FB(FORTE_ZephyrIO)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventINITOID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    // using FORTE_ZephyrIOBase::executeEvent

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    static constexpr size_t numberOfIOs = 0;

    void setConfig() override;

  protected:
    void onStartup(CEventChainExecutionThread * const paECET) override;

  public:
    FORTE_ZephyrIO(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_TIME var_UpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_STRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_STATUS;

    CEventConnection conn_INITO;

    CDataConnection *conn_QI;
    CDataConnection *conn_UpdateInterval;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_TIME &paUpdateInterval, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_UpdateInterval = paUpdateInterval;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_TIME &paUpdateInterval, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paUpdateInterval, paQO, paSTATUS);
    }
};


