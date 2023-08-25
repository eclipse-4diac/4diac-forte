/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: WagoMaster
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-09-12/cabral - null -
 *************************************************************************/

#ifndef _WAGOMASTER_H_
#define _WAGOMASTER_H_

#include "../../../../core/io/configFB/io_master_multi.h"
#include "WagoBusAdapter.h"

class FORTE_WagoMaster : public forte::core::io::IOConfigFBMultiMaster {
  DECLARE_FIRMWARE_FB(FORTE_WagoMaster)

  public:
    FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(FORTE_WagoMaster, forte::core::io::IOConfigFBMultiMaster){
    };

    ~FORTE_WagoMaster() override = default;

    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS(){
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_BOOL &UpdateInterval(){
      return *static_cast<CIEC_BOOL*>(getDI(1));
    };

    FORTE_WagoBusAdapter& BusAdapterOut(){
      return (*static_cast<FORTE_WagoBusAdapter*>(mAdapters[0]));
    };

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];

    static const TEventID scmEventINITID = 0;
    static const TForteInt16 scmEIWithIndexes[];
    static const TDataIOID scmEIWith[];
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TForteInt16 scmEOWithIndexes[];
    static const TDataIOID scmEOWith[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SAdapterInstanceDef scmAdapterInstances[];

    static const int scmBusAdapterOutAdpNum = 0;
    static const SFBInterfaceSpec scmFBInterfaceSpec;


  virtual void setInitialValues();

  forte::core::io::IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution);

    void setConfig();

};

#endif //close the ifdef sequence from the beginning of the file

