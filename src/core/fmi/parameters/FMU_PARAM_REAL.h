/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: FMU_PARAM_REAL
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2017-08-11/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#ifndef _FMU_PARAM_REAL_H_
#define _FMU_PARAM_REAL_H_

#include <funcbloc.h>
#include <forte_lreal.h>

class FORTE_FMU_PARAM_REAL: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_FMU_PARAM_REAL)

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_LREAL &PARAM() {
    return *static_cast<CIEC_LREAL*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 0, 1, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_FMU_PARAM_REAL){
    };

  virtual ~FORTE_FMU_PARAM_REAL(){};

};

#endif //close the ifdef sequence from the beginning of the file

