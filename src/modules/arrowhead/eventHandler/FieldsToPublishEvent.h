/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: FieldsToPublishEvent
 *** Description: Template for a simple Basic Function Block Type
 *** Version: 
 ***     1.0: 2019-09-17/Cabral - fortiss GmbH - 
 *************************************************************************/

#ifndef _FIELDSTOPUBLISHEVENT_H_
#define _FIELDSTOPUBLISHEVENT_H_

#include <basicfb.h>
#include <forte_publishevent.h>
#include <forte_arrowheadsystem.h>
#include <forte_arrowheadevent.h>
#include <forte_wstring.h>

class FORTE_FieldsToPublishEvent: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToPublishEvent)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ArrowheadSystem &source() {
    return *static_cast<CIEC_ArrowheadSystem*>(getDI(0));
  };

  CIEC_ArrowheadEvent &event() {
    return *static_cast<CIEC_ArrowheadEvent*>(getDI(1));
  };

  CIEC_WSTRING &deliveryCompleteUri() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_PublishEvent &publishEvent() {
    return *static_cast<CIEC_PublishEvent*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 3, 1, 0, 0);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART(void);
  void enterStateREQ(void);

  virtual void executeEvent(int pa_nEIID);

public:
  FORTE_FieldsToPublishEvent(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~FORTE_FieldsToPublishEvent(){};

};

#endif //close the ifdef sequence from the beginning of the file

