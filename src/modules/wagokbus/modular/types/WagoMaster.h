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

    virtual ~FORTE_WagoMaster(){};

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
      return (*static_cast<FORTE_WagoBusAdapter*>(m_apoAdapters[0]));
    };

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

    static const TEventID scm_nEventINITID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventINDID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SAdapterInstanceDef scm_astAdapterInstances[];

    static const int scm_nBusAdapterOutAdpNum = 0;
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 2, 2, 1);

  virtual void setInitialValues();

  forte::core::io::IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution);

    void setConfig();

};

#endif //close the ifdef sequence from the beginning of the file

