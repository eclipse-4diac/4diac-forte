/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPCUA_NodeID
 *** Description: Basic Function Block Type
 *** Version: 
 ***     0.0: 2016-05-09/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#ifndef _OPCUA_NODEID_H_
#define _OPCUA_NODEID_H_

#include <basicfb.h>
#include <forte_usint.h>
#include <forte_any.h>


#include <forte_uint.h>
#include "AOPCUANodeID.h"

class FORTE_OPCUA_NodeID: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_OPCUA_NodeID)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_UINT &nsIndex() {
    return *static_cast<CIEC_UINT*>(getDI(0));
  };

  CIEC_USINT &identifierType() {
    return *static_cast<CIEC_USINT*>(getDI(1));
  };

  CIEC_ANY &identifier() {
    return *static_cast<CIEC_ANY*>(getDI(2));
  };

  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventREQID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventCNFID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_AOPCUANodeID& AOPCUANodeID() {
    return (*static_cast<FORTE_AOPCUANodeID*>(m_apoAdapters[0]));
  };
  static const int scm_nAOPCUANodeIDAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(2, 3, 0, 0, 1);
  void alg_sendNodeId(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateState = 1;
  static const TForteInt16 scm_nStateState_1 = 2;

  void enterStateSTART(void);
  void enterStateState(void);
  void enterStateState_1(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_OPCUA_NodeID(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_OPCUA_NodeID(){};

};

#endif //close the ifdef sequence from the beginning of the file

