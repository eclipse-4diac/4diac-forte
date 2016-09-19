/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: AOPCUANodeID
 *** Description: null
 *** Version: 
 *************************************************************************/

#ifndef _AOPCUANODEID_H_
#define _AOPCUANODEID_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_usint.h>
#include <forte_any.h>


#include <forte_uint.h>

class FORTE_AOPCUANodeID: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_AOPCUANodeID)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_UINT &nsIndex() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_USINT &identifierType() {
    return *static_cast<CIEC_USINT*>((isSocket()) ? getDO(1) : getDI(1));
  };

  CIEC_ANY &identifier() {
    return *static_cast<CIEC_ANY*>((isSocket()) ? getDO(2) : getDI(2));
  };

 public:
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
 public:
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(0, 0, 0, 3, 0);

public:
  ADAPTER_CTOR(FORTE_AOPCUANodeID){
  };

  virtual ~FORTE_AOPCUANodeID(){};

};

#endif //close the ifdef sequence from the beginning of the file

