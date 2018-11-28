/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: WagoBusAdapter
 *** Description:
 *** Version:
 *************************************************************************/

#ifndef _WAGOBUSADAPTER_H_
#define _WAGOBUSADAPTER_H_

#include "../../../../core/io/configFB/io_adapter_multi.h"
#include <typelib.h>
#include <forte_bool.h>
#include <forte_uint.h>

class FORTE_WagoBusAdapter : public forte::core::io::IOConfigFBMultiAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_WagoBusAdapter)

  public:

    ADAPTER_CTOR_FOR_IO_MULTI (FORTE_WagoBusAdapter){
    };

    static const TForteUInt8 scmSlaveConfigurationIO[];
    static const TForteUInt8 scmSlaveConfigurationIONum;

    virtual ~FORTE_WagoBusAdapter(){};

    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
    };

    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>((isSocket()) ? getDO(0) : getDI(0));
    };

    CIEC_UINT &MasterId(){
      return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(1) : getDI(1));
    };

    CIEC_UINT &Index(){
      return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(2) : getDI(2));
    };

    int INITO(){
      return m_nParentAdapterListEventID + scm_nEventINITOID;
    }

    int INIT(){
      return m_nParentAdapterListEventID + scm_nEventINITID;
    }

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;
    static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventINITID = 0;

    FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 3, 0);
};

#endif //close the ifdef sequence from the beginning of the file

