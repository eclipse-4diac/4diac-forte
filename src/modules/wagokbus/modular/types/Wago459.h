/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago459
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium -
 *************************************************************************/

#ifndef _WAGO459_H_
#define _WAGO459_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "WagoBusAdapter.h"
#include "wagoSlaveBase.h"

class FORTE_Wago459: public WagoSlaveBase{
  DECLARE_FIRMWARE_FB(FORTE_Wago459)

  public:
    FUNCTION_BLOCK_CTOR_FOR_WAGO_SLAVES(FORTE_Wago459, 459){
    };

    ~FORTE_Wago459() override = default;

  protected:

    INIT_HANLDLES(0, 0, 4, 0)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_WSTRING &AnalogInput_1(){
      return *static_cast<CIEC_WSTRING*>(getDI(1));
    };

    CIEC_WSTRING &AnalogInput_2(){
      return *static_cast<CIEC_WSTRING*>(getDI(2));
    };

    CIEC_WSTRING &AnalogInput_3(){
      return *static_cast<CIEC_WSTRING*>(getDI(3));
    };

    CIEC_WSTRING &AnalogInput_4(){
      return *static_cast<CIEC_WSTRING*>(getDI(4));
    };

    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS(){
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    static const TForteInt16 scmEIWithIndexes[];
    static const TDataIOID scmEIWith[];
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TForteInt16 scmEOWithIndexes[];
    static const TDataIOID scmEOWith[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SAdapterInstanceDef scmAdapterInstances[];

    FORTE_WagoBusAdapter& BusAdapterOut(){
      return (*static_cast<FORTE_WagoBusAdapter*>(mAdapters[0]));
    };
    static const int scmBusAdapterOutAdpNum = 0;
    FORTE_WagoBusAdapter& BusAdapterIn(){
      return (*static_cast<FORTE_WagoBusAdapter*>(mAdapters[1]));
    };
    static const int scmBusAdapterInAdpNum = 1;
    static const SFBInterfaceSpec scmFBInterfaceSpec;


};

#endif //close the ifdef sequence from the beginning of the file

