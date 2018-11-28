/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1504_5
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#ifndef _WAGO1504_5_H_
#define _WAGO1504_5_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "WagoBusAdapter.h"
#include "wagoSlaveBase.h"

class FORTE_Wago1504_5: public WagoSlaveBase{
  DECLARE_FIRMWARE_FB(FORTE_Wago1504_5)

  public:
    FUNCTION_BLOCK_CTOR_FOR_WAGO_SLAVES(FORTE_Wago1504_5, 36866){
    };

    virtual ~FORTE_Wago1504_5(){};

  protected:

    INIT_HANLDLES(0, 16, 0, 0)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_WSTRING &DigitalOutput_1(){
      return *static_cast<CIEC_WSTRING*>(getDI(1));
    };

    CIEC_WSTRING &DigitalOutput_2(){
      return *static_cast<CIEC_WSTRING*>(getDI(2));
    };

    CIEC_WSTRING &DigitalOutput_3(){
      return *static_cast<CIEC_WSTRING*>(getDI(3));
    };

    CIEC_WSTRING &DigitalOutput_4(){
      return *static_cast<CIEC_WSTRING*>(getDI(4));
    };

    CIEC_WSTRING &DigitalOutput_5(){
      return *static_cast<CIEC_WSTRING*>(getDI(5));
    };

    CIEC_WSTRING &DigitalOutput_6(){
      return *static_cast<CIEC_WSTRING*>(getDI(6));
    };

    CIEC_WSTRING &DigitalOutput_7(){
      return *static_cast<CIEC_WSTRING*>(getDI(7));
    };

    CIEC_WSTRING &DigitalOutput_8(){
      return *static_cast<CIEC_WSTRING*>(getDI(8));
    };

    CIEC_WSTRING &DigitalOutput_9(){
      return *static_cast<CIEC_WSTRING*>(getDI(9));
    };

    CIEC_WSTRING &DigitalOutput_10(){
      return *static_cast<CIEC_WSTRING*>(getDI(10));
    };

    CIEC_WSTRING &DigitalOutput_11(){
      return *static_cast<CIEC_WSTRING*>(getDI(11));
    };

    CIEC_WSTRING &DigitalOutput_12(){
      return *static_cast<CIEC_WSTRING*>(getDI(12));
    };

    CIEC_WSTRING &DigitalOutput_13(){
      return *static_cast<CIEC_WSTRING*>(getDI(13));
    };

    CIEC_WSTRING &DigitalOutput_14(){
      return *static_cast<CIEC_WSTRING*>(getDI(14));
    };

    CIEC_WSTRING &DigitalOutput_15(){
      return *static_cast<CIEC_WSTRING*>(getDI(15));
    };

    CIEC_WSTRING &DigitalOutput_16(){
      return *static_cast<CIEC_WSTRING*>(getDI(16));
    };

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS(){
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SAdapterInstanceDef scm_astAdapterInstances[];

    FORTE_WagoBusAdapter& BusAdapterOut(){
      return (*static_cast<FORTE_WagoBusAdapter*>(m_apoAdapters[0]));
    };
    static const int scm_nBusAdapterOutAdpNum = 0;
    FORTE_WagoBusAdapter& BusAdapterIn(){
      return (*static_cast<FORTE_WagoBusAdapter*>(m_apoAdapters[1]));
    };
    static const int scm_nBusAdapterInAdpNum = 1;
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 17, 2, 2);

};

#endif //close the ifdef sequence from the beginning of the file

