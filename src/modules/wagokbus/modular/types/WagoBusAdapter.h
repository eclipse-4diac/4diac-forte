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

    ~FORTE_WagoBusAdapter() override = default;

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

    TEventID INITO(){
      return mParentAdapterListEventID + scmEventINITOID;
    }

    TEventID INIT(){
      return mParentAdapterListEventID + scmEventINITID;
    }

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];

    static const TForteInt16 scmEIWithIndexes[];
    static const TDataIOID scmEIWith[];
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TForteInt16 scmEOWithIndexes[];
    static const TDataIOID scmEOWith[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpecSocket;
    static const SFBInterfaceSpec scmFBInterfaceSpecPlug;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINITID = 0;


};

#endif //close the ifdef sequence from the beginning of the file

