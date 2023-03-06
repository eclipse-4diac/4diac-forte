/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SET_STRUCT_VALUE
 *** Description: Service Interface Function Block Type
 *** Version:
***     1.0: 2018-10-16/Jose Cabral -  - 
 *************************************************************************/

#ifndef _SET_STRUCT_VALUE_H_
#define _SET_STRUCT_VALUE_H_

#include "funcbloc.h"
#include "forte_any.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_SET_STRUCT_VALUE: public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_SET_STRUCT_VALUE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  
  static const TEventID scm_nEventREQID = 0;
  
   static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  
  static const TEventID scm_nEventCNFID = 0;
  
   static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];
  

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  CIEC_ANY &st_in_struct() {
    return *static_cast<CIEC_ANY*>(getDI(0));
  }
  
  CIEC_STRING &st_member() {
    return *static_cast<CIEC_STRING*>(getDI(1));
  }
  
  CIEC_ANY &st_element_value() {
    return *static_cast<CIEC_ANY*>(getDI(2));
  }
  
  CIEC_ANY &st_out_struct() {
    return *static_cast<CIEC_ANY*>(getDO(0));
  }
  

  FORTE_FB_DATA_ARRAY(1, 3, 1, 0);

  void executeEvent(int paEIID);

public:
   FORTE_SET_STRUCT_VALUE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
   };

  ~FORTE_SET_STRUCT_VALUE() override = default;

private:
  bool checkStructPorts();
};

#endif // _SET_STRUCT_VALUE_H_


